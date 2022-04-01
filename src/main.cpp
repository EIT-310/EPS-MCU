#include "mbed.h"


// identify state by nr (0,1,2,3)
typedef struct stateTp {
  int lastState;
  int state;
} stateTp;


void batHigh(stateTp * stateInfo);
void batMidHigh(stateTp * stateInfo);
void batMid(stateTp * stateInfo);
void batMidLow(stateTp * stateInfo);
void batLow(stateTp * stateInfo);


const int BATHIGH = 0;

const int BATMIDHIGH = 1;

const int BATMID = 2;

const int BATMIDLOW = 3;

const int BATLOW = 4;
int vBat=6; // TODO: Måske float?

void (*stateFunc[])(struct stateTp *ss) = {batHigh, batMidHigh, batMid, batMidLow, batLow};
//                             state         0     1     2    ...

void batHigh(stateTp * stateInfo)
{

  printf("\nHIGH\n");
if (vBat<5)
{
      stateInfo->state = BATMIDHIGH;
}else
{
    stateInfo->state = BATHIGH;
}

  ThisThread::sleep_for(1s);
  return;
}

void batMidHigh(stateTp * stateInfo)
{
  printf("\nMIDHIGH\n");
    if (vBat>5)
    {
        stateInfo->state = BATHIGH;
    }else if (vBat>4)
    {   
        stateInfo->state = BATMIDHIGH;
    }
    else
    {
        stateInfo->state = BATMID;
    }
      ThisThread::sleep_for(1s);
  return;
}

void batMid(stateTp * stateInfo)
{
  printf("\nMID\n");
    if (vBat>4)
    {
        stateInfo->state = BATMIDHIGH;
    }else if (vBat>3)
    {   
        stateInfo->state = BATMID;
    }
    else
    {
        stateInfo->state = BATMIDLOW;
    }
    ThisThread::sleep_for(1s);
    return;
}

void batMidLow(stateTp * stateInfo)
{
  printf("\nMIDLOW\n");
    if (vBat>3)
    {
        stateInfo->state = BATMID;
    }else if (vBat>2)
    {   
        stateInfo->state = BATMIDLOW;
    }
    else
    {
        stateInfo->state = BATLOW;
    }
    ThisThread::sleep_for(1s);
    return;
}

void batLow(stateTp * stateInfo)
{
  printf("\nledOff\n");
    if (vBat>2)
    {
        stateInfo->state = BATMID;
    }
    else
    {
        stateInfo->state = BATLOW;
    }
    ThisThread::sleep_for(1s);

    return;
}

int main()
{
    stateTp curState;

    curState.state = BATMID;
    
    curState.lastState = BATMID;
    printf("\nstarting state machine\n");

    while (1) {
        printf("\nback in scheduler");
        //Læs ADC
        
        (*stateFunc[curState.state])(&curState);


    }
}


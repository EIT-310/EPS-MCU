#include "mbed.h"
#include "rtos.h"
#include "adc_read.h"
#define BATHIGH 0
#define BATMIDHIGH 1
#define BATMID 2
#define BATMIDLOW 3
#define BATLOW 4
#define RADIOPIN D0
#define OBCPIN D1
#define ADCSPIN D2
#define PAYLOADPIN D3

UnbufferedSerial serial(USBTX, USBRX, 2e6);
AdcRead adc;

DigitalOut obc(OBCPIN);
DigitalOut radio(RADIOPIN);
DigitalOut adcs(ADCSPIN);
DigitalOut payload(PAYLOADPIN);

// identify state by nr (0,1,2,3)
int state;
int vBat = 6; // TODO: Måske float?



void getbatHigh(int *stateInfo);
void getbatMidHigh(int *stateInfo);
void getbatMid(int *stateInfo);
void getbatMidLow(int *stateInfo);
void getbatLow(int *stateInfo);

void batHigh();
void batMidHigh();
void batMid();
void batMidLow();
void batLow();

void (*getStateFunc[])(int *ss) = {
  getbatHigh, 
  getbatMidHigh, 
  getbatMid, 
  getbatMidLow, 
  getbatLow
  };

void (*stateFunc[])() = {
  batHigh, 
  batMidHigh, 
  batMid, 
  batMidLow,
  batLow};


void getbatHigh(int *stateInfo)
{

  printf("\ngetHIGH\n");
  vBat= adc.read_int(adc.one);


  if (vBat < 5)
  {
    *stateInfo = BATMIDHIGH;
  }
  else
  {
    *stateInfo = BATHIGH;
  }

  ThisThread::sleep_for(1s);
  return;
}

void getbatMidHigh(int *stateInfo)
{
  printf("\ngetMIDHIGH\n");
  // Læse ADC

  if (vBat > 5)
  {
    *stateInfo = BATHIGH;
  }
  else if (vBat > 4)
  {
    *stateInfo = BATMIDHIGH;
  }
  else
  {
    *stateInfo = BATMID;
  }

  ThisThread::sleep_for(1s);
  return;
}

void getbatMid(int *stateInfo)
{
  printf("\ngetMID\n");
  // Læse ADC
  if (vBat > 4)
  {
    *stateInfo = BATMIDHIGH;
  }
  else if (vBat > 3)
  {
    *stateInfo = BATMID;
  }
  else
  {
    *stateInfo = BATMIDLOW;
  }
  ThisThread::sleep_for(1s);
  return;
}

void getbatMidLow(int *stateInfo)
{
  printf("\ngetMIDLOW\n");
  // Læse ADC
  if (vBat > 3)
  {
    *stateInfo = BATMID;
  }
  else if (vBat > 2)
  {
    *stateInfo = BATMIDLOW;
  }
  else
  {
    *stateInfo = BATLOW;
  }
  ThisThread::sleep_for(1s);
  return;
}

void getbatLow(int *stateInfo)
{
  printf("\ngetBATLOW\n");
  // Læse ADC

  if (vBat > 2)
  {
    *stateInfo = BATMID;
  }
  else
  {
    *stateInfo = BATLOW;
  }
  ThisThread::sleep_for(1s);

  return;
}

// Udører ting

void batHigh()
{
  payload = 1;
  printf("\nHIGH\n");
}

void batMidHigh()
{
  printf("\nMIDHIGH\n");
  // Læse ADC
  obc = 1;
  radio = 1;
  payload = 0;
  adcs = 1;
  return;
}

void batMid()
{
  printf("\nMID\n");
  obc = 0;
  radio = 1;
  payload = 0;
  adcs = 1;
  return;
}

void batMidLow()
{
  printf("\nMIDLOW\n");
    obc = 0;
    radio = 1;
    payload = 0;
    adcs = 1;
  return;
}

void batLow()
{
  printf("\nLOW\n");

  obc = 0;
  radio = 0;
  adcs = 0;

  ThisThread::sleep_for(1s);

  return;
}

int main()
{
  state = BATMID;

  printf("\nstarting state machine\n");

  while (1)
  {
    printf("\nback in scheduler");
    // Læs ADC

    (*getStateFunc[state])(&state);

    (*stateFunc[state])();
  }
}


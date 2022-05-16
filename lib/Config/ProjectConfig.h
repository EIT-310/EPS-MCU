//
// Created by stormand on 01/04/2022.
//

#ifndef EPS_MCU_LIB_PROJECT_CONFIG_H_
#define EPS_MCU_LIB_PROJECT_CONFIG_H_


#ifdef TARGET_NUCLEO_L432KC
// PINS
//ADC'er
#define BAT_V_1_PIN         A0
#define BAT_I_1_PIN         A1
#define MPPT_V_1_PIN        A2
#define MPPT_I_1_PIN	    A3
#define SUB_V_1_PIN	        A4
#define SUB_I_1_PIN	        A5
#define ADC_NUM_CHANNELS    6

//DIVERSE
#define CAN_RD_PIN	        D10
#define CAN_TD_PIN	        D2
#define RAIL_1_PIN	        D4
#define OCE_1_PIN	        D11
#define SUBMODULES_NUM      1


#endif

#ifdef TARGET_NUCLEO_L496ZG
//ADC'er
#define BAT_V_1_PIN         PA_3
#define BAT_I_1_PIN         PC_0
#define MPPT_V_1_PIN        PC_3
#define MPPT_I_1_PIN	    PC_1
#define SUB_V_1_PIN	        PC_4
#define SUB_I_1_PIN	        PC_5
#define ADC_NUM_CHANNELS    6

//DIVERSE
#define CAN_RD_PIN	        PD_0
#define CAN_TD_PIN	        PD_1
#define RAIL_1_PIN	        PB_2
#define OCE_1_PIN	        PC_8
#define SUBMODULES_NUM      1

//// PINS
//#define BAT_V_1_PIN          PA_3
//#define BAT_V_2_PIN          PC_0
//// TODO Define bat current pins
//#define MMPT_V_1_PIN         PC_3
//#define MPPT_I_1_PIN         PC_1
//#define MPPT_V_2_PIN         PC_4
//#define MPPT_I_2_PIN         PC_5
//#define MPPT_V_3_PIN         PF_3
//#define MPPT_I_3_PIN         PF_5
//#define MPPT_V_4_PIN         PF_8
//#define MPPT_I_4_PIN         PF_7
//#define SUB_V_1_PIN          PF_9
//#define SUB_I_1_PIN          PA_4
//#define SUB_V_2_PIN          PB_1
//#define SUB_I_2_PIN          PC_2
//#define SUB_V_3_PIN          PA_1
//#define SUB_I_3_PIN          PA_2
//#define SUB_V_4_PIN          PB_0
//#define SUB_I_4_PIN          PA_0
//#define SUB_V_5_PIN          PA_5
//#define SUB_I_5_PIN          PA_6
//#define ADC_NUM_CHANNELS    20
//
////DIV
//#define CAN_RD_PIN          PD_0
//#define CAN_TD_PIN          PD_1
//#define RAIL_1_PIN          PB_2
//#define RAIL_2_PIN          PB_6
//#define RAIL_3_PIN          PF_2
//#define RAIL_4_PIN          PF_1
//#define RAIL_5_PIN          PF_0
//#define OCE_1_PIN           PC_8
//#define OCE_2_PIN           PC_9
//#define OCE_3_PIN           PC_10
//#define OCE_4_PIN           PC_11
//#define OCE_5_PIN           PC_12
//#define SUBMODULES_NUM      5

#endif

// STATIC CONFIG
#define ADC_MEASURE_LOOPS   5
#define ADC_BUFFER_SIZE     100
#define VBAT_MID_HIGH       4.0f //TODO: Ret så det passer med hvad Vref på ADC er
#define VBAT_MID            3.7f //TODO: Ret så det passer med hvad Vref på ADC er
#define VBAT_MID_LOW        3.4f //TODO: Ret så det passer med hvad Vref på ADC er
#define VBAT_LOW            3.2f //TODO: Ret så det passer med hvad Vref på ADC er
#define VBAT_EMERGENCY      3.0f //TODO: Ret så det passer med hvad Vref på ADC er
#define MPPT_CURR_AMP       200.0f
#define MPPT_SHUNT_RES      0.02f
#define CAN_BITRATE         5e5  // 1 MHz / 500kHz


// MUTABLE CONFIG
#define WATCHDOG_TIMEOUT    10000  //TODO: Evt ret så det giver mening
#define FSM_START_STATE     Fsm::MID
#define CRIT_MPPT_POW_SUM   1.0f   //TODO: ret så det passer

// DEV CONFIG
#define LOG_LEVEL           LOG_DEBUG
#define LOG_COM_TYPE        SERIAL


#endif //EPS_MCU_LIB_PROJECT_CONFIG_H_

#ifndef TIMER_H
#define TIMER_H

/*****************************************************
Before use this, need to call HalTimerInit()
**/

#define   TIMER0              HAL_TIMER_0
#define   TIMER_TIME_OUT      
#define   TIMER_CHANNEL_MODE  HAL_TIMER_CH_MODE_OUTPUT_COMPARE
/*
HAL_TIMER_CH_MODE_INPUT_CAPTURE  Input Capture Mode 
HAL_TIMER_CH_MODE_OUTPUT_COMPARE  Output Compare Mode 
HAL_TIMER_CH_MODE_OVERFLOW  Overflow Mode
*/
#define   TIMER_OPS_MODE      HAL_TIMER_MODE_CTC  //Clear Timer on Compare

extern halTimerCBack_t Timer0ISR(uint8 timerId, uint8 channel, uint8 channelMode);

uint8 TimerConfig(uint8 timer, halTimerCBack_t cback);
uint8 TimerStart(uint8 timer, uint32 tick);
uint8 TimerStop(uint8 timer);

#endif

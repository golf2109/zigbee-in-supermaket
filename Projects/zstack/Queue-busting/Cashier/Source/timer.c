#include <hal_timer.h>
#include "timer.h"


uint8 TimerConfig(uint8 timer, halTimerCBack_t cback);
uint8 TimerStart(uint8 timer, uint32 tick);
uint8 TimerStop(uint8 timer);

halTimerCBack_t Timer0ISR(uint8 timerId, uint8 channel, uint8 channelMode);

uint8 TimerConfig(uint8 timer, halTimerCBack_t cback){
  uint8 ret;
  ret = HalTimerConfig (timer, TIMER_OPS_MODE,
                  HAL_TIMER_CHANNEL_SINGLE,
                  TIMER_CHANNEL_MODE,
                  TRUE, cback);
  if(ret==HAL_TIMER_OK) return 1;
  else return 0;
}

uint8 TimerStart(uint8 timer, uint32 tick){
  uint8 ret = HalTimerStart (timer, tick);
  if(ret==HAL_TIMER_OK) return 1;
  else return 0;
}

uint8 TimerStop(uint8 timer){
  uint8 ret = HalTimerStop (timer);
  if(ret==HAL_TIMER_OK) return 1;
  else return 0;
}

halTimerCBack_t Timer0ISR(uint8 timerId, uint8 channel, uint8 channelMode){
  return 0;
}

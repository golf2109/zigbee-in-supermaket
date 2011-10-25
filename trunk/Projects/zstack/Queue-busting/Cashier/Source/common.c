/*********************************************************************
 * INCLUDES
 */

/*USER CODE*/
#include "common.h"
#include "hal_lcd.h"
#include "OSAL.h"

/*********************************************************************
 * SECURITY
 */

/*********************************************************************
 * CONNECT TO PC
 */
 
 /*********************************************************************
 * SERIAL
 */
 
/*********************************************************************
 * ZIGBEE
 */
 
 /*********************************************************************
 * GLOBAL FUNCTIONS
 */
#if defined( LCD_SUPPORTED )
void printText(char* text, uint8 line){
  if(line==1)
    HalLcdWriteString(text, HAL_LCD_LINE_1 );
  else if(line==2)
    HalLcdWriteString(text, HAL_LCD_LINE_2 );
  else
    HalLcdWriteString(text, HAL_LCD_LINE_3 );
  //HalLcdWriteScreen(text1, text2);
}
#endif

uint8 strcmp(char* txt1, char* txt2){
  uint8 ret = 0;
  uint8 i;
  if(osal_strlen(txt1) > osal_strlen(txt2))
    ret = osal_strlen(txt2);
  else if(osal_strlen(txt1) < osal_strlen(txt2))
    ret = osal_strlen(txt1);
  else{
    for(i=0;i<osal_strlen(txt1);i++)
      if(*(txt1+i) != *(txt2+i)){
        ret = i;
        break;
      }
  }
  return ret;
}

char* int2char(int num){
  char i=0,j=0;
  int tmp = num;
  while(1){
    tmp = tmp/10;
    i++;
    if(tmp<=0) break;
  }
  char* ret = (char*)osal_mem_alloc(sizeof(char)*i);
  for(j=i;j>0;j--){
    *(ret+j) = num%10 + 48;
    num=num/10;
  }
  *(ret+j) = '\0';
  return ret;
}

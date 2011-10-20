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
void printText(char* text1, char* text2){
  HalLcdWriteScreen(text1, text2);
}
#endif

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

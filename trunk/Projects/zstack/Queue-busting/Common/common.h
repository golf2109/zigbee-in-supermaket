#ifndef COMMON_H
#define COMMON_H

#include <hal_types.h>

/*********************************************************************
 * DATA FOR BASKET
 */

#define MAX_PRODS		25		//max products in one basket
#define PRODS_ID_LEN		13		//len of product_id
#define BASKET_ID_LEN		8		//BASKET_ID_FORMAT +7
#define BASKET_ID_FORMAT        '#'
#define PRODS_NUM_SIZE          1
/*
*
*/
typedef struct Product{
  uint8* id;
  uint8  num;
}Product;
/*
*
*/
typedef struct {
	char* id;
	uint8 len;
	Product prods[MAX_PRODS];
}Basket;
/*********************************************************************
 * DATA FROM PC
 */
//[] [len] [cmd]
#define PC_CMD_P2P		'p'
#define PC_CMD_BRD		'b'
/*********************************************************************
 * MODE
 */
#define CONFIG_MODE	1		//only use when user config from scanner
#define NORMAL_MODE	2		//normal operation
/*********************************************************************
 * CODE
 */
//======For Handheld
#define REQUEST_BASKET			'%'	//request for basket_id
#define DEL_BASKET			'^'	//del basket_id
#define ALL_BASKET                      '0'
#define ALL_BASKET_SIZE                  3

#define ADD_PRODS_CODE			'+'	//add product to basket
#define DEL_PRODS_CODE			'-'	//del product out of basket
#define CHANGE_PRODS_LABEL		'@'	//change lable for data



/*********************************************************************
 * SECURITY
 */
#define KEY_SEC		0x123		//use for security

/*********************************************************************
 * CONNECT TO PC
 */
 
 /*********************************************************************
 * SERIAL
 */
 
/*********************************************************************
 * ZIGBEE
 */
#define Q_BUSTING_ENDPOINT           10
#define Q_BUSTING_PROFID             0x0F04
#define Q_BUSTING_DEVICEID           0x0001
#define Q_BUSTING_DEVICE_VERSION     0
#define Q_BUSTING_FLAGS              0
#define Q_BUSTING_MAX_CLUSTERS       1
#define Q_BUSTING_CLUSTERID          1

#define	Q_BUSTING_TIMEOUT			5000	//5 seconds

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * COMMON VARIABLES
 */
extern uint8 mode;
extern uint8 basket_id_len;
extern uint8 product_id_len;

/*********************************************************************
 * COMMON FUNCTIONS
 */
 extern char* int2char(int num);
 extern void printText(char* text, uint8 line);
 extern uint8 strcmp(char* txt1, char* txt2);
 extern uint8 IsSameString(uint8 *a, uint8* b,const uint8 numBytsCompare);
 void CopyString(uint8 *d, uint8* s,const uint8 numByts);
 
#endif
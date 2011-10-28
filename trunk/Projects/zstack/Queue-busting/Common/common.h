#ifndef COMMON_H
#define COMMON_H

#include <hal_types.h>
#include "common.h"
/*********************************************************************
 * DATA FOR BASKET
 */
#define MAX_PRODS			50		//max products in one basket
#define PRODS_ID_LEN		7		//len of product_id
#define PROD_ID_MIN_LEN         7
#define PROD_ID_MAX_LEN         7
#define OFFSET				PRODS_ID_LEN + 1	//offset beetwen two product_id
#define BASKET_ID_LEN		8		//len of basket_id
#define BASKET_ID_SIGN          '#'
typedef struct {
	char basket_id[BASKET_ID_LEN];
	uint8 len;
	uint8 offset;
	char prods_id[MAX_PRODS*(OFFSET)];
}Basket;
typedef struct {
	uint8 *id;
	uint8  num;
}Product;
/*********************************************************************
 * MODE
 */
#define CONFIG_MODE	1		//only use when user config from scanner
#define NORMAL_MODE	2		//normal operation

/*********************************************************************
 * CODE
 */
#define REQUEST_CODE			"**"	//request for basket_id
#define ADD_PRODS_CODE			"++"	//add product to basket
#define DEL_PRODS_CODE			"--"	//del product out of basket
#define CHANGE_PRODS_LABEL		"&&"	//change lable for data

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

/*********************************************************************
 * COMMON FUNCTIONS
 */
 extern char* int2char(int num);
 extern void printText(char* text1, char* text2);
 extern uint8 IsSameString(uint8 *a, uint8* b,const uint8 numBytsCompare);
 void CopyString(uint8 *d, uint8* s,const uint8 numByts);
#endif
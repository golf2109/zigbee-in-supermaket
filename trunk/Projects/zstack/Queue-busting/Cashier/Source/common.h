#ifndef COMMON_H
#define COMMON_H

#include <hal_types.h>

/*********************************************************************
 * DATA FOR BASKET
 */
#define MAX_PRODS		100		//max products in one basket
#define PRODS_ID_LEN		7		//len of product_id
#define OFFSET			PRODS_ID_LEN + 1	//offset beetwen two product_id
#define BASKET_ID_LEN		15		//len of basket_id
typedef struct {
	char basket_id[BASKET_ID_LEN];
	uint8 len;
	uint8 offset;
	char prods_id[MAX_PRODS*(OFFSET)];
}Basket;

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
#define ENDPOINT           10
#define PROFID             0x0F04
#define DEVICEID           0x0001
#define DEVICE_VERSION     0
#define FLAGS              0
#define MAX_CLUSTERS       1
#define CLUSTERID          1

#define	TIMEOUT			5000	//5 seconds

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
 extern void printText(char* text, uint8 line);
 extern uint8 strcmp(char* txt1, char* txt2);
 
#endif
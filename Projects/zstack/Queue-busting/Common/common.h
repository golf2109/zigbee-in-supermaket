#ifndef COMMON_H
#define COMMON_H

#include <hal_types.h>

/**
 * Defines
 */

#define MAX_PRODS		25		//max products in one basket
#define BASKET_ID_FORMAT        '#'
#define PRODS_NUM_SIZE          1

/*
 * Product Data structure
 */
#define PRODS_ID_LEN		13		//len of product_id
typedef struct Product{
  uint8 id[PRODS_ID_LEN];
  uint8  num;
}Product;
/*
 * Basket Data Structure
 */
#define BASKET_ID_LEN		8		//BASKET_ID_FORMAT +7
typedef struct {
	char id[BASKET_ID_LEN];
	uint8 len;
	Product prods[MAX_PRODS];
}Basket;
/**
* Define request/respone to/from Handheld
*/
#define REQUEST_BASKET			'%'	//request for basket_id
#define REPONSE_BASKET			'H'
#define DEL_BASKET			'^'	//del basket_id
#define STATUS_REQ                      'S'
#define STATUS_RESP_LEN                  11
/**
* Frame format for request/respone to/from Handheld:
*
* -Deleting req: '^'+num+{Basket IDs}; //num=0: delete all, other: the number of basket deleted
* -Status req: 'S'
* -Status resp: 'S'+[MAC addr:6bytes]+[Short Addr:2bytes]+[Parent Addr:2bytes]
* -Basket req: '%'+[Basket ID]
* -Basket resp: 'H'+[Short Addr]+[Basket ID len]+[Product ID len]+[Basket]
*/

/*********************************************************************
 * DATA FROM PC
 */
//[] [len] [cmd]
#define PC_CMD_P2P		'p'
#define PC_CMD_BRD		'b'

#define ADD_PRODS_CODE			'+'	//add product to basket
#define DEL_PRODS_CODE			'-'	//del product out of basket
#define CHANGE_PRODS_LABEL		'@'	//change lable for data
/*********************************************************************
 * MODE
 */
#define CONFIG_MODE	1		//only use when user config from scanner
#define NORMAL_MODE	2		//normal operation

/*********************************************************************
 * SECURITY
 */
#define KEY_SEC		0x123		//use for security

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
/*
 * Wireless-coms.c
 *
 * Created: 2023/04/06 21:15:29
 * Author : 219015150
 */ 
//#define F_CPU 8000000UL			// 8MHz clock speed

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "CU.h"


int main(void){
    /***************INITISIALISATIONS*************************/
    espStart(CWJAP, ssid, password);	// start  the esp8266-01 wifi module
    //wmuInit();
   // emuInit();
    //lcdClear();
    
    while (1){
       /*************UPDATE READINGS*************/
       _delay_ms(50);
       
       /***********************BUILD GET************************/
       buildGET();
       
       /**************SEND METER READINGS**************/
       sendData();
       
       /********************RECEIVE DATA********************/
       receiveData();
       
       /********************UPDATE STATUS******************/
       parseData();
       //electricity_status = checkStatus(elec_status);
       //water_status = checkStatus(wat_status);
       
       /********************RESET BUFFERS******************/
       resetBuffers();
	   
     }
     return 0;
}



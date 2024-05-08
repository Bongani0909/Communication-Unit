
#ifndef WCU_H_
#define WCU_H_
 
#define F_CPU 8000000L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "LCD_def.h"
//#include "WMU.h"


/****************************************
*				DEFINITIONS 			*
*****************************************/
#define BAUD 4800												// Baud rate
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)		// pre-scaler calculation macro
#define ESP8266_OK 1
#define ESP8266_READY 2
#define ESP8266_FAIL 3
#define ESP8266_NOCHANGE 4
#define ESP8266_LINKED 5
#define ESP8266_UNLINK 6
/****************************************
*				VARIABLES				*
*****************************************/
// Count Data
volatile int data_count = 0;
 // Data Receive buffer
 volatile char dataBuffer[200] = "0";
 
char data[50] = {0};
// keys to find data
char my_key[] = "y#";
char my_key_2[] = "0,";

// Length of data to be sent
    int dataLength;
    int responseLength;
    int userID = 1;
    int volume = 1;
	int energy = 2;  
	int balance = 4;
    
    /*******STRING DEFINITIONS********/
    char* domainName = "192.168.137.40";
    char* ssid = "bonganiN";
    char* password = "123456789";
    
    char elec_status[4] = {0};
    char wat_status[4] = {0};
    
    /*******ESP STRING BUFFERS*********/
    char request[150] = "GET \\data.php?userid=";
    char CIPSTART[150] = "\nAT+CIPSTART=0,\"TCP\",\"";
    char CWJAP[50] = "\nAT+CWJAP_DEF=\"";
    char CIPSEND[50] = "\nAT+CIPSEND=0,";
    
    
/****************************************
*				UART Functions			*
*****************************************/
void UART_init(unsigned long USART_BAUDRATE){
	UCSRB |= (1 << RXEN) | (1 << TXEN);				//Turn on transmission and reception & interrupt
	UCSRC |= (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1);			// Use 8-bit character sizes
	UBRRL = BAUD_PRESCALE;									// Load lower 8-bits of the baud rate value
	UBRRH = (BAUD_PRESCALE >> 8);							//load upper 8-bits
}
void UART_Set_Interrupt(){
   UCSRB |=(1 << RXCIE);
}

void UART_Clear_Interrupt(){
   UCSRB &= ~(1 << RXCIE);
}

unsigned char UART_RxChar(){
	while ((UCSRA & (1 << RXC)) == 0);	//wait till data is received
	return(UDR);						//return the byte
}

void UART_TxChar(char ch){
	while (! (UCSRA & (1<<UDRE)));	//wait for empty transmit buffer
	UDR = ch ;
}

void UART_SendString(char *str){
	unsigned char j=0;
	
	while (str[j]!=0)				//send string till null
	{
		UART_TxChar(str[j]);	
		j++;
	}
}

/*****************************************
*			STRING MANIPULATION		     *
******************************************/

// Function for cheching water satuses
int checkStatus(char* str){
	int i;
	char stsON[4] = "ON";
	
	for(i=0; i<2; i++){
		if(str[i]!=stsON[i]){
			return 0;
		}
	}
	return 1;
}

// function for concatenating strings
void concatenate(char * s1, char * s2){
    int length = 0;
    int j;

    //store length of s1
    while (s1[length] != '\0'){
        ++length;
    }

    //concatenate s2 to s1
    for(j = 0; s2[j] != '\0'; ++j, ++length){
        s1[length] = s2[j];
    }

    // terminate the string
    s1[length] = '\0';
}

// function that clears string content
void clearString(char* str){
    // get the size of the string
    int size = strlen(str)-1;
    int i;
    // clear all the characters of the string
    for(i = 0; i<size; i++){
    	str[i] = 0;
	}
}

// function to reset string to set string
void resetString(char* target, char* str){
    int strLength = strlen(str);
    int i;
    clearString(target);

    for(i=0; i<strLength; i++){
        target[i] = str[i];
    }
}

// find the start
int find_start(char *key, int key_size, int data_length, int start_loc){
    //decrement the size of array (used sizeof)
    key_size --;
    
    int i;
    int buff_size = 0;
    int loc = start_loc;

    while(buff_size<data_length){
        // go thouth te array searching for the string
        for(i=0; i<key_size; i++){
            loc++;
            if(dataBuffer[loc]!=key[i]){
                break;
            }
            // if it is found return the location
            if(i==key_size-1){
                return loc;
            }
        }
        buff_size++;

        // check if character is not found.
        if(buff_size == 94){
            return -1;
        }
    }
    return 0;
}

int find_data(char *key, char *key2, int key_size, int data_length){
    // get the starting point
    int starting = find_start(key, key_size, data_length, -1)+1;
    int ending = find_start(key2, key_size, data_length, starting)-(key_size);

    int count = 0;
    int i;
   
    for(i=starting; i<=ending; i++){
        data[count] = dataBuffer[i];
        count++;
    }
    return ending;

}
/***************************************
*      Wait for the received string    *
***************************************/
void UART_ReceiveString(char* str, uint8_t max_size)
{
	uint8_t count = 0;
	char c;
	
	do {
		c = UART_RxChar();
		if (c == '\r' || c == '\n') {
			break;
		}
		str[count] = c;
		count++;
	} while (count < max_size - 1);
	
	str[count] = '\0';
}

/*********************************************************
*          WAIT for the Response                        *
*********************************************************/
//int waitfor(char str[], uint8_t max_size) {
	//char receive_data[max_size];
	//clearString(receive_data);
//
	//// Wait for data with timeout
	//uint16_t timeout = 1000; // Timeout in milliseconds
	//uint16_t elapsed_time = 0;
	//while (elapsed_time < timeout) {
		//UART_ReceiveString(receive_data, max_size);
		//if (receive_data[0] != '\0') {
			//break; // Exit loop if data received
		//}
		//_delay_ms(10); // Short delay before checking again
		//elapsed_time += 10; // Increment elapsed time
	//}
//
	//if (strcmp(receive_data, str) == 0) {
		//return ESP8266_OK;
		//} else {
		//return ESP8266_FAIL;
	//}
//}

int waitfor(char str[], uint8_t max_size)
{
	char receive_data[max_size];
	clearString(receive_data);
	// Wait until some data is received
	while(receive_data[0] == '\0') {
		UART_ReceiveString(receive_data, max_size);
	}
	
	_delay_ms(1000);
	if (strcmp(receive_data, str) == 0) {
		return ESP8266_OK;
		
	}
	else {
		return ESP8266_FAIL;
	}
}


/*********************************************
*				ESP FUNCTIONS			     *
**********************************************/
// connect to access point
void connect(char* buffer1, char* ssid, char* password){
    // strings to concatenate
    char* string = "\",\"";
    char* escape = "\"\r\n";

    // concatenating the string
    concatenate(buffer1, ssid);
    concatenate(buffer1, string);
    concatenate(buffer1, password);
    concatenate(buffer1, escape);

    // send the CWJAP command
	UART_SendString("\n");
    UART_SendString(buffer1);
	_delay_ms(100);
	int P = waitfor("OK",100);
	if(P = ESP8266_OK){
		LCD_Clear();
		LCD_String(" Connected");
	}
	else{
		connect( buffer1, ssid, password);
	}
}

void espStart(char* buffer1, char* ssid, char* password) {
	UART_init(BAUD);
	lcd_init();
	sei();

	int retry_count = 0;
	int max_retries = 5; // Maximum number of retries

	while (retry_count < max_retries) {
		UART_SendString("AT\r\n");
		if (waitfor("OK", 1000) == ESP8266_OK) {
			_delay_ms(1000);
			UART_SendString("\nAT+RST\r\n");
			if (waitfor("OK", 1000) == ESP8266_OK) {
				LCD_Clear();
				LCD_String("Station mode");
				_delay_ms(1000);
				LCD_Clear();
				LCD_String("Activated");
				break; // Exit loop if reset successful
			}
		}
		retry_count++;
	}

	if (retry_count == max_retries) {
		LCD_Clear();
		LCD_String("Error: Failed to initialize ESP8266");
		return;
	}

	// Connect to access point
	connect(buffer1, ssid, password);
	_delay_ms(11000);

	retry_count = 0; // Reset retry count for the next phase

	while (retry_count < max_retries) {
		UART_SendString("\nAT+CWMODE=1\r\n");
		if (waitfor("OK", 1000) == ESP8266_OK) {
			LCD_Clear();
			LCD_String("Station mode");
			_delay_ms(1000);
			LCD_Clear();
			LCD_String("Activated");
			break; // Exit loop if set to station mode successful
		}
		retry_count++;
	}

	if (retry_count == max_retries) {
		LCD_Clear();
		LCD_String("Error: Failed to set station mode");
		return;
	}
}

//// start up the esp01 + connect to access point in station mode
//void espStart(char* buffer1, char* ssid, char* password){
   //// start UART
   //UART_init(BAUD);
   //lcd_init();
   //sei();
   //// send AT
   //UART_SendString("AT\r\n");
   //int r = waitfor("OK",100);
   //if(r == ESP8266_OK){
	  //_delay_ms(100);
	  //// Reset wifi module
//Rest_Mod:
	  //UART_SendString("\nAT+RST\r\n");
	  //_delay_ms(100);
	  //int R = waitfor("OK",100);
	  //if(R == ESP8266_OK){
		  //LCD_Clear();
		  //LCD_String("station mode");
		  //dis_cmd(0xC0);
		  //LCD_String("Activated");
	  //}
	  //else{
		  //goto Rest_Mod;
	  //}
	  //// Connect to access point
	  //connect(buffer1, ssid, password);
	  //_delay_ms(11000);
//set_as_station:
	  //// set to station mode
	  //UART_SendString("\nAT+CWMODE=1\r\n");
	  //int Z = waitfor("OK",100);
	  //_delay_ms(1500); 
	  //if(Z == ESP8266_OK){
		 //LCD_Clear();
		 //LCD_String("station mode");
		 //dis_cmd(0xC0);
		  //LCD_String("Activated");
	  //}
	  //else{
		  //goto set_as_station;
	  //}    
   //}
   //else{
	  //espStart(buffer1, ssid, password);
   //}
   ////_delay_ms(100);
   ////// Reset wifi module
   //////UART_SendString("AT+RST\r\n");
   ////_delay_ms(100);
   ////// Connect to access point
   ////connect(buffer1, ssid, password);
   ////_delay_ms(11000);
   //// set to station mode
   ////UART_SendString("AT+CWMODE=1\r\n");	
   ////_delay_ms(1500);
//}

// function to set connection to multiple connections
void setconnection(){
Mult_con:
   UART_SendString("\nAT+CIPMUX=1\r\n");
   int r = waitfor("OK",100);
   if(r == ESP8266_OK){
	   LCD_Clear();
	   LCD_String("Successfully");  
   }
   else{
	   goto Mult_con;
   }
   _delay_ms(800);
  
   UART_SendString("\nAT+CIPSERVER=1,80\r\n");
   int s = waitfor("OK",100);
   if(s == ESP8266_OK){
	   LCD_Clear();
	   LCD_String("Successfully");
   }
   else{
	   goto Mult_con;
   }
   _delay_ms(800);
}

// function for building the GET request
void buildRequest(char* buffer, char* host, int userID, int water, int electricity, int balance){
    // string buffers for integer values
    char user[5];
    char wat[5];
    char elect[5];
    char bal[5];

    // Strings to append
    char* string1 = "&water=";
    char* string2 = "&electricity=";
    char* string3 = "&balance=";
    char* string4 = "\r\nHost:";
    char* string5 = " HTTP\\1.1\r\n\r\n";

    // convert integers to strings
    itoa(userID, user, 10);
    itoa(water, wat, 10);
    itoa(electricity, elect, 10);
    itoa(balance, bal, 10);

    // concatenate the strings
    concatenate(buffer, user);
    concatenate(buffer, string1);
    concatenate(buffer, wat);
    concatenate(buffer, string2);
    concatenate(buffer, elect);
    concatenate(buffer, string3);
    concatenate(buffer, bal);
    concatenate(buffer, string4);
    concatenate(buffer, host);
    concatenate(buffer, string5);
}


//function to build CIPSTART string
void startTCP(char* buff, char* domain){

    // String to append
    char* str = "\",80,0\r\n";

    // concatenate strings
    concatenate(buff, domain);
    concatenate(buff, str);

    // send data
    UART_SendString(buff);
	int s = waitfor("OK",100);
	if(s == ESP8266_OK){
		LCD_Clear();
		LCD_String("Successfully");
	}
	else{
		startTCP(buff,domain);
	}
}

// send command CIPSEND command
void sendCommand(char* buffer, int dataLength){
    char len[3];
    char* crnl = "\r\n";
    //convert the integer datalenght to a string
    itoa(dataLength, len, 10);

    // build the command
    concatenate(buffer, len);
    concatenate(buffer, crnl);

    // send the CIPSEND command
    UART_SendString(buffer);
	int s = waitfor("OK",100);
	if(s == ESP8266_OK){
		LCD_Clear();
		LCD_String("Successfully");
	}
	else{
		sendCommand(buffer,dataLength);
	}
}

/*******************************************
*				LOGIC CONTROL		       *
********************************************/

/**************build GET request string***************/
void buildGET(){
    buildRequest(request, domainName, userID, volume, energy, balance);
    // get lengh of request to be sent
    dataLength = strlen(request);
}
    
/********************SEND DATA************************/
void sendData(){
    setconnection();
    startTCP(CIPSTART, domainName);
    _delay_ms(5000);
    sendCommand(CIPSEND, dataLength);
    _delay_ms(800);
    UART_SendString(request);
}

/*******************RECEIVE DATA********************/
void receiveData(){
    UART_Set_Interrupt();		// Start UART receive interrupt
    _delay_ms(3000);
    UART_Clear_Interrupt();		// Stop UART receive interrupt
} 
   
/*******************PARSE DATA***********************/
void parseData(){
    responseLength = strlen(dataBuffer);
    find_data(my_key, my_key_2, 3, responseLength);
    sscanf(data, "%[ONF]%*[^OFN]%[ONF]", elec_status, wat_status);
}
    
/************* RESET ALL STRING BUFFERS*******/
void resetBuffers(){
    data_count=0;
    clearString(dataBuffer);
    clearString(data);
    clearString(wat_status);
    clearString(elec_status);
    resetString(request, "GET \\data.php?userid=");
    resetString(CIPSTART, "AT+CIPSTART=0,\"TCP\",\"");
    resetString(CIPSEND, "AT+CIPSEND=0,");
    resetString(CWJAP, "AT+CWJAP_DEF=\"");
}

/**************************************
*				USART ISR			  *
***************************************/
 // USART interrupt service routine, saves received data to buffer
 ISR(USART_RXC_vect){
    dataBuffer[data_count] = UDR;
    data_count+=1;
}


#endif /* WCU_H_ */
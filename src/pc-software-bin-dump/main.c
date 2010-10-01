/*
   ####################################################################################
   #                                                                                  #
   #                              DCF39 Parser V1.0.0                                 #
   #                                  Main-Program                                    #
   #                                                                                  #
   #    Copyright (C) 2010 Philipp Fabian Benedikt Maier (aka. Dexter)                #
   #                                                                                  #
   #    This program is free software; you can redistribute it and/or modify          #
   #    it under the terms of the GNU General Public License as published by          #
   #    the Free Software Foundation; either version 2 of the License, or             #
   #    (at your option) any later version.                                           #
   #                                                                                  #
   #    This program is distributed in the hope that it will be useful,               #
   #    but WITHOUT ANY WARRANTY; without even the implied warranty of                #
   #    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                 #
   #    GNU General Public License for more details.                                  #
   #                                                                                  #
   #    You should have received a copy of the GNU General Public License             #
   #    along with this program; if not, write to the Free Software                   #
   #    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA    #
   #                                                                                  #
   #################################################################################### */


/* ## HEADER ########################################################################## */

/* Header */
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <ttybanana.h> 
#include <toolbanana.h>
#include <string.h>

/* Transmission parameters */
#define DCF39_BAUDRATE 115200			/* Baudrate of the dcf39 receiver */
#define DCF39_PORT "/dev/ttyUSB0"			/* Serial port - change if needed */

#define DCF39_START 0x68				/* Start token used by DCF39 */
#define DCF39_END 0x16					/* End token used by DCF39 */
#define DCF39_TIMEOUT 30000 /* us */			/* Burst timeout */

/* DCF39 frame as described in http://www.qru.de/dcf39-beacon.html */
typedef struct
{
	char lField;
	char cField;
	char aField;
	char ciField;
	char userData[16];
	int userDataLen;
	char checkSum;
} dcf39Frame;


/* #################################################################################### */

/* ## RECEIVE A FRAME WITH DCF39 DATA ############################################## */
int dcf39Receive(dcf39Frame *data)
{
	char dataByte = 0;
	char dataBurst[10000];
	int i;

	/* Detect beginning of a frame (0x68) */
	do
	{
		dataByte = ttyGetchar(DCF39_PORT);
	} while(dataByte != DCF39_START);

	printf("   Received Startmark: %02hx\r\n",DCF39_START);

	/* Read data burst */
	ttyReadDynamic (DCF39_PORT, dataBurst, DCF39_TIMEOUT);

	/* Verify L Field */
/*	data->lField = dataBurst[0];
	if(dataBurst[1] != data->lField)
	{
		printf(" * Error: L-Field mismatch! %02hhx is not equal to %02hhx\r\n", data->lField, dataBurst[1]);
		printf(" * Buffer content (first 50 bytes) is:\r\n");
		hexBinAsciiDump(dataBurst,50);
		return -1;

	}
	else 
		printf("   Received L-Field: %02hhx\r\n",data->lField);
	
	/* Check start mark */
/*	if(dataBurst[2] != DCF39_START)
	{
		printf(" * Error: DCF39 Startmark %02hhx expected, received %02hhx\r\n",DCF39_END, dataBurst[2]);
		printf(" * Buffer content (first 50 bytes) is:\r\n");
		hexBinAsciiDump(dataBurst,50);
		return -1;
	}
	else
		printf("   Received Startmark: %02hhx\r\n",DCF39_START);

	/* C,A and CI Field */
/*	data->cField = dataBurst[3];
	printf("   Received C-Field: %02hhx\r\n",data->cField);

	data->aField = dataBurst[4];
	printf("   Received A-Field: %02hhx\r\n",data->aField);

	data->ciField = dataBurst[5];
	printf("   Received CI-Field: %02hhx\r\n",data->ciField);


	/* User data */
/*	for(i=0;i<(data->lField-3);i++)
	{
		data->userData[i] = dataBurst[i+6];
	}

	data->userDataLen = data->lField-3;

	printf("   Received User Data body:\r\n");
	hexBinAsciiDump(data->userData, data->lField-3);
	printf("   User data length is %i byte\r\n", data->userDataLen);

	/* Checksum */
/*	data->checkSum = dataBurst[i];

	printf("   Received Checksum: %02hhx\r\n",data->checkSum);

	/* Check end token */
//	if(dataBurst[i+6+1] != DCF39_END)
//	{
//		printf(" * Error: DCF39 Endmark %02hhx expected, received %02hhx\r\n",DCF39_END, dataBurst[i+6+1]);
//		printf(" * Buffer content (first 50 bytes) is:\r\n");
		hexBinAsciiDump(dataBurst,50);
//		return -1;
//	}
//	else
//		printf("   Received Endmark: %02hx\r\n",DCF39_END);

	return 0;
}
/* #################################################################################### */


/* Main program: Perform some testpages */
int main(void)
{

	dcf39Frame dcf39FrameData;		/* Data of the latest transmission */

	printf("_____________________________________________________________________________________________\n\r");
	printf("DCF39 Receiver Copyright(c) Philipp Fabian Benedikt Maier\n\r");
	printf("\n\r");
	printf("Note: This is an application that can decode dcf39 transmissions.\n\r");
	printf("\n\r");

	printf("Initalizing...\n\r");

	printf("* Opening serial port...\n\r");
	ttyInitProf(DCF39_PORT,DCF39_BAUDRATE,8,NONE);		/* Initalize tty */

	ttyClearBuffer(DCF39_PORT);					/* Clear buffer */

	printf("* Serial port is set to %s at %i Baud - edit the sourcecode to alter this setting.\n\r",DCF39_PORT,DCF39_BAUDRATE);
	printf("\n\r");




	while(1)
	{
		printf(" * Waiting for frame...\n\r");

		if(dcf39Receive(&dcf39FrameData) == 0)
		{
			printf(" * Frame ok!\r\n");



			if(dcf39FrameData.userData[0] == 0)
			{
				printf(" * Time: %hhu:%hhu ",dcf39FrameData.userData[3],dcf39FrameData.userData[2]);
				printf("  Date: %hhu.%hhu.%hhu \r\n",dcf39FrameData.userData[4]&0xF,dcf39FrameData.userData[5],dcf39FrameData.userData[6]);
			}
			
			






		}
		else
			printf(" * Frame error!\r\n");

		printf("\r\n");

		fflush(stdout);
	}

	return 0;
}

/* #################################################################################### */

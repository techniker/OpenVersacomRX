/*
   ####################################################################################
   #                                                                                  #
   #                                  DCF39 V1.0.0                                    #
   #                                    Receiver                                      #
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
#include <stdio.h>
#include <string.h>
#include <ttybanana.h> 
#include <toolbanana.h> 
#include <time.h>
#include "receive.h"

#define DCF39_BAUDRATE 115200			/* Baudrate of the dcf39 receiver */
#define DCF39_TIMEOUT 30000 /* us */		/* Burst timeout */

static int optionDatalog = 0;
static int optionRs232 = 0;
static int optionReplay = 0;

static char receiverRS232Port[100];
static FILE *datalogFile;
/* #################################################################################### */


/* #################################################################################### */

/* Initalize DCF39 receiver */
int dcf39ReceiverInit(char *port, char *datalog)
{
	optionDatalog = 0;
	optionRs232 = 0;
	optionReplay = 0;

	if(port)
	{
		printf(" * Serial port is set to %s at %i Baud \n\r",port,DCF39_BAUDRATE);
		ttyInitProf(port,DCF39_BAUDRATE,8,NONE);	/* Initalize tty */
		ttyClearBuffer(port);				/* Clear buffer */
		strcpy(receiverRS232Port, port);
		optionRs232 = 1;
	}
	else
	{
		printf(" * Will replay from datalog file...\r\n");
		optionReplay = 1;
	}

	/* Open datalog file for writing */
	if((datalog)&&(port))
	{
		printf(" * Datalog file: %s\r\n", datalog);
		datalogFile = fopen(datalog,"w");
		if(datalogFile < 0)
		{
			printf(" * Error: Could not open datalog file!\r\n");
			optionDatalog = 0;
			optionRs232 = 0;
			optionReplay = 0;
			return -1;
		}

		optionDatalog = 1;
	}
	/* Open datalog file for reading */
	else if(datalog)
	{
		if(testFile(datalog) != 0)
		{
			printf(" * Error: Could not open datalog file!\r\n");
			optionDatalog = 0;
			optionRs232 = 0;
			optionReplay = 0;
			return -1;
		}

		printf(" * Datalog file: %s\r\n", datalog);
		datalogFile = fopen(datalog,"r");
		optionDatalog = 1;
	}
	else
	{
		printf(" * Info: No datalog file will be created!\r\n");
		optionDatalog = 0;
	}

	/* Check for invalid mode configuration */
	if((optionRs232 == 0)&&(optionDatalog == 0)&&(optionReplay == 0))
		return -1;
	else if((optionRs232 == 1)&&(optionDatalog == 1)&&(optionReplay == 1))
		return -1;
	else if((optionRs232 == 1)&&(optionDatalog == 0)&&(optionReplay == 1))
		return -1;

	return 0;
}

/* Receive DCF39 databurst */
int dcf39ReceiveBurst(char *data, time_t *timestamp)
{
	int burstLen = 0;
	int datalogLen;

	time_t timeInSeconds;

	memset(data,0,DCF39_MAXIMUM_BURSTLEN);


	if(optionRs232)
	{
		time(&timeInSeconds);
		burstLen = ttyReadDynamic (receiverRS232Port, data, DCF39_TIMEOUT);
	}
	else if(optionReplay)
	{
		fread(&timeInSeconds, sizeof(time_t), 1, datalogFile);
		burstLen = fgetc(datalogFile);
		datalogLen = fread(data, sizeof(char), DCF39_MAXIMUM_BURSTLEN, datalogFile);
		if(datalogLen == 0)
			return -1;
	}
	
	if((optionDatalog)&&(optionReplay == 0))
	{
		time(&timeInSeconds);
		fwrite(&timeInSeconds, sizeof(time_t), 1, datalogFile);
		fputc(burstLen,datalogFile);
		fwrite(data, sizeof(char), DCF39_MAXIMUM_BURSTLEN, datalogFile);
		fflush(datalogFile);
	}	

	*timestamp = timeInSeconds;

	return burstLen;
}
/* #################################################################################### */

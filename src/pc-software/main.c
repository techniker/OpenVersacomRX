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
#include <getopt.h>
#include <ctype.h>
#include <unistd.h>
#include <libgen.h>
#include <time.h>
#include <libgen.h>

#include "receive.h"
#include "dcf39.h"

char programName[255];
/* #################################################################################### */

/* #################################################################################### */
void showUsage(void)
{
	printf(" * Usage:\r\n");
	printf("    %s -ris [ -p PORT] [ -l DATALOG] [ -e EVU-Address ]\r\n",programName);
	printf("            -r) Dump raw data in the form |Time|Length|Data(hex)|\r\n");
	printf("            -i) Ignore broadcast frames\r\n");
	printf("            -s) Strict - Ignore all frames that are not DIN 19244 conform\r\n");
	printf("            -p) Receive data from serial port (115200 Baud)\r\n");
	printf("            -l) Receive/Write data from/to logfile.\r\n");
	printf("            -e) Filter data by EVU-Address (e.g. -e FFFF)\r\n");
	printf("\r\n");
}

/* Main program: Perform some testpages */
int main(int argc, char *argv[])
{
	char dcf39DataBurst[DCF39_MAXIMUM_BURSTLEN];
	int dcf39DataBurstLen;
	time_t frameTimeTimestamp;
	struct tm *frameTime;
	char timeAsString[100];
	
	int getoptOption;
	char *getoptPort = 0;			/* serial port */
	char *getoptDatalog = 0;		/* binary datalog */
	int getoptRawdump = 0;
	int getoptIgnoreBroadcast = 0;
	int getoptIgnoreInvalid = 0;
	char *getoptEVUAddress = 0;

	unsigned int EVUAddressFilter = 0;
	int ignoreData = 0;
	dcf39Frame dcf39FrameData;

	strcpy(programName,basename(argv[0]));

	printf("_____________________________________________________________________________________________\n\r");
	printf("DCF39 Receiver Copyright(c) Philipp Fabian Benedikt Maier\n\r");
	printf("\n\r");
	printf("Note: This is an application that can decode dcf39 transmissions.\n\r");
	printf("\n\r");

  	/* Parse Options */
	while ((getoptOption = getopt (argc, argv, ":p:l:?hirse:")) != -1)
		switch (getoptOption)
		{
			case 'r':
				getoptRawdump = 1;
			break;
			case 'i':
				getoptIgnoreBroadcast = 1;
			break;
			case 's':
				getoptIgnoreInvalid = 1;
			break;
			case 'e':
				getoptEVUAddress = optarg;
			break;
			case 'h':
				showUsage();
				return 0;
			break;
			case '?':
				showUsage();
				return 0;
			break;
			case 'p':
				getoptPort = optarg;
			break;
			case 'l':
				getoptDatalog = optarg;
			break;
		}


	if(dcf39ReceiverInit(getoptPort,getoptDatalog) != 0)
	{
		printf(" * Error: Could not initalize receiver!\r\n");
		return -1;
	}

	if(getoptEVUAddress)
	{
		sscanf(getoptEVUAddress, "%x", &EVUAddressFilter);
		EVUAddressFilter &= 0xFFFF;
		printf(" * Info: Will only display Frames that have EVU-Address %02hhx %02hhx.\r\n",EVUAddressFilter >> 8,EVUAddressFilter);
	}

	while(1)
	{
		/* Receive frame */
		dcf39DataBurstLen = dcf39ReceiveBurst(dcf39DataBurst,&frameTimeTimestamp);
		if(dcf39DataBurstLen == -1)
			return 0;

		frameTime = localtime(&frameTimeTimestamp);
		sprintf(timeAsString, "%s", asctime(frameTime));
		replaceChar(timeAsString, '\r' , 0, sizeof(timeAsString));
		replaceChar(timeAsString, '\n' , 0, sizeof(timeAsString));

		/* Ignore frames that do not begin with 0x86 */
		if(dcf39Check(dcf39DataBurst) != 0)
		{
			if((dcf39DataBurst[0]&0xFF) != DCF39_START)
				ignoreData = 1;
		}

		/* Ignore broadcast frames */
		if(getoptIgnoreBroadcast == 1)
		{
			if(((dcf39DataBurst[5]&0xFF) == 0x00)&&((dcf39DataBurst[6]&0xFF) == 0x00))
				ignoreData = 1;
			if(((dcf39DataBurst[5]&0xFF) == 0xFF)&&((dcf39DataBurst[6]&0xFF) == 0xFF))
				ignoreData = 1;
		}

		/* Apply EVU-Address filter */
		if(getoptEVUAddress)
		{
			if((dcf39DataBurst[5]&0xFF) != (EVUAddressFilter>>8)) 
				ignoreData = 1;

			if((dcf39DataBurst[6]&0xFF) != (EVUAddressFilter&0xFF)) 
				ignoreData = 1;
		}


		/* Start data analysis */
		if(ignoreData == 0)
		{
			if(getoptRawdump == 1)
			{
				printf("|%s|", timeAsString);
				printf("%i|", dcf39DataBurstLen);

				printfHex(dcf39DataBurst, dcf39DataBurstLen, ' ');

				printf("|\r\n");
			}
			else
			{
				printf( " * Frame was received on: %s\r\n", timeAsString);
			
				if(dcf39Analyse(dcf39DataBurst, dcf39DataBurstLen, &dcf39FrameData) == 0)
				{
					printf(" * DCF39-Frame ok!\r\n");
					if(dcf39FrameData.userData[0] == 0)
					{
						printf(" * Time: %hhu:%hhu ",dcf39FrameData.userData[3]&0x1F,dcf39FrameData.userData[2]);
						printf("  Date: %hhu.%hhu.%hhu \r\n",dcf39FrameData.userData[4]&0x1F,dcf39FrameData.userData[5],dcf39FrameData.userData[6]);
					}
				}
				else
					printf(" * Unknown frame type?\r\n");

				printf("\r\n");
			}
		}

		ignoreData = 0;
		fflush(stdout);
	}

	return 0;
}

/* #################################################################################### */

//# Copyright (c) 2010, Bjoern Heller <tec@hellercom.de>. All rights reserved
//# This code is licensed under GNU/ GPL

/* ## HEADER ########################################################################## */
#include <stdio.h>
#include <string.h>
#include <ttybanana.h> 
#include <toolbanana.h> 
#include "dcf39.h"
/* #################################################################################### */

/* Analyse and Parse DCF39 packet */
int dcf39Analyse(char *dataBurst, int length, dcf39Frame *data)
{
	int i,k;
	int checksum = 0;

	memset(data,0,sizeof(data));

	printf(" * Parsing DCF39 Frame...\r\n");

	if(dataBurst[0] != DCF39_START)
	{
		printf(" * Error: Startmark (%02hhx) is not equal to %02hhx - aborting!", dataBurst[0], DCF39_START);
		printf(" * Buffer content is:\r\n");
		hexBinAsciiDump(dataBurst,length);
		return -1;
	}
	else
		printf("   Startmark: %02hhx\r\n",DCF39_START);

	/* Verify L Field */
	data->lField = dataBurst[1];
	if(dataBurst[2] != data->lField)
	{
		printf(" * Error: L-Field mismatch! %02hhx is not equal to %02hhx\r\n", data->lField, dataBurst[2]);
		printf(" * Buffer content is:\r\n");
		hexBinAsciiDump(dataBurst,length);
		return -1;

	}
	else 
		printf("   L-Field: %02hhx\r\n",data->lField);
	
	/* Check start mark */
	if(dataBurst[3] != DCF39_START)
	{
		printf(" * Error: DCF39 Startmark %02hhx expected, received %02hhx\r\n",DCF39_END, dataBurst[3]);
		printf(" * Buffer content is:\r\n");
		hexBinAsciiDump(dataBurst,length);
		return -1;
	}
	else
		printf("   Startmark: %02hhx\r\n",DCF39_START);

	/* C,A and CI Field */
	data->cField = dataBurst[4];
	printf("   C-Field: %02hhx  ==> Telegramm-number: %i\r\n",data->cField, (data->cField >> 4) & 0xF);

	data->aField = dataBurst[5];
	printf("   EVU-Address 1  (A-Field): %02hhx\r\n",data->aField);

	data->ciField = dataBurst[6];
	printf("   EVU-Address 2 (CI-Field): %02hhx\r\n",data->ciField);


	/* User data */
	for(i=0;i<(data->lField-3);i++)
	{
		data->userData[i] = dataBurst[i+7];
	}

	data->userDataLen = data->lField-3;

	printf("   User Data body:\r\n");
	hexBinAsciiDump(data->userData, data->lField-3);
	printf("   User data length is %i byte\r\n", data->userDataLen);

	/* Verify checksum */
	checksum += data->cField;
	checksum += data->aField;
	checksum += data->ciField;
	for(k=0;k<data->userDataLen;k++)
		checksum += data->userData[k];
	checksum &= 0xFF;
	data->checkSum = dataBurst[i+7];

	if((char)checksum == (char)data->checkSum)
		printf("   Checksum ok: %02hhx\r\n",data->checkSum);
	else
	{
		printf(" * Error: Invalid checksum: %02hhx, should be: %02hhx\r\n",data->checkSum, checksum);
		printf(" * Buffer content is:\r\n");
		hexBinAsciiDump(dataBurst,length);
		return -1;
	}

	/* Check end token */
	if(dataBurst[i+7+1] != DCF39_END)
	{
		printf(" * Error: DCF39 Endmark %02hhx expected, received %02hhx\r\n",DCF39_END, dataBurst[i+6+1]);
		printf(" * Buffer content is:\r\n");
		hexBinAsciiDump(dataBurst,length);
		return -1;
	}
	else
		printf("   Endmark: %02hx\r\n",DCF39_END);

	return 0;
}

/* Check DCF39 frame integrity */
int dcf39Check(char *dataBurst)
{
	int checksum = 0;
	int k;

	/* Startmark correct ? */
	if(dataBurst[0] != DCF39_START)
		return -1;

	/* Length correct ? */
	if(dataBurst[1] != dataBurst[2])
		return -1;

	/* Startmark 2 correct */
	if(dataBurst[3] != DCF39_START)
		return -1;

	/* Verify checksum */
	checksum += dataBurst[4];
	checksum += dataBurst[5];
	checksum += dataBurst[6];
	for(k=7;k<dataBurst[2]+4;k++)
		checksum += dataBurst[k];
	checksum &= 0xFF;

	if(!((char)checksum == (char)dataBurst[k]))
		return -1;

	return 0;
}


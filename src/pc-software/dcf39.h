//# Copyright (c) 2010, Bjoern Heller <tec@hellercom.de>. All rights reserved
//# This code is licensed under GNU/ GPL

/* ## HEADER ########################################################################## */
#ifndef DCF39_H
#define DCF39_H

#define DCF39_START 0x68				/* Start token used by DCF39 */
#define DCF39_END 0x16					/* End token used by DCF39 */

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

/* Analyse and Parse DCF39 packet */
int dcf39Analyse(char *dataBurst, int length, dcf39Frame *data);	

/* Check DCF39 frame integrity */
int dcf39Check(char *dataBurst);

#endif /*DCF39_H*/
/* #################################################################################### */






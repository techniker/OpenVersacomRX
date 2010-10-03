/*
   ####################################################################################
   #                                                                                  #
   #                                  DCF39 V1.0.0                                    #
   #                               DCF39 packet parser                                #
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






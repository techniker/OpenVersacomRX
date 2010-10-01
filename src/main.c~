/*
   ####################################################################################
   #                                                                                  #
   #                          DCF39 Reciver Firmware V1.0.0                           #
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
#include <avr/io.h>					/* Include I/O definitions */
#include <stdint.h>					/* Include Integer types */
#include <stdio.h>					/* Include standard I/O */
#include <avr/pgmspace.h>				/* Include pgmspace definition */

#include "uart.h"					/* Include serial port control */
#include "dcf39.h"					/* Include dcf39 receiver */
/* #################################################################################### */

/* ## MAIN ############################################################################ */
int main(void)
{
	/* 34 = 57,6k (ISDN/MODEM Speed) */
	/* 68 = 28.8 */
	/* 16 = 115.2 */

	systemUartInit(16);				/* Initalize UART */

	printf("\r\n\n\n\n\n\n\n\n\n\n\n");
	printf("Rundsteuerrung Receiver (c)2010 Detxer\r\n");
	printf("Data format is binary stream, please use external software to decode.\r\n");

	while(1)
	{
		systemUartTransmit(systemDcf39Receive());
	}

	return 0;
}
/* #################################################################################### */

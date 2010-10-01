/*
   ####################################################################################
   #                                                                                  #
   #                          DCF39 Reciver Firmware V1.0.0                           #
   #                                Software interface                                #
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
#include "delay.h"					/* Include Delay control mechanisms */
#include "dcf39.h"					/* Include Own header file */
/* #################################################################################### */


/* ## SOFTWARE UART RECEIVER ########################################################## */
/* Receive byte from dcf30 */
uint8_t systemDcf39Receive(void)
{
	uint8_t i;
	uint8_t result = 0;

		while(((PIND >> PD2) & 1) == 0);		/* Wait for incoming start bit */
		systemDelay100us(RX_BAUDRATE);
	
		systemDelay100us(RX_BAUDRATE / 2);
	

		result |= (((~PIND >> PD2) & 1) << 0);
		systemDelay100us(RX_BAUDRATE);

		result |= (((~PIND >> PD2) & 1) << 1);
		systemDelay100us(RX_BAUDRATE);

		result |= (((~PIND >> PD2) & 1) << 2);
		systemDelay100us(RX_BAUDRATE);

		result |= (((~PIND >> PD2) & 1) << 3);
		systemDelay100us(RX_BAUDRATE);
	
		result |= (((~PIND >> PD2) & 1) << 4);
		systemDelay100us(RX_BAUDRATE);

		result |= (((~PIND >> PD2) & 1) << 5);
		systemDelay100us(RX_BAUDRATE);

		result |= (((~PIND >> PD2) & 1) << 6);
		systemDelay100us(RX_BAUDRATE);

		result |= (((~PIND >> PD2) & 1) << 7);
		systemDelay100us(RX_BAUDRATE);

		/* Override parity bit */
		systemDelay100us(RX_BAUDRATE);

	return result;
}
/* #################################################################################### */




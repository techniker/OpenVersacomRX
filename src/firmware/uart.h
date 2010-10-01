/*
   ####################################################################################
   #                                                                                  #
   #                          DCF39 Reciver Firmware V1.0.0                           #
   #                                   Uart Handler                                   #
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
#ifndef uart_h
#define uart_h

void systemUartInit(uint16_t UartBaudrate);				/* Initalize UART */
void systemUartTransmit(uint8_t byte2transmit);
uint8_t systemUartReceive(void);
uint8_t systemUartPutchar(char c, FILE *stream);			/* Putchar, used by printf ect... */

#endif 
/* #################################################################################### */


//# Copyright (c) 2010, Bjoern Heller <tec@hellercom.de>. All rights reserved
//# This code is licensed under GNU/ GPL


/* ## HEADER ########################################################################## */
#ifndef RECEIVE_H
#define RECEIVE_H

#define DCF39_MAXIMUM_BURSTLEN 100

int dcf39ReceiverInit(char *port, char *datalog);	/* Initalize DCF39 receiver */
int dcf39ReceiveBurst(char *data, time_t *timestamp);	/* Receive DCF39 databurst */


#endif /*RECEIVE_H*/
/* #################################################################################### */




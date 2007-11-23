/*--------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Ledru Sylvestre */
/*--------------------------------------------------------------------------*/ 
#ifndef __INFFIC_H__
#define __INFFIC_H__
/*--------------------------------------------------------------------------*/ 
#include "machine.h"
/*--------------------------------------------------------------------------*/ 
/** 
* definitions for get_sci_data_strings
* MANUAL_ID returns "$MANCHAPTERS"
* STARTUP_ID returns "exec('SCI/etc/scilab.start',-1);"
* DEMOS_ID returns "scilab_demos();"
* SAVE_ID returns "home/scilab.save"
* QUIT_ID returns "exec('SCI/etc/scilab.quit',-1);quit;"
*/
#define MANUAL_ID 0 
#define STARTUP_ID 1
#define DEMOS_ID 2
#define SAVE_ID 3
#define QUIT_ID 4

void C2F(inffic)(integer *iopt, char *name, integer *nc);

void C2F(infficl)(integer *iopt, integer *nc);

char *get_sci_data_strings(int n);

#endif /*__INFFIC_H__*/
/*--------------------------------------------------------------------------*/ 

/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Ledru Sylvestre */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __INFFIC_H__
#define __INFFIC_H__

#include "machine.h"

void C2F(inffic)(integer *iopt, char *name, integer *nc);

void C2F(infficl)(integer *iopt, integer *nc);

char *get_sci_data_strings(int n);

#endif /*__INFFIC_H__*/

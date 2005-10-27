/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __FROMC__
#define __FROMC__

#include <stdio.h>
#include <string.h>
#include "../machine.h"
#include "../stack-c.h"
#include "../version.h"

int C2F(intfromc) _PARAMS((char *fname));
void SetFromCToON(void);
int IsFromC(void);

#endif /*FROMC*/
/*-----------------------------------------------------------------------------------*/ 

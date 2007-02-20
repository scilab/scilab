/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __GW_INTEGER__
#define __GW_INTEGER__

#ifdef _MSC_VER
  #include <windows.h>
  #include <stdio.h>
#endif

#include <string.h>

#include "machine.h"
#include "stack-c.h"
#include "sciprint.h"

int C2F(gw_integer)(void);

typedef int (*Integer_Interf) __PARAMS((char *fname,unsigned long fname_len));

typedef struct table_struct 
{
  Integer_Interf f;    /** function **/
  char *name;      /** its name **/
} IntegerTable;


int C2F(sciint32) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sciint16) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sciint8) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sciinttype) _PARAMS((char *fname,unsigned long fname_len));
int C2F(scidouble) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sciuint32) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sciuint16) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sciuint8) _PARAMS((char *fname,unsigned long fname_len));
int C2F(scimgeti) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sciimput) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sciiconvert) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sciiabs) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sciidiag) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sciitriu) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sciitril) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sciisum) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sciicumsum) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sciimax) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sciimin) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sciimaxi) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sciimini) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sciip) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sciiconvert) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sciimatrix) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sciiprod) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sciicumprod) _PARAMS((char *fname,unsigned long fname_len));

#endif /*  __GW_INTEGER__ */
/*-----------------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __GW_STATISTICS__
#define __GW_STATISTICS__

#ifdef _MSC_VER
  #include <windows.h>
  #include <stdio.h>
#endif

#include <string.h>

#include "machine.h"
#include "stack-c.h"
#include "sciprint.h"

int C2F(gw_statistics)(void);

typedef int (*Statistics_Interf) __PARAMS((char *fname,unsigned long l));

typedef struct table_struct 
{
  Statistics_Interf f;    /** function **/
  char *name;      /** its name **/
} StatisticsTable;

int cdfbetI(char* fname,unsigned long l);
int cdfbinI(char* fname,unsigned long l);
int cdfchiI(char* fname,unsigned long l);
int cdfchnI(char* fname,unsigned long l);
int cdffI(char* fname,unsigned long l);
int cdffncI(char* fname,unsigned long l);
int cdfgamI(char* fname,unsigned long l);
int cdfnbnI(char* fname,unsigned long l);
int cdfnorI(char* fname,unsigned long l);
int cdfpoiI(char* fname,unsigned long l);
int cdftI(char* fname,unsigned long l);

#endif /*  __GW_STATISTICS__ */
/*-----------------------------------------------------------------------------------*/


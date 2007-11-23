/*--------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#ifndef __GW_STATISTICS__
#define __GW_STATISTICS__
/*--------------------------------------------------------------------------*/ 
#include "machine.h"
/*--------------------------------------------------------------------------*/ 
int C2F(gw_statistics)(void);
/*--------------------------------------------------------------------------*/ 
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
/*--------------------------------------------------------------------------*/ 
#endif /*  __GW_STATISTICS__ */
/*--------------------------------------------------------------------------*/


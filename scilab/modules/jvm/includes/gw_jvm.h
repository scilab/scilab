/*--------------------------------------------------------------------------*/
/**
* @author Allan CORNET INRIA 2007
*/
/*--------------------------------------------------------------------------*/
#ifndef __GW_JVM_H__
#define __GW_JVM_H__
/*--------------------------------------------------------------------------*/
#include "machine.h"
/*--------------------------------------------------------------------------*/
int C2F(gw_jvm)(void);
/*--------------------------------------------------------------------------*/
/* Declaration of all the profile function declared and */
/* used in sci_gateway */
int C2F(sci_with_embedded_jre) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_system_setproperty) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_system_getproperty) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_javaclasspath) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_javalibrarypath) _PARAMS((char *fname,unsigned long fname_len));
/*--------------------------------------------------------------------------*/
#endif /*  __GW_JVM_H__ */



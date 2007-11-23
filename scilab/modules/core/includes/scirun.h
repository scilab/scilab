/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Sylvestre LEDRU */
/*--------------------------------------------------------------------------*/
#ifndef __SCIRUN_H__
#define __SCIRUN_H__ 

#include "machine.h"

/**
 * Run scilab parser with initial instruction 
 * @param startupCode : character string which contains the initial instruction 
 * @param startupCode_len : length of startupCode 
 */
int C2F(scirun)(char *startupCode, long int startupCode_len);

#endif /* __SCIRUN_H__ */

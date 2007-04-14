/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Sylvestre LEDRU */
/*-----------------------------------------------------------------------------------*/
#ifndef __SCIRUN_H__
#define __SCIRUN_H__ 

#include "machine.h"
/**
 * Run scilab parser with initial instruction 
 * @param bu1 : character string which contains the initial instruction 
 * @param bu1_len : length of bu1 
 */
int C2F(scirun)(char *bu1, long int bu1_len);
#endif /* __SCIRUN_H__ */
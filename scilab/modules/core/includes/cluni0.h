/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Sylvestre LEDRU */
/*-----------------------------------------------------------------------------------*/
#ifndef __CLUNI0_H__
#define __CLUNI0_H__

#include "machine.h"
/**
 * expand in_name to produce out_name 
 *
 * @param in_name the input string that we want to expand
 * @param out_name the output string
 * @param out_n number of character of out_name
 * @param lin length of in_name
 * @param lout length of out_name
 * @return 0
 */
int C2F(cluni0)(char *in_name, char *out_name, int *out_n, long int lin, long int lout);

#endif /* __CLUNI0_H__ */

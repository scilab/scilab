/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Sylvestre LEDRU */
/*-----------------------------------------------------------------------------------*/
#include "machine.h"
#include "sciprint.h"

/**
 * Retrieve a system variable 
 * @param[out] ierr the error code
 * @param[in] var the name of the variable
 * @param[out] buf the value of the variable
 * @param[out] buflen  the length of the variable
 * @param[in] iflag 
 */
void C2F(getenvc)(int *ierr,char *var,char *buf,int *buflen,int *iflag);


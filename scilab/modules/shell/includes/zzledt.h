/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __ZZLEDT_H__
#define __ZZLEDT_H__

#include "machine.h" /* C2F */

/**
* line editor
* @param buffer
* @param buffer size
* @param length line
* @param eof
* @param menu flag
* @param mode
* @param dummy1 (fortran)
*/
void C2F(zzledt)(char *buffer,int *buf_size,int *len_line,int * eof,int *menusflag,int * modex,long int dummy1);

#endif /* __ZZLEDT_H__ */
/*-----------------------------------------------------------------------------------*/ 



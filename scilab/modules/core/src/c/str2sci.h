/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Sylvestre LEDRU */
/*-----------------------------------------------------------------------------------*/
#ifndef __STR2SCI_H__
#define __STR2SCI_H__
#include "machine.h"
/** C string  var2vec  to scilab
 *  @param x is supposed to be a fortran image of var2vec result
 */
void str2sci(char** x,int n,int m);

#endif /* __STR2SCI_H__ */
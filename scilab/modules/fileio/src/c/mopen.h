/*-----------------------------------------------------------------------------------*/
/* Scilab */
/* INRIA 2007 */
/*-----------------------------------------------------------------------------------*/
#ifndef __MOPEN_H__
#define __MOPEN_H__

#include "machine.h"

/**
* Opens file given by file and return it's id 
* @param[out] fd (id)
* @param filename
* @param status "r","w","a" or "rb","wb","ab"
* @param swap
* @param[out] res
* @param[out] error
*/

void C2F(mopen)(int *fd, char *file, char *status, int *f_swap, double *res, int *error);

#endif  /* __MOPEN_H__ */
/*-----------------------------------------------------------------------------------*/

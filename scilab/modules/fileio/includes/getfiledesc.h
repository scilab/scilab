/*--------------------------------------------------------------------------*/
/* Scilab */
/* INRIA 2007 */
/*--------------------------------------------------------------------------*/
#ifndef __GETFILEDESC_H__
#define __GETFILEDESC_H__

#include "machine.h"

/**
* returns the first available position in Files descriptor Table
* @param fd (id)
* @param[out] fd (-1 error)
*/
void C2F(getfiledesc)(integer *fd); 

#endif /* __GETFILEDESC_H__ */
/*--------------------------------------------------------------------------*/

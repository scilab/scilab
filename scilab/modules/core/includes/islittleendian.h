/*-----------------------------------------------------------------------------------*/
/* Scilab */
/* INRIA 2007 */
/*-----------------------------------------------------------------------------------*/
#ifndef __ISLITTLEENDIAN_H__
#define __ISLITTLEENDIAN_H__

#include "machine.h"

/**
* islittleendian 
* @return if it is a little endian (0 or 1)
*/
int islittleendian(void);

/**
* C2F(getendian)
* @param void  
* @return the endian (0 or 1)
*/
int C2F(getendian)(void);

#endif /* __ISLITTLEENDIAN_H__ */
/*-----------------------------------------------------------------------------------*/

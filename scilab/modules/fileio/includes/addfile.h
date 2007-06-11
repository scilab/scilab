/*-----------------------------------------------------------------------------------*/
/* Scilab */
/* INRIA 2007 */
/*-----------------------------------------------------------------------------------*/
#ifndef __ADDFILE_H__
#define __ADDFILE_H__

#include <stdio.h>
#include "machine.h"

/**
 *add a file in the files table
 *
 * @param fd    
 * @param fa    
 * @param swap2 
 * @param type  
 * @param mode  
 * @param filename  
 * @param ierr  
 */
void C2F(addfile)(integer *fd, FILE *fa, integer *swap2, integer *type, integer *mode, char *filename, integer *ierr);

#endif /*  __ADDFILE_H__ */
/*-----------------------------------------------------------------------------------*/

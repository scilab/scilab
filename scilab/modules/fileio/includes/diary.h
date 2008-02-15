/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Sylvestre LEDRU
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#ifndef __DIARY_H__
#define __DIARY_H__

#include "machine.h"

/**
* TODO
* @return
*/
int getdiary(void);

/**
* TODO
* @param str
* @param n
*/
void diary(char *str,long int *n);


/**
* TODO
* @param str
* @param n
*/
void diary_nnl(char *str,int *n);

#endif /* __DIARY_H__ */


/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Sylvestre LEDRU
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#ifndef __READLINE_H__
#define __READLINE_H__

#include "machine.h"
int LineRead(FILE *fd,char buf[],int n,int *cnt,int *nr);
void C2F(readnextline)(int *fd,char buf[],int *n,int *count,int *nr,int *ierr);

#endif /* __READLINE_H__ */

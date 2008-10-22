/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#ifndef __MERROR_H__
#define __MERROR_H__

#include "machine.h"

/**
 * check error
 *
 * @param fd    
 * @param res   
 */
void C2F(merror) (int *fd, double *res);

#endif /* __MERROR_H__ */
/*--------------------------------------------------------------------------*/

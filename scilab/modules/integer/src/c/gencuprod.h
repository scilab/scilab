
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Sylvestre LEDRU
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#ifndef __GENCUPROD_H__
#define __GENCUPROD_H__
#include "machine.h"
#include "def.h"

/**
 * TODO : comment 
 * @param typ
 * @param n
 * @param w be carreful using this parameter (crappy way of doing polymorphism)
 * @param iw
 * @return 
 */
int C2F(gencuprod)(int *typ,int *n, void *w, int *iw);

#endif /* __GENCUPROD_H__ */

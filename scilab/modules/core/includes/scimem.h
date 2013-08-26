/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Sylvestre LEDRU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef __SCIMEM_H__
#define __SCIMEM_H__
#include "machine.h"

int C2F(scimem) (int *newsize, int *ptr);
int C2F(scigmem) (int *newsize, int *ptr);

void C2F(freestacklastmemory)();
void C2F(freeglobalstacklastmemory)();

void freeStackCurrentMemory();
void freeGlobalStackCurrentMemory();
#endif /* __SCIMEM_H_ */
/*--------------------------------------------------------------------------*/

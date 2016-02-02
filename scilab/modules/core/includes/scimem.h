/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Sylvestre LEDRU
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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

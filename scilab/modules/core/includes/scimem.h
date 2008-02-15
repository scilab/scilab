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

#ifndef __SCIMEM_H__
#define __SCIMEM_H__

#include "machine.h"

integer C2F(scimem)(integer *n, integer *ptr);
integer C2F(scigmem)(integer *n, integer *ptr);

void C2F(freegmem)(void);
void C2F(freemem)(void);
#endif /* __SCIMEM_H_ */
/*--------------------------------------------------------------------------*/

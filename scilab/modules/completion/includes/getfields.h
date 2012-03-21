/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __GETFIELDS_H__
#define __GETFIELDS_H__

/**
 * Get the fields name for a type and a mlist
 * @param typeName the type name
 * @param mlist the mlist address
 * @return the arry of fields name
 */
const char ** getFieldsForType(const char * typeName, int * mlist);

/**
 * Initialization
 */
void initializeFieldsGetter();

#endif

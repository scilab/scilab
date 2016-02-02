/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Calixte DENIZET
 * Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
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

#ifndef __GETFIELDS_H__
#define __GETFIELDS_H__

/**
 * Get the fields name for a type and a mlist
 * @param typeName the type name
 * @param mlist the mlist address
 * @return the arry of fields name
 */
const char ** getFieldsForType(const char * typeName, int * mlist, char ** fieldPath, const int fieldPathLen, int * fieldsSize);

const char ** getFields(int * mlist, char ** fieldPath, const int fieldPathLen, int * fieldsSize);

/**
 * Initialization
 */
void initializeFieldsGetter();

char ** getFieldPath(const char * _str, int * len);

#endif

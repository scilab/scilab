/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - 2012 - INRIA - Allan CORNET
 * Copyright (C) 2011 - INRIA - Michael Baudin
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 * This code is also published under the GPL v3 license.
 *
 */

#ifndef __GW_CSV_HELPERS_H__
#define __GW_CSV_HELPERS_H__


char *csv_getArgumentAsStringWithEmptyManagement(void* _pvCtx, int _iVar,
        const char *fname, const char *defaultValue,
        int *iErr);

char *csv_getArgumentAsString(void* _pvCtx, int _iVar,
                              const char *fname, int *iErr);

int csv_getArgumentAsScalarBoolean(void* _pvCtx, int _iVar,
                                   const char *fname, int *iErr);

char **csv_getArgumentAsMatrixOfString(void* _pvCtx, int _iVar,
                                       const char *fname,
                                       int *m, int *n, int *iErr);

int *csv_getArgumentAsMatrixofIntFromDouble(void* _pvCtx, int _iVar,
        const char *fname,
        int *m, int *n, int *iErr);

double csv_getArgumentAsScalarDouble(void* _pvCtx, int _iVar,
                                     const char *fname, int *iErr);


int csv_isRowVector(void* _pvCtx, int _iVar);
int csv_isColumnVector(void* _pvCtx, int _iVar);
int csv_isScalar(void* _pvCtx, int _iVar);
int csv_isDoubleScalar(void* _pvCtx, int _iVar);
int csv_isEmpty(void* _pvCtx, int _iVar);

#endif /* __GW_CSV_HELPERS_H__ */
// =============================================================================


/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - 2012 - INRIA - Allan CORNET
 * Copyright (C) 2011 - INRIA - Michael Baudin
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


/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010-2011 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#ifndef __CSV_COMPLEX_H__
#define __CSV_COMPLEX_H__

typedef struct
{
    double *realPart;
    double *imagPart;
    int nbElements;
    int isComplex;
} csv_complexArray;

#ifdef __cplusplus
extern "C" {
#endif

    csv_complexArray *createCsvComplexArray(const double *realpart, const double *imagpart, int nbSize);
    csv_complexArray *createCsvComplexArrayEmpty(int nbSize);
    int cleanImagPartCsvComplexArray(csv_complexArray *pCsvComplexArray);
    void freeCsvComplexArray(csv_complexArray *pCsvComplexArray);

#ifdef __cplusplus
}
#endif


#endif /* __CSV_COMPLEX_H__ */


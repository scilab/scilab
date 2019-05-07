/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010-2011 - DIGITEO - Allan CORNET
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
#ifndef __CSV_READ_H__
#define __CSV_READ_H__

#include <wchar.h>

typedef enum
{
    CSV_READ_NO_ERROR = 0,
    CSV_READ_MOPEN_ERROR = 1,
    CSV_READ_FILE_NOT_EXIST = 2,
    CSV_READ_MEMORY_ALLOCATION = 3,
    CSV_READ_READLINES_ERROR = 4,
    CSV_READ_COLUMNS_ERROR = 5,
    CSV_READ_ERROR = 6,
    CSV_READ_SEPARATOR_DECIMAL_EQUAL = 7,
    CSV_READ_REGEXP_ERROR = 8
} csvReadError;

typedef struct
{
    wchar_t **pwstrValues;
    int nbLines;
    wchar_t **pwstrComments;
    int nbComments;
    wchar_t **pwstrHeader;
    int nbHeader;
    csvReadError err;
} csvResult;

#ifdef __cplusplus
extern "C" {
#endif

csvResult* csvRead(const wchar_t *filename, const wchar_t *separator, const wchar_t *decimal,
                   wchar_t **toreplace, int sizetoreplace, const wchar_t *regexpcomments, int header);

int csvTextScanInPlace(wchar_t** text, int nbLines, const wchar_t* separator,
                       const wchar_t* decimal, int haveRange, const int* iRange, int m1, int n1,
                       wchar_t** pstrValues, double* pDblRealValues, double** pDblImgValues);

char* csvTextScanSize(wchar_t** lines, int* numberOfLines, const wchar_t* separator, int *rows, int *cols, int haveRange, int* iRange);

void freeCsvResult(csvResult *result);

#ifdef __cplusplus
}
#endif


#endif /* __CSV_READ_H__ */


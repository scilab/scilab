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
#ifndef __CSV_READ_H__
#define __CSV_READ_H__

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
    char **pstrValues;
    int m;
    int n;
    char **pstrComments;
    int nbComments;
    csvReadError err;
} csvResult;

#ifdef __cplusplus
extern "C" {
#endif

    csvResult* csvRead(const char *filename, const char *separator, const char *decimal,
                       const char **toreplace, int sizetoreplace, const char *regexpcomments);

    csvResult* csvTextScan(const char **lines, int numberOfLines, const char *separator, const char *decimal);

    void freeCsvResult(csvResult *result);

#ifdef __cplusplus
}
#endif


#endif /* __CSV_READ_H__ */


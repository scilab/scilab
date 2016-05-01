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
                   const char **toreplace, int sizetoreplace, const char *regexpcomments, int header);

csvResult* csvTextScan(const char **lines, int numberOfLines, const char *separator, const char *decimal);

void freeCsvResult(csvResult *result);

#ifdef __cplusplus
}
#endif


#endif /* __CSV_READ_H__ */


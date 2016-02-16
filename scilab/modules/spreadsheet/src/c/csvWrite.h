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

#ifndef __CSV_WRITE_H__
#define __CSV_WRITE_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    CSV_WRITE_NO_ERROR = 0,
    CSV_WRITE_FOPEN_ERROR = 1,
    CSV_WRITE_ERROR = 2,
    CSV_WRITE_SEPARATOR_DECIMAL_EQUAL = 3
}
csvWriteError;


csvWriteError csvWrite_double(const char *filename,
                              const double *pdValues, int m, int n,
                              const char *separator,
                              const char *decimal,
                              const char *precisionFormat,
                              const char **headersLines,
                              int nbHeadersLines);

csvWriteError csvWrite_complex(const char *filename,
                               const double *pdValuesReal,
                               const double *pdValuesImag,
                               int m, int n,
                               const char *separator,
                               const char *decimal,
                               const char *precisionFormat,
                               const char **headersLines,
                               int nbHeadersLines);

csvWriteError csvWrite_string(const char *filename,
                              const char **pStrValues, int m, int n,
                              const char *separator,
                              const char *decimal,
                              const char **headersLines,
                              int nbHeadersLines);

#ifdef __cplusplus
}
#endif

#endif /* __CSV_WRITE_H__ */
// =============================================================================


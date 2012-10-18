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

#ifndef __CSV_WRITE_H__
#define __CSV_WRITE_H__

#ifdef __cplusplus
extern "C" {
#endif

    typedef enum {
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


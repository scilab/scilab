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
#ifndef __CSV_DEFAULT_H__
#define __CSV_DEFAULT_H__

#ifdef __cplusplus
extern "C" {
#endif

const char *getCsvDefaultSeparator(void);
const char *getCsvDefaultDecimal(void);
const char *getCsvDefaultConversion(void);
const char *getCsvDefaultPrecision(void);
const char *getCsvDefaultCommentsRegExp(void);
const char *getCsvDefaultEOL(void);
const char *getCsvDefaultEncoding(void);
const char *getCsvDefaultCsvIgnoreBlankLine(void);

int setCsvDefaultSeparator(const char *separator);
int setCsvDefaultDecimal(const char *decimal);
int setCsvDefaultConversion(const char *conversion);
int setCsvDefaultPrecision(const char *precision);
int setCsvDefaultCommentsRegExp(const char *commentsRegExp);
int setCsvDefaultEOL(const char *eol);
int setCsvDefaultEncoding(const char *encoding);
int setCsvDefaultCsvIgnoreBlankLine(const char *blankMode);

int setCsvDefaultReset(void);

#ifdef __cplusplus
}
#endif


#endif /* __CSV_DEFAULT_H__ */
// =============================================================================

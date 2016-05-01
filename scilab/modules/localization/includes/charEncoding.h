/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
* Copyright (C) 2009 - DIGITEO - Antoine ELIAS , Allan CORNET
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
#ifndef __CHARENCODING_H__
#define __CHARENCODING_H__

#include <wchar.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "BOOL.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
* wcfopen macro for fopen Multibyte char multiplatform
* @param[out] fp file descriptor
* @param[in] x filename
* @param[in] y mode
*/

#ifdef _MSC_VER
#define wcfopen(fp, x,y) \
{\
	wchar_t* wfilename = NULL;\
	wchar_t* wmode = NULL;\
	wfilename = to_wide_string(x);\
	wmode = to_wide_string(y);\
	if(wfilename == NULL || wmode == NULL){fp = 0;}\
	else {fp = _wfopen(wfilename, wmode);}\
	if(wfilename != NULL){FREE(wfilename);}\
	if(wmode != NULL){FREE(wmode);} \
}
#else
#define wcfopen(fp, x,y) \
{\
	fp = fopen(x, y);\
}
#endif

/**
* convert a UTF string to wide char string
* @param[in] UTF string
* @return wide char string converted
*/
wchar_t* to_wide_string(const char *_UTFStr);

/**
* convert a wide char string to UTF-8
* @param[in] wide char string
* @return UTF string converted
*/
char *wide_string_to_UTF8(const wchar_t *_wide);

/*file management with UTF filename*/
#ifdef _MSC_VER
int wcstat(char* filename, struct _stat *st);
#endif

/**
* checks input text is a valid UTF-8 format
* @param[in] string to check
* @return TRUE or FALSE
*/
BOOL IsValidUTF8(const char* pStText);


#ifdef __cplusplus
}
#endif
#endif /* __CHARENCODING_H__ */


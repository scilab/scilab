
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Cong WU
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

/*------------------------------------------------------------------------*/
#ifndef __PCRE_PRIVATE_H__
#define __PCRE_PRIVATE_H__

#include "dynlib_string.h"
#include "charEncoding.h"
typedef enum
{
    PCRE_EXIT = 1,
    PCRE_FINISHED_OK = 0,
    NO_MATCH = -1,
    NOT_ENOUGH_MEMORY_FOR_VECTOR = -2,
    DELIMITER_NOT_ALPHANUMERIC = -3,
    CAPTURING_SUBPATTERNS_ERROR = -4,
    PARTIAL_MATCHING_NOT_SUPPORTED = -5,
    CONTAINS_EXPLICIT_CR_OR_LF_MATCH = -6,
    DUPLICATE_NAME_STATUS_CHANGES = -7,
    TOO_BIG_FOR_OFFSET_SIZE = -8,
    LIMIT_NOT_RELEVANT_FOR_DFA_MATCHING = -9,
    CAN_NOT_COMPILE_PATTERN = -10,
    MATCH_LIMIT = -11,
    UTF8_NOT_SUPPORTED = -12
} pcre_error_code;


STRING_IMPEXP pcre_error_code pcre_private(char *INPUT_LINE, char *INPUT_PAT, int *Output_Start, int *Output_End, char*** _pstCapturedString, int* _piCapturedStringCount);
STRING_IMPEXP pcre_error_code wide_pcre_private(wchar_t *INPUT_LINE, wchar_t *INPUT_PAT, int *Output_Start, int *Output_End, wchar_t*** _pstCapturedString, int* _piCapturedStringCount);

#endif /* __PCRE_PRIVATE_H__ */
/*------------------------------------------------------------------------*/

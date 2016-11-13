/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Sylvestre LEDRU
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


/**
 * File: pcre_error.c
 * desc : Display a human readable error message (PCRE error).
 */
#include "pcre_private.h"
#include "localization.h"
#include "Scierror.h"
#include "pcre_error.h"

void pcre_error(char const * fname, int errorCode)
{
    switch (errorCode)
    {
        case NO_MATCH:
            /*No match */
            break;
        case NOT_ENOUGH_MEMORY_FOR_VECTOR:
            Scierror(999, _("%s: No more memory.\n"), fname);
            break;
        case DELIMITER_NOT_ALPHANUMERIC:
            Scierror(999, _("%s: Delimiter must not be alphameric.\n"), fname);
            break;
        case CAPTURING_SUBPATTERNS_ERROR:
            Scierror(999, _("%s: Capturing subpatterns error.\n"), fname);
            break;
        case PARTIAL_MATCHING_NOT_SUPPORTED:
            Scierror(999, _("%s: Partial matching not supported.\n"), fname);
            break;
        case CONTAINS_EXPLICIT_CR_OR_LF_MATCH:
            Scierror(999, _("%s: Contains explicit CR or LF match.\n"), fname);
            break;
        case DUPLICATE_NAME_STATUS_CHANGES:
            Scierror(999, _("%s: Duplicate name status changes.\n"), fname);
            break;
        case TOO_BIG_FOR_OFFSET_SIZE:
            Scierror(999, _("%s: Returned count is too big for offset size.\n"), fname);
            break;
        case LIMIT_NOT_RELEVANT_FOR_DFA_MATCHING:
            Scierror(999, _("%s: Match limit not relevant for DFA matching: ignored.\n"), fname);
            break;
        case CAN_NOT_COMPILE_PATTERN:
            Scierror(999, _("%s: Can not compile pattern.\n"), fname);
            break;
        case MATCH_LIMIT:
            Scierror(999, _("%s: Matched text exceeds buffer limit.\n"), fname);
            break;
        case UTF8_NOT_SUPPORTED:
            Scierror(999, _("%s: Current PCRE library does not support UTF-8.\n"), fname);
            break;
        default:
            Scierror(999, _("%s: Unknown error.\n"), fname);
            break;
    }
}

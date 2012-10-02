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
#ifndef __CSV_STRSUBST_H__
#define __CSV_STRSUBST_H__

#ifdef __cplusplus
extern "C" {
#endif

    char * csv_strsubst(const char* input_string,
                        const char* string_to_search,
                        const char* replacement_string);

#ifdef __cplusplus
}
#endif


#endif /* __CSV_STRSUBST_H__ */
// =============================================================================

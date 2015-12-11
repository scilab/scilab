/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010-2011 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
#ifndef __SPLITLINE_H__
#define __SPLITLINE_H__

#ifdef __cplusplus
extern "C" {
#endif

    /**
    * split a line by separator
    */
    char **splitLineCSV(const char *str, const char *sep, int *toks);

#ifdef __cplusplus
}
#endif


#endif /* __SPLITLINE_H__ */
/* ==================================================================== */

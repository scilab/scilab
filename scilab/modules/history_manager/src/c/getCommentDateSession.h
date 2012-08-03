/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Allan CORNET
 * Copyright (C) 2010 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
#ifndef __GETCOMMENTDATESESSION_H__
#define __GETCOMMENTDATESESSION_H__
/*------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C"
{
#endif
/*------------------------------------------------------------------------*/
#include "BOOL.h"               /* BOOL */
/*------------------------------------------------------------------------*/
#define SESSION_PRAGMA_BEGIN "// -- "
#define SESSION_PRAGMA_END " -- //"
/*------------------------------------------------------------------------*/
    char *getCommentDateSession(BOOL longFormat);
/*------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
/*------------------------------------------------------------------------*/
#endif                          /* __GETCOMMENTDATESESSION_H__ */
/*------------------------------------------------------------------------*/

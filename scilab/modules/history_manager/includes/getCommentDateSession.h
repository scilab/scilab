/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Allan CORNET
 * Copyright (C) 2010 - DIGITEO - Allan CORNET
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
#ifndef __GETCOMMENTDATESESSION_H__
#define __GETCOMMENTDATESESSION_H__
/*------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif
/*------------------------------------------------------------------------*/
#include "dynlib_history_manager.h"
#include "BOOL.h" /* BOOL */
#include <wchar.h>
/*------------------------------------------------------------------------*/
#define SESSION_PRAGMA_BEGIN    "// -- "
#define SESSION_PRAGMA_END      " -- //"
/*------------------------------------------------------------------------*/
HISTORY_MANAGER_IMPEXP char *getCommentDateSession(BOOL longFormat);
/*------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
/*------------------------------------------------------------------------*/
#endif /* __GETCOMMENTDATESESSION_H__ */
/*------------------------------------------------------------------------*/

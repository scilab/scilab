/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * Copyright (C) 2010 - DIGITEO - Yann COLLETTE
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

#ifndef __GETMATLABVARIABLE_H__
#define __GETMATLABVARIABLE_H__

#include "matfile_manager.h"
#include "dynlib_matio.h"

#include "sci_malloc.h"
#include "localization.h"
#include "Scierror.h"
#include "sciprint.h"

/*
 * Get a Matlab variable from a Scilab variable on stack
 * This function reads nothing on stack but calls the function which
 * does the work according to Scilab Variable type
 *
 * @param pvApiCtx Scilab context
 * @param iVar position on the stack
 * @param name the name of the variable
 * @param matfile_version version of the file to write (See MATIO library)
 * @param parent parent mlist. If NULL, a normal variable is created.
 * @parem item_position position of the variable in the mlist. Only effective if parent !=NULL
 *
 * @return Matlab variable (See MATIO library)
 */
MATIO_IMPEXP matvar_t *GetMatlabVariable(void *pvApiCtx, int iVar, const char *name, int matfile_version, int * parent, int item_position);

/*
 * Get a Matlab Integer variable from stack
 *
 * @param pvApiCtx Scilab context
 * @param iVar position on the stack
 * @param name the name of the variable
 * @param parent parent mlist. If NULL, a normal variable is created.
 * @parem item_position position of the variable in the mlist. Only effective if parent !=NULL
 *
 * @return Matlab variable (See MATIO library)
 */
matvar_t *GetIntegerVariable(void *pvApiCtx, int iVar, const char *name, int * parent, int item_position);

/*
 * Get a Matlab Mlist-equivalent variable from stack
 *
 * @param pvApiCtx Scilab context
 * @param iVar position on the stack
 * @param name the name of the variable
 * @param matfile_version version of the file to write (See MATIO library)
 * @param parent parent mlist. If NULL, a normal variable is created.
 * @parem item_position position of the variable in the mlist. Only effective if parent !=NULL
 *
 * @return Matlab variable (See MATIO library)
 */
matvar_t *GetMlistVariable(void *pvApiCtx, int iVar, const char *name, int matfile_version, int * parent, int item_position);

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Get a Matlab Struct variable from stack
 *
 * @param pvApiCtx Scilab context
 * @param iVar position on the stack
 * @param name the name of the variable
 * @param matfile_version version of the file to write (See MATIO library)
 * @param fieldNames structure fields names including "st" and "dims"
 * @param nbFields number of fields including "st" and "dims"
 * @param parent parent mlist. If NULL, a normal variable is created.
 * @parem item_position position of the variable in the mlist. Only effective if parent !=NULL
 *
 * @return Matlab variable (See MATIO library)
 */
// MATIO_IMPEXP matvar_t *GetStructVariable(void *pvApiCtx, int iVar, const char *name, int matfile_version, char **fieldNames, int nbFields, int * parent, int item_position);
MATIO_IMPEXP matvar_t *GetStructVariable(void *pvApiCtx, int iVar, const char *name, int matfile_version, int * parent, int item_position);


/*
 * Get a Matlab Cell variable from stack
 *
 * @param pvApiCtx Scilab context
 * @param iVar position on the stack
 * @param name the name of the variable
 * @param matfile_version version of the file to write (See MATIO library)
 * @param parent parent mlist. If NULL, a normal variable is created.
 * @parem item_position position of the variable in the mlist. Only effective if parent !=NULL
 *
 * @return Matlab variable (See MATIO library)
 */
MATIO_IMPEXP matvar_t *GetCellVariable(void *pvApiCtx, int iVar, const char *name, int matfile_version, int * parent, int item_position);

#ifdef __cplusplus
}
#endif

/*
 * Get a Matlab Char variable from stack
 *
 * @param pvApiCtx Scilab context
 * @param iVar position on the stack
 * @param name the name of the variable
 * @param parent parent mlist. If NULL, a normal variable is created.
 * @parem item_position position of the variable in the mlist. Only effective if parent !=NULL
 *
 * @return Matlab variable (See MATIO library)
 */
matvar_t *GetCharVariable(void *pvApiCtx, int iVar, const char *name, int * parent, int item_position);

/*
 * Get a Matlab Sparse variable from stack
 *
 * @param pvApiCtx Scilab context
 * @param iVar position on the stack
 * @param name the name of the variable
 * @param parent parent mlist. If NULL, a normal variable is created.
 * @parem item_position position of the variable in the mlist. Only effective if parent !=NULL
 *
 * @return Matlab variable (See MATIO library)
 */
matvar_t *GetSparseVariable(void *pvApiCtx, int iVar, const char *name, int * parent, int item_position);

/*
 * Get a Matlab Double variable from stack
 *
 * @param pvApiCtx Scilab context
 * @param iVar position on the stack
 * @param name the name of the variable
 * @param matfile_version version of the file to write (See MATIO library)
 * @param parent parent mlist. If NULL, a normal variable is created.
 * @parem item_position position of the variable in the mlist. Only effective if parent !=NULL
 *
 * @return Matlab variable (See MATIO library)
 */
matvar_t *GetDoubleVariable(void *pvApiCtx, int iVar, const char *name, int matfile_version, int * parent, int item_position);

#endif /* !__GETMATLABVARIABLE_H__ */

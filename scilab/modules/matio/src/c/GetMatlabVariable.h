/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT 
 * Copyright (C) 2010 - DIGITEO - Yann COLLETTE
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __GETMATLABVARIABLE_H__
#define __GETMATLABVARIABLE_H__

#include "matfile_manager.h"
#include "MALLOC.h"
#include "localization.h"
#include "Scierror.h"
#include "sciprint.h"

/*
 * Get a Matlab variable from a Scilab variable on stack
 * This function reads nothing on stack but calls the function which 
 * does the work according to Scilab Variable type
 * 
 * @param iVar position on the stack
 * @param name the name of the variable
 * @param matfile_version version of the file to write (See MATIO library)
 * @param parent parent mlist. If NULL, a normal variable is created.
 * @parem item_position position of the variable in the mlist. Only effective if parent !=NULL
 *
 * @return Matlab variable (See MATIO library)
 */
matvar_t *GetMatlabVariable(int iVar, const char *name, int matfile_version, int * parent, int item_position);

/*
 * Get a Matlab Integer variable from stack
 * 
 * @param iVar position on the stack
 * @param name the name of the variable
 * @param parent parent mlist. If NULL, a normal variable is created.
 * @parem item_position position of the variable in the mlist. Only effective if parent !=NULL
 *
 * @return Matlab variable (See MATIO library)
 */
matvar_t *GetIntegerVariable(int iVar, const char *name, int * parent, int item_position);

/*
 * Get a Matlab Mlist-equivalent variable from stack
 * 
 * @param iVar position on the stack
 * @param name the name of the variable
 * @param matfile_version version of the file to write (See MATIO library)
 * @param parent parent mlist. If NULL, a normal variable is created.
 * @parem item_position position of the variable in the mlist. Only effective if parent !=NULL
 *
 * @return Matlab variable (See MATIO library)
 */
matvar_t *GetMlistVariable(int iVar, const char *name, int matfile_version, int * parent, int item_position);

/*
 * Get a Matlab Struct variable from stack
 * 
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
matvar_t *GetStructVariable(int iVar, const char *name, int matfile_version, char **fieldNames, int nbFields, int * parent, int item_position);

/*
 * Get a Matlab Cell variable from stack
 * 
 * @param iVar position on the stack
 * @param name the name of the variable
 * @param matfile_version version of the file to write (See MATIO library)
 * @param parent parent mlist. If NULL, a normal variable is created.
 * @parem item_position position of the variable in the mlist. Only effective if parent !=NULL
 *
 * @return Matlab variable (See MATIO library)
 */
matvar_t *GetCellVariable(int iVar, const char *name, int matfile_version, int * parent, int item_position);

/*
 * Get a Matlab Char variable from stack
 * 
 * @param iVar position on the stack
 * @param name the name of the variable
 * @param parent parent mlist. If NULL, a normal variable is created.
 * @parem item_position position of the variable in the mlist. Only effective if parent !=NULL
 *
 * @return Matlab variable (See MATIO library)
 */
matvar_t *GetCharVariable(int iVar, const char *name, int * parent, int item_position);

/*
 * Get a Matlab Sparse variable from stack
 * 
 * @param iVar position on the stack
 * @param name the name of the variable
 * @param parent parent mlist. If NULL, a normal variable is created.
 * @parem item_position position of the variable in the mlist. Only effective if parent !=NULL
 *
 * @return Matlab variable (See MATIO library)
 */
matvar_t *GetSparseVariable(int iVar, const char *name, int * parent, int item_position);

/*
 * Get a Matlab Double variable from stack
 * 
 * @param iVar position on the stack
 * @param name the name of the variable
 * @param matfile_version version of the file to write (See MATIO library)
 * @param parent parent mlist. If NULL, a normal variable is created.
 * @parem item_position position of the variable in the mlist. Only effective if parent !=NULL
 *
 * @return Matlab variable (See MATIO library)
 */
matvar_t *GetDoubleVariable(int iVar, const char *name, int matfile_version, int * parent, int item_position);

#endif /* !__GETMATLABVARIABLE_H__ */

/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT 
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
#include "stack-c.h" /* stack-def.h, stack1.h, stack2.h, stack3.h included */
#include "localization.h"
#include "Scierror.h"
#include "sciprint.h"

/*
 * Get a Matlab variable from a Scilab variable on stack
 * This function reads nothing on stack but calls the function which 
 * does the work according to Scilab Variable type
 * 
 * @param stkPos position on the stack
 * @param name the name of the variable
 * @param matfile_version version of the file to write (See MATIO library)
 *
 * @return Matlab variable (See MATIO library)
 */
matvar_t *GetMatlabVariable(int stkPos, const char *name, int matfile_version);

/*
 * Get a Matlab Integer variable from stack
 * 
 * @param stkPos position on the stack
 * @param name the name of the variable
 *
 * @return Matlab variable (See MATIO library)
 */
matvar_t *GetIntegerVariable(int stkPos, const char *name);

/*
 * Get a Matlab Mlist-equivalent variable from stack
 * 
 * @param stkPos position on the stack
 * @param name the name of the variable
 * @param matfile_version version of the file to write (See MATIO library)
 *
 * @return Matlab variable (See MATIO library)
 */
matvar_t *GetMlistVariable(int stkPos, const char *name, int matfile_version);

/*
 * Get a Matlab Struct variable from stack
 * 
 * @param stkPos position on the stack
 * @param name the name of the variable
 * @param matfile_version version of the file to write (See MATIO library)
 * @param fieldNames structure fields names including "st" and "dims"
 * @param nbFields number of fields including "st" and "dims"
 *
 * @return Matlab variable (See MATIO library)
 */
matvar_t *GetStructVariable(int stkPos, const char *name, int matfile_version, char **fieldNames, int nbFields);

/*
 * Get a Matlab Cell variable from stack
 * 
 * @param stkPos position on the stack
 * @param name the name of the variable
 * @param matfile_version version of the file to write (See MATIO library)
 *
 * @return Matlab variable (See MATIO library)
 */
matvar_t *GetCellVariable(int stkPos, const char *name, int matfile_version);

/*
 * Get a Matlab Char variable from stack
 * 
 * @param stkPos position on the stack
 * @param name the name of the variable
 *
 * @return Matlab variable (See MATIO library)
 */
matvar_t *GetCharVariable(int stkPos, const char *name);

/*
 * Get a Matlab Sparse variable from stack
 * 
 * @param stkPos position on the stack
 * @param name the name of the variable
 *
 * @return Matlab variable (See MATIO library)
 */
matvar_t *GetSparseVariable(int stkPos, const char *name);

/*
 * Get a Matlab Double variable from stack
 * 
 * @param stkPos position on the stack
 * @param name the name of the variable
 * @param matfile_version version of the file to write (See MATIO library)
 *
 * @return Matlab variable (See MATIO library)
 */
matvar_t *GetDoubleVariable(int stkPos, const char *name, int matfile_version);

#endif /* !__GETMATLABVARIABLE_H__ */

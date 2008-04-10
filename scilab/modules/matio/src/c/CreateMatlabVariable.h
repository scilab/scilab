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

#ifndef __CREATEMATLABVARIABLE_H__
#define __CREATEMATLABVARIABLE_H__

#include "matfile_manager.h"
#include "MALLOC.h"
#include "stack-c.h" /* stack-def.h, stack1.h, stack2.h, stack3.h included */
#include "localization.h"
#include "Scierror.h"
#include "sciprint.h"

/*
 * Create a Scilab variable on stack from a Matlab variable
 * This function write nothing on stack but calls the function which 
 * does the work according to Matlab Variable type
 * 
 * @param stkPos position on the stack
 * @param matVariable Matlab variable (See MATIO library)
 *
 * @return TRUE if the variable has been written without problem
 */
int CreateMatlabVariable(int stkPos, matvar_t *matVariable);


/*
 * Create a Scilab Integer variable on stack from a Matlab variable
 * 
 * @param stkPos position on the stack
 * @param integerType Scilab integer type (See stack-c.h)
 * @param matVariable Matlab variable (See MATIO library)
 *
 * @return TRUE if the variable has been written without problem
 */
int CreateIntegerVariable(int stkPos, int integerType, matvar_t *matVariable);

/*
 * Create a Scilab HyperMatrix variable on stack from a Matlab variable
 * 
 * @param stkPos position on the stack
 * @param type contents data type
 * @param iscomplex TRUE if the variable is Complex
 * @param rank number of dimensions of the variable
 * @param dims variable dimensions
 * @param realdata pointer to real data
 * @param complexdata pointer to complex data
 *
 * @return TRUE if the variable has been written without problem
 */
int CreateHyperMatrixVariable(int stkPos, const char *type, int *iscomplex, int * rank, int *dims, double *realdata, double *complexdata);

/*
 * Create a Scilab Struct variable on stack from a Matlab variable
 * 
 * @param stkPos position on the stack
 * @param matVariable Matlab variable (See MATIO library)
 *
 * @return TRUE if the variable has been written without problem
 */
int CreateStructVariable(int stkPos, matvar_t *matVariable);

/*
 * Create a Scilab Cell variable on stack from a Matlab variable
 * 
 * @param stkPos position on the stack
 * @param matVariable Matlab variable (See MATIO library)
 *
 * @return TRUE if the variable has been written without problem
 */
int CreateCellVariable(int stkPos, matvar_t *matVariable);

/*
 * Create a Scilab Char variable on stack from a Matlab variable
 * 
 * @param stkPos position on the stack
 * @param matVariable Matlab variable (See MATIO library)
 *
 * @return TRUE if the variable has been written without problem
 */
int CreateCharVariable(int stkPos, matvar_t *matVariable);

/*
 * Create a Scilab Sparse variable on stack from a Matlab variable
 * 
 * @param stkPos position on the stack
 * @param matVariable Matlab variable (See MATIO library)
 *
 * @return TRUE if the variable has been written without problem
 */
int CreateSparseVariable(int stkPos, matvar_t *matVariable);

/*
 * Create a Scilab Double variable on stack from a Matlab variable
 * 
 * @param stkPos position on the stack
 * @param matVariable Matlab variable (See MATIO library)
 *
 * @return TRUE if the variable has been written without problem
 */
int CreateDoubleVariable(int stkPos, matvar_t *matVariable);

#endif /* !__CREATEMATLABVARIABLE_H__ */

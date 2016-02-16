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

#ifndef __CREATEMATLABVARIABLE_H__
#define __CREATEMATLABVARIABLE_H__

#include "matfile_manager.h"
#include "dynlib_matio.h"

/*
 * Create a Scilab variable on stack from a Matlab variable
 * This function write nothing on stack but calls the function which
 * does the work according to Matlab Variable type
 *
 * @param pvApiCtx Scilab context
 * @param iVar position on the stack
 * @param matVariable Matlab variable (See MATIO library)
 * @param parent parent mlist. If NULL, a normal variable is created.
 * @param item_position position of the variable in the mlist. Only effective if parent !=NULL
 *
 * @return TRUE if the variable has been written without problem
 */
MATIO_IMPEXP int CreateMatlabVariable(void *pvApiCtx, int iVar, matvar_t *matVariable, int * parent, int item_position);


/*
 * Create a Scilab Integer variable on stack from a Matlab variable
 *
 * @param pvApiCtx Scilab context
 * @param iVar position on the stack
 * @param integerType Scilab int type (See stack-c.h)
 * @param matVariable Matlab variable (See MATIO library)
 * @param parent parent mlist. If NULL, a normal variable is created.
 * @param item_position position of the variable in the mlist. Only effective if parent !=NULL
 *
 * @return TRUE if the variable has been written without problem
 */
int CreateIntegerVariable(void *pvApiCtx, int iVar, int integerType, matvar_t *matVariable, int * parent, int item_position);

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Create a Scilab HyperMatrix variable on stack from a Matlab variable
 *
 * @param pvApiCtx Scilab context
 * @param iVar position on the stack
 * @param type contents data type
 * @param iscomplex TRUE if the variable is Complex
 * @param rank number of dimensions of the variable
 * @param dims variable dimensions
 * @param realdata pointer to real data
 * @param complexdata pointer to complex data
 * @param parent parent mlist. If NULL, a normal variable is created.
 * @param item_position position of the variable in the mlist. Only effective if parent !=NULL
 *
 * @return TRUE if the variable has been written without problem
 */
MATIO_IMPEXP int CreateHyperMatrixVariable(void *pvApiCtx, int iVar, int type, int *iscomplex, int * rank, int *dims, matvar_t *matVariable, int * parent, int item_position);

/*
* Create a Scilab Cell variable on stack from a Matlab variable
*
* @param pvApiCtx Scilab context
* @param iVar position on the stack
* @param matVariable Matlab variable (See MATIO library)
* @param parent parent mlist. If NULL, a normal variable is created.
* @param item_position position of the variable in the mlist. Only effective if parent !=NULL
*
* @return TRUE if the variable has been written without problem
*/
MATIO_IMPEXP int CreateCellVariable(void *pvApiCtx, int iVar, matvar_t *matVariable, int * parent, int item_position);

/*
* Create a Scilab Struct variable on stack from a Matlab variable
*
* @param pvApiCtx Scilab context
* @param iVar position on the stack
* @param matVariable Matlab variable (See MATIO library)
* @param parent parent mlist. If NULL, a normal variable is created.
* @param item_position position of the variable in the mlist. Only effective if parent !=NULL
*
* @return TRUE if the variable has been written without problem
*/
MATIO_IMPEXP int CreateStructVariable(void *pvApiCtx, int iVar, matvar_t *matVariable, int * parent, int item_position);

#ifdef __cplusplus
}
#endif

/*
 * Create a Scilab Char variable on stack from a Matlab variable
 *
 * @param pvApiCtx Scilab context
 * @param iVar position on the stack
 * @param matVariable Matlab variable (See MATIO library)
 * @param parent parent mlist. If NULL, a normal variable is created.
 * @param item_position position of the variable in the mlist. Only effective if parent !=NULL
 *
 * @return TRUE if the variable has been written without problem
 */
int CreateCharVariable(void *pvApiCtx, int iVar, matvar_t *matVariable, int * parent, int item_position);

/*
 * Create a Scilab Sparse variable on stack from a Matlab variable
 *
 * @param pvApiCtx Scilab context
 * @param iVar position on the stack
 * @param matVariable Matlab variable (See MATIO library)
 * @param parent parent mlist. If NULL, a normal variable is created.
 * @param item_position position of the variable in the mlist. Only effective if parent !=NULL
 *
 * @return TRUE if the variable has been written without problem
 */
int CreateSparseVariable(void *pvApiCtx, int iVar, matvar_t *matVariable, int * parent, int item_position);

/*
 * Create a Scilab Double variable on stack from a Matlab variable
 *
 * @param pvApiCtx Scilab context
 * @param iVar position on the stack
 * @param matVariable Matlab variable (See MATIO library)
 * @param parent parent mlist. If NULL, a normal variable is created.
 * @param item_position position of the variable in the mlist. Only effective if parent !=NULL
 *
 * @return TRUE if the variable has been written without problem
 */
int CreateDoubleVariable(void *pvApiCtx, int iVar, matvar_t *matVariable, int * parent, int item_position);

/*
 * Create a Scilab Boolean variable on stack from a Matlab variable
 *
 * @param pvApiCtx Scilab context
 * @param iVar position on the stack
 * @param matVariable Matlab variable (See MATIO library)
 * @param parent parent mlist. If NULL, a normal variable is created.
 * @param item_position position of the variable in the mlist. Only effective if parent !=NULL
 *
 * @return TRUE if the variable has been written without problem
 */
int CreateBooleanVariable(void *pvApiCtx, int iVar, matvar_t *matVariable, int * parent, int item_position);

#endif /* !__CREATEMATLABVARIABLE_H__ */

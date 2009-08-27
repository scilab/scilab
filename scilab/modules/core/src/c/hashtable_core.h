/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * Copyright (C) DIGITEO - 2009 - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#ifndef __HASHTABLE_CORE_H__
#define __HASHTABLE_CORE_H__
/*--------------------------------------------------------------------------*/
#include "addinter.h"
#include "BOOL.h"
/*--------------------------------------------------------------------------*/
typedef enum 
{
	SCI_HFUNCTIONS_FIND = 1,
	SCI_HFUNCTIONS_BACKSEARCH = 2,
	SCI_HFUNCTIONS_ENTER = 3,
	SCI_HFUNCTIONS_DELETE = 4
} SCI_HFUNCTIONS_ACTION;
/*--------------------------------------------------------------------------*/
/**
 * scilab 5.2 can manage 550000 primitives (max) MAXELEMENTFUNCTIONLIST
 * initialized to DEFAULT_ELEMENTFUNCTIONLIST (800) by default
 * dynamic reallocation when we add a function if it is necessary
 */
#define MAXELEMENTFUNCTIONLIST (DynInterfStart + MAXDYNINTERF) * NumberMaxFunctionsByGateway
#define DEFAULT_ELEMENTFUNCTIONLIST 800
/*--------------------------------------------------------------------------*/
/** 
 * Create the hashtable of Scilab functions
 * @return TRUE or FALSE
 */
BOOL create_hashtable_scilab_functions(void);

/** 
 * Destroy the hashtable of Scilab functions
 */
void destroy_hashtable_scilab_functions(void);

/** 
 * Perform an action on the hashtable of Scilab functions
 * @param[in,out] key hash key
 * @param[in,out] name function name
 * @param[in,out] scilab_funptr coded by GATEWAY_ID * 1000 + PRIMITIVE_ID
 * @param[in] action SCI_HFUNCTIONS_ACTION 
 * @return TRUE or FALSE
 */
BOOL action_hashtable_scilab_functions(int *key,char *name, int *scilab_funptr, SCI_HFUNCTIONS_ACTION action);

/*--------------------------------------------------------------------------*/
#endif /* __HASHTABLE_CORE_H__ */
/*--------------------------------------------------------------------------*/

/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
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
#include "stack-def.h"
#include "addinter.h"
/*--------------------------------------------------------------------------*/
#ifdef FAILED
	#undef FAILED
#endif

#ifdef OK
	#undef OK
#endif
#define FAILED 0
#define OK 1

#define MAXLENGHTFUNCTIONNAME 256 /* 24 in fact in scilab */
/*--------------------------------------------------------------------------*/
typedef struct entry 
{
	int key[nsiz];
	int data;
	char namefunction[MAXLENGHTFUNCTIONNAME];
} ENTRY;
/*--------------------------------------------------------------------------*/
typedef struct 
{ 
	unsigned int   used;
	ENTRY entry;
} _ENTRY;
/*--------------------------------------------------------------------------*/
typedef enum 
{
	SCI_HFUNCTIONS_FIND = 1,
	SCI_HFUNCTIONS_BACKSEARCH = 2,
	SCI_HFUNCTIONS_ENTER = 3,
	SCI_HFUNCTIONS_DELETE = 4
} SCI_HFUNCTIONS_ACTION;
/*--------------------------------------------------------------------------*/
/* maximum number of entries in the htable 
 * in fact create_hashtable_scilab_functions used a 
 * prime > MAXELEMENTFUNCTIONLIST 
 * this limitation should be removed with scilab 6
 */
#define MAXELEMENTFUNCTIONLIST (DynInterfStart + MAXDYNINTERF) * NumberMaxFunctionsByGateway
/* scilab 5 can manage 550000 primitives (max) */
/*--------------------------------------------------------------------------*/
/** 
 * Create the hashtable of Scilab functions
 * @param nelmax size max of the hashtable
 */
int	create_hashtable_scilab_functions(unsigned int nelmax);

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
 * @return result of the operation
 */
int action_hashtable_scilab_functions(int *key,char *name, int *scilab_funptr, SCI_HFUNCTIONS_ACTION action);

/*--------------------------------------------------------------------------*/
#endif /* __HASHTABLE_CORE_H__ */
/*--------------------------------------------------------------------------*/

/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#ifndef __STACKINFO_H__
#define __STACKINFO_H__
/*--------------------------------------------------------------------------*/ 
#include "machine.h"
#include "BOOL.h"
/*--------------------------------------------------------------------------*/ 
/**
* returns some infos about stack (local)
* @param[out] total 
* @param[out] used
* @return 0
*/
int C2F(getstackinfo)(int *total,int *used);

/**
* returns some infos about stack (global)
* @param[out] total 
* @param[out] used
* @return 0
*/
int C2F(getgstackinfo)(int *total,int *used);

/* elements on stacks */

/**
* returns some infos about variables on stack (local)
* @param[out] total
* @param[out] used
* @return 0
*/
int C2F(getvariablesinfo)(int *total,int *used);

/**
* returns some infos about variables on stack (global)
* @param[out] total
* @param[out] used
* @return 0
*/
int C2F(getgvariablesinfo)(int *total,int *used);

/**
* returns Intermediate Memory size needed (stack) used by "error(17)"
* @return Intermediate Memory size needed
*/
int getIntermediateMemoryNeeded(void);

/**
* check if sizestack is a valid size for scilab's stack
* @param sizestack
* @return TRUE or FALSE
*/
BOOL is_a_valid_size_for_scilab_stack(int sizestack);

/**
* returns max memory usable by scilab's stack
* @return max size
*/
unsigned long get_max_memory_for_scilab_stack(void);

/**
* get name of "n"th variable on local stack
* @return string variable name
*/
char *getLocalNamefromId(int n);

/**
* get name of "n"th variable on global stack
* @return string variable name
*/
char *getGlobalNamefromId(int n);

/**
* get size of "n"th variable on local stack
* @return int : size of variable
*/
int getLocalSizefromId(int n);

/**
* get size of "n"th variable on global stack
* @return int : size of variable
*/
int getGlobalSizefromId(int n);

/**
* check if a variable named exists on scilab stack (local & global)
* @param[in] variable name
* @return BOOL : TRUE if variable exists
*/
BOOL existVariableNamedOnStack(char *varname);

/**
* check if a variable named exists on scilab stack (local)
* @param[in] variable name
* @return BOOL : TRUE if variable exists
*/
BOOL existLocalVariableNamedOnStack(char *varname);

/**
* check if a variable named exists on scilab stack (global)
* @param[in] variable name
* @return BOOL : TRUE if variable exists
*/
BOOL existGlobalVariableNamedOnStack(char *varname);


#endif /*__STACKINFO_H__*/
/*--------------------------------------------------------------------------*/ 


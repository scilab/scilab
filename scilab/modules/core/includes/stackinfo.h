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
/*--------------------------------------------------------------------------*/ 
/**
* returns some infos about stack (local)
* @param[out] total 
* @param[out] used
* @return 0
*/
integer C2F(getstackinfo)(integer *total,integer *used);

/**
* returns some infos about stack (global)
* @param[out] total 
* @param[out] used
* @return 0
*/
integer C2F(getgstackinfo)(integer *total,integer *used);

/* elements on stacks */

/**
* returns some infos about variables on stack (local)
* @param[out] total
* @param[out] used
* @return 0
*/
integer C2F(getvariablesinfo)(integer *total,integer *used);

/**
* returns some infos about variables on stack (global)
* @param[out] total
* @param[out] used
* @return 0
*/
integer C2F(getgvariablesinfo)(integer *total,integer *used);

/**
* returns Intermediate Memory size needed (stack) used by "error(17)"
* @return Intermediate Memory size needed
*/
integer getIntermediateMemoryNeeded(void);

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

#endif /*__STACKINFO_H__*/
/*--------------------------------------------------------------------------*/ 


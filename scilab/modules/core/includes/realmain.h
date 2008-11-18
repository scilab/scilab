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
#ifndef __REALMAIN_H__
#define __REALMAIN_H__
/*--------------------------------------------------------------------------*/
#include <signal.h>
#include <stdio.h>
#include <string.h>
#ifdef _MSC_VER
#include <Windows.h>
#endif

#include "version.h"

/** 
 * When Scilab is started with arguments ... defines if it is a scilab
 * script or a scilab code
 */
typedef enum {
	SCILAB_SCRIPT=0,  /* SCILAB_SCRIPT means filename as argument */
	SCILAB_CODE=1 /*  SCILAB_CODE means a scilab code */
} InitScriptType;


/**
* The real main of Scilab
*
* @param no_startup_flag_l  
* @param initial_script 
* @param initial_script_type    
* @param memory 
* @return 0 is everything went fine. not 0 is everything went wrong. (@TODO: should be changed... returning only 0 here)
*/
int realmain(int no_startup_flag_l,char *initial_script,InitScriptType initial_script_type,int memory);

/**
 * @TODO add comment
 *
 * @param void  
 * @return <ReturnValue>
 */
int Get_no_startup_flag(void);

/**
 * @TODO add comment
 *
 * @param start 
 */
void Set_no_startup_flag(int start);

#ifndef _MSC_VER
char ** create_argv(int *argc);
#endif

#endif /* __REALMAIN_H__ */
/*--------------------------------------------------------------------------*/


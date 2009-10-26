/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2005 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _Included_javasci_globals
#define _Included_javasci_globals

#include <stdio.h> 
#include <string.h>
#include "jni.h"
#include "stack-c.h"
#include "tmpdir.h"
#include "scirun.h"
#include "sciquit.h"
#include "call_scilab.h"
#include "fromjava.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
#ifndef SCI 
  #define SCI ".."
#endif 
/*--------------------------------------------------------------------------*/
#define MAX_STR 512
/*--------------------------------------------------------------------------*/
/**
 * Enable the Interface
 */
void EnableInterf(void);

/**
 * Disable the Interface
 */
void DisableInterf(void);

/**
 * Return the state of the Interface
 * @return int the state (1 or 0)
 */
int GetInterfState(void);

/**
 * Send a Scilab Job
 * @param the Job
 * @return the result of the process
 */
int send_scilab_job(char *job) ;

/**
 * Initialisation of Scilab
 */
void Initialize(void) ;
/*--------------------------------------------------------------------------*/
#endif
/*--------------------------------------------------------------------------*/

/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) DIGITEO - 2009 - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/ 
#ifndef __MODE_EXEC_H__
#define __MODE_EXEC_H__

#include "BOOL.h"

/**
* by default at Prompt mode == 2
*/


typedef enum { 
	INITIALIZATION_EXEC_MODE = 2,
	SILENT_EXEC_MODE = -1,
	ECHO_EXEC_MODE = 1
} execMode;


/**
* set a mode in exec file
* @param[in] mode
* @return TRUE if mode is modified
*/
BOOL setExecMode(execMode _mode);

/**
* get mode in exec file
* @return current mode
*/
execMode getExecMode(void);

#endif /* __MODE_EXEC_H__ */
/*--------------------------------------------------------------------------*/ 

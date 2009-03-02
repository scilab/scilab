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
#define INITIALIZATION_EXEC_MODE 2
#define SILENT_EXEC_MODE -1
#define ECHO_EXEC_MODE 1
/**
* set a mode in exec file
* @param[in] mode
* @return TRUE if mode is modified
*/
BOOL setExecMode(int _mode);

/**
* get mode in exec file
* @return current mode
*/
int getExecMode(void);

#endif /* __MODE_EXEC_H__ */
/*--------------------------------------------------------------------------*/ 

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
* set a mode in exec file
* @param[in] mode
* @return TRUE if mode is modified
*/
BOOL set_mode_exec(int mode);

/**
* get mode in exec file
* @return current mode
*/
int get_mode_exec(void);

#endif /* __MODE_EXEC_H__ */
/*--------------------------------------------------------------------------*/ 

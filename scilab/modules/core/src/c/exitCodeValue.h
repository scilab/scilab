/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2010 - DIGITEO - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#ifndef __EXITCODEVALUE_H__
#define __EXITCODEVALUE_H__

/**
* set exit code returned by scilab at exit
* @param[in] exit code
*/
void setExitCodeValue(int _exitcode);

/**
* get exit code returned by scilab
* @return int exit code
*/
int getExitCodeValue(void);

#endif /* __EXITCODEVALUE_H__ */
/*--------------------------------------------------------------------------*/

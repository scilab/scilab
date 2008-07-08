/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#ifndef __PARTFUNCTION_H__
#define __PARTFUNCTION_H__


/**
* part : extraction of strings
* @param[in] stringInput : matrix of strings
* @param[in] m : col of stringInput
* @param[in] n : row of stringInput
* @param[in] vectInput : row vector
* @param[in] row : row of vectInput
* @return matrix of strings (same size as stringInput)
*/
char **partfunction(char** stringInput,int m,int n,int *vectInput,int row);

#endif /* __PARTFUNCTION_H__ */
/*---------------------------------------------------------------------------*/ 

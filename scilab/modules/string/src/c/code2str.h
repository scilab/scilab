
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#ifndef __CODE2STR__
#define __CODE2STR__

/**
* code2str  returns character string associated with Scilab integer codes
* @param[out] Output_Matrix vector of character 
* @param[in] scilab codes of a string
* @param[in] dimension of Input_Matrix
*/
void code2str(char **Output_Matrix, int *Input_Matrix,int Dim_Input_Matrix);


/**
* str2code  return scilab integer codes associated with a character string 
* @param[out] Output_Matrix  vector of character integer codes 
* @param[in] Input_Matrix a character string 
*/
void str2code(int *Output_Matrix, char **Input_Matrix);

#endif /* __CODE2STR__ */
/*--------------------------------------------------------------------------*/

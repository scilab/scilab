/*------------------------------------------------------------------------*/
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Cong WU
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
#ifndef __STRSPLIT_H__
#define __STRSPLIT_H__

/**
* string split split a string into a vector of strings 
* @param[in] Input_StringMatrix a string 
* @param[in] Input_IntMatrix a vector of stricly increasing indices in the interval  [1 length(str)-1]
* @param[out]Output_StringMatrix the resulting column vector of strings (dimension  size(ind,'*')+1  ).  
*@return
*/

void strsplit(char **Input_StringMatrix,char **Output_StringMatrix,int *Row_Pointer,int *Row_Pointer_Two,int *Col_Pointer,int *Input_IntMatrix);

#endif /* __STRSPLIT_H__ */
/*------------------------------------------------------------------------*/

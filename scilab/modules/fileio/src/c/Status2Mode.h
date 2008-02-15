/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/ 
#ifndef __STATUS2MODE_H__
#define __STATUS2MODE_H__

/** 
* convert status to an integer (open a file)
* @param status path example : "rt" "rb"
* @return a integer
*/
int Status2Mode(char *status);

#endif /* __STATUS2MODE_H__ */
/*--------------------------------------------------------------------------*/ 

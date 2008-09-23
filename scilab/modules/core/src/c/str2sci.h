/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Sylvestre LEDRU
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#ifndef __STR2SCI_H__
#define __STR2SCI_H__
/** C string  var2vec  to scilab
 *  @param x is supposed to be a fortran image of var2vec result
 */
void str2sci(char** x,int n,int m);

#endif /* __STR2SCI_H__ */

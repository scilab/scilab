/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Sylvestre Ledru
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#ifndef _LOADTEXTRENDERINGAPI_H_
#define _LOADTEXTRENDERINGAPI_H_


/**
 * Taken a Matrix of string, we check if any char is starting by 
 * $ or < which are the two tags for, respectively, latex & MathML
 *
 * @param text  the matrix of string
 * @param nbRow the number of row
 * @param nbCol  the number of col
 */
void loadTextRenderingAPI(char **text, int nbRow, int nbCol);

#endif /* _LOADTEXTRENDERINGAPI_H_ */

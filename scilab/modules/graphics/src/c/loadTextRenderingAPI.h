/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Sylvestre Ledru
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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
void loadTextRenderingAPI(char const* const* text, int nbRow, int nbCol);

#endif /* _LOADTEXTRENDERINGAPI_H_ */

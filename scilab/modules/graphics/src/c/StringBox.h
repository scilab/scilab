/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: StringBox.h                                                      */
/* desc : Contains a set of functions to compute the bounding box of a    */
/*        text                                                            */
/*------------------------------------------------------------------------*/

#ifndef _STRING_BOX_H_
#define _STRING_BOX_H_

#include "ObjectStructure.h"

/*----------------------------------------------------------------------------------*/
/**
 * Compute the position of the 4 corners of a text in 2d view coordinates
 * @param text matrix of text of size nbRow x nbCol.
 * @param xPos X coordinate of the string
 * @param yPos Y coordinate of the string
 * @param angle font angle in degree, clockwise
 * @param corners result containing the postions of the 4 corners of the text box
 */
void getTextBoundingBox(char ** text, int nbRow, int nbCol,
                        double xPos, double yPos,
                        double angle, int fontId, double fontSize,
                        double corners[4][2]);
/*----------------------------------------------------------------------------------*/
#endif /* _STRING_BOX_H_ */

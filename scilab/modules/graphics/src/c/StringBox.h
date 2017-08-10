/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
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

/*------------------------------------------------------------------------*/
/* file: StringBox.h                                                      */
/* desc : Contains a set of functions to compute the bounding box of a    */
/*        text                                                            */
/*------------------------------------------------------------------------*/

#ifndef _STRING_BOX_H_
#define _STRING_BOX_H_

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

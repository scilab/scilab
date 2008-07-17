#ifndef __CALLCOLORCHOOSER_H__
#define __CALLCOLORCHOOSER_H__

/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/**
 * Create a new Color Chooser
 *
 * @param void  
 * @return this ID of the Java Color Chooser
 */
int createColorChooser(void);

/**
 * Set the title of a ColorChooser
 *
 * @param colorChooserID the ID of the Java Color Chooser
 * @param title the title
 */
void setColorChooserTitle(int colorChooserID, char *title);

/**
 * Set the default color for a Color Chooser
 *
 * @param colorChooserID the ID of the Java Color Chooser
 * @param RBG a 3-values vector containing red, green, blue value
 */
void setColorChooserDefaultRGB(int colorChooserID, double *RGB);

/**
 * Set the default color for a Color Chooser
 *
 * @param colorChooserID the ID of the Java Color Chooser
 * @param R red value
 * @param G red value
 * @param B red value
 */
void setColorChooserDefaultRGBSeparateValues(int colorChooserID, int R, int G, int B);

/**
 * Get the selected color for a Color Chooser
 *
 * @param colorChooserID the ID of the Java Color Chooser
 * @return a 3-values vector containing red, green, blue value
 */
double *getColorChooserSelectedRGB(int colorChooserID);

/**
 * Display the ColorChooser and wait for a user input
 *
 * @param colorChooserID the ID of the Java ColorChooser
 */
void colorChooserDisplayAndWait(int colorChooserID);


#endif /* !__CALLCOLORCHOOSER_H__ */

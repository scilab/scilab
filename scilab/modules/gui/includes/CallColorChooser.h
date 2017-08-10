#ifndef __CALLCOLORCHOOSER_H__
#define __CALLCOLORCHOOSER_H__

/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
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

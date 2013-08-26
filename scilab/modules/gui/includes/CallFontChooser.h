#ifndef __CALLFONTCHOOSER_H__
#define __CALLFONTCHOOSER_H__

/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "BOOL.h"

/**
 * Create a new Font Chooser
 *
 * @param void
 * @return this ID of the Java Font Chooser
 */
int createFontChooser(void);

/**
 * Set the default font name of a Font Chooser
 *
 * @param fontChooserID the ID of the Java FontChooser
 * @param fontName the name of the default font
 */
void setFontChooserFontName(int fontChooserID, char *fontName);

/**
 * Set the default font size of a Font Chooser
 *
 * @param fontChooserID the ID of the Java FontChooser
 * @param fontSize the size of the default font
 */
void setFontChooserFontSize(int fontChooserID, int fontSize);

/**
 * Set the default bold attribute of a Font Chooser
 *
 * @param fontChooserID the ID of the Java FontChooser
 * @param bold the bold attribute of the default font
 */
void setFontChooserBold(int fontChooserID, BOOL bold);

/**
 * Set the default italic attribute of a Font Chooser
 *
 * @param fontChooserID the ID of the Java FontChooser
 * @param italic the italic attribute of the default font
 */
void setFontChooserItalic(int fontChooserID, BOOL italic);

/**
 * Display the FontChooser and wait for a user input
 *
 * @param fontChooserID the ID of the Java FontChooser
 */
void fontChooserDisplayAndWait(int fontChooserID);

/**
 * Get the selected font name of a Font Chooser
 *
 * @param fontChooserID the ID of the Java FontChooser
 * @return the name of the font
 */
char *getFontChooserFontName(int fontChooserID);

/**
 * Get the selected font size of a Font Chooser
 *
 * @param fontChooserID the ID of the Java FontChooser
 * @return the size of the font
 */
int getFontChooserFontSize(int fontChooserID);

/**
 * Get the selected font bold attribute of a Font Chooser
 *
 * @param fontChooserID the ID of the Java FontChooser
 * @return the bold attribute of the font
 */
BOOL getFontChooserBold(int fontChooserID);

/**
 * Get the selected font italic attribute of a Font Chooser
 *
 * @param fontChooserID the ID of the Java FontChooser
 * @return the italic attribute of the font
 */
BOOL getFontChooserItalic(int fontChooserID);

#endif

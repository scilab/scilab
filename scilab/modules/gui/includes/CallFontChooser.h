#ifndef __CALLFONTCHOOSER_H__
#define __CALLFONTCHOOSER_H__

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

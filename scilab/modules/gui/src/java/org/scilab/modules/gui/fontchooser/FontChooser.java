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

package org.scilab.modules.gui.fontchooser;

import java.awt.Font;

import org.scilab.modules.gui.uielement.UIElement;

/**
 * Interface for font chooser for Scilab
 * @author Vincent COUVERT
 */
public interface FontChooser extends UIElement {

    /**
     * Gets this Bridge component object
     * @return this Bridge component object
     */
    SimpleFontChooser getAsSimpleFontChooser();

    /**
     * Retrieve the selected font, or null
     * @return the selected font
     */
    Font getSelectedFont();

    /**
     * Set the default font name
     * @param fontName the default font name
     */
    void setDefaultFontName(String fontName);

    /**
     * Set the default font size
     * @param fontSize the default font size
     */
    void setDefaultFontSize(int fontSize);

    /**
     * Set the default font bold attribute
     * @param bold the default font bold attribute
     */
    void setDefaultBold(boolean bold);

    /**
     * Set the default font italic attribute
     * @param italic the default font italic attribute
     */
    void setDefaultItalic(boolean italic);

    /**
     * Display the font chooser and wait for a user input
     */
    void displayAndWait();
}

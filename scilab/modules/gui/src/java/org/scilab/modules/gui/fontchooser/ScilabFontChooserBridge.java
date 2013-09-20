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

import org.scilab.modules.gui.bridge.fontchooser.SwingScilabFontChooser;

/**
 * Bridge dedicated to Scilab font chooser GUIs
 * @author Vincent COUVERT
 */
public class ScilabFontChooserBridge {

    /**
     * Constructor
     */
    protected ScilabFontChooserBridge() {
        throw new UnsupportedOperationException(); /* Prevents calls from subclass */
    }

    /**
     * Creates a new FontChooser
     * @param font the default font
     * @return the created FontChooser
     */
    public static SimpleFontChooser createFontChooser(Font font) {
        return new SwingScilabFontChooser(font);
    }

    /**
     * Retrieve the selected font, or null
     * @param fontChooser the FontChooser
     * @return the selected font
     */
    public static Font getSelectedFont(FontChooser fontChooser) {
        return fontChooser.getAsSimpleFontChooser().getSelectedFont();
    }

    /**
     * Set the default font name
     * @param fontChooser the FontChooser
     * @param fontName the default font name
     */
    public static void setDefaultFontName(FontChooser fontChooser, String fontName) {
        fontChooser.getAsSimpleFontChooser().setDefaultFontName(fontName);
    }

    /**
     * Set the default font size
     * @param fontChooser the FontChooser
     * @param fontSize the default font size
     */
    public static void setDefaultFontSize(FontChooser fontChooser, int fontSize) {
        fontChooser.getAsSimpleFontChooser().setDefaultFontSize(fontSize);
    }

    /**
     * Set the default font bold attribute
     * @param fontChooser the FontChooser
     * @param bold the default font bold attribute
     */
    public static void setDefaultBold(FontChooser fontChooser, boolean bold) {
        fontChooser.getAsSimpleFontChooser().setDefaultBold(bold);
    }

    /**
     * Set the default font italic attribute
     * @param fontChooser the FontChooser
     * @param italic the default font italic attribute
     */
    public static void setDefaultItalic(FontChooser fontChooser, boolean italic) {
        fontChooser.getAsSimpleFontChooser().setDefaultItalic(italic);
    }

    /**
     * Display the font chooser and wait for a user input
     * @param fontChooser the FontChooser
     */
    public static void displayAndWait(FontChooser fontChooser) {
        fontChooser.getAsSimpleFontChooser().displayAndWait();
    }

}

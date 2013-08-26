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

package org.scilab.modules.gui.colorchooser;

import java.awt.Color;

import org.scilab.modules.gui.bridge.colorchooser.SwingScilabColorChooser;

/**
 * Bridge dedicated to Scilab ColorChooser GUIs
 * @author Vincent COUVERT
 */
public class ScilabColorChooserBridge {

    /**
     * Constructor
     */
    protected ScilabColorChooserBridge() {
        throw new UnsupportedOperationException(); /* Prevents calls from subclass */
    }

    /**
     * Creates a new ColorChooser
     * @param color the default Color
     * @return the created ColorChooser
     */
    public static SimpleColorChooser createColorChooser(Color color) {
        return new SwingScilabColorChooser(color);
    }

    /**
     * Retrieve the selected Color, or null
     * @param colorChooser the ColorChooser
     * @return the selected Color
     */
    public static Color getSelectedColor(ColorChooser colorChooser) {
        return colorChooser.getAsSimpleColorChooser().getSelectedColor();
    }

    /**
     * Set the title of the ColorChooser
     * @param colorChooser the ColorChooser
     * @param title the title to set
     */
    public static void setTitle(ColorChooser colorChooser, String title) {
        colorChooser.getAsSimpleColorChooser().setTitle(title);
    }

    /**
     * Set the default color
     * @param colorChooser the ColorChooser
     * @param color the default color
     */
    public static void setDefaultColor(ColorChooser colorChooser, Color color) {
        colorChooser.getAsSimpleColorChooser().setDefaultColor(color);
    }

    /**
     * Display the font chooser and wait for a user input
     * @param colorChooser the ColorChooser
     */
    public static void displayAndWait(ColorChooser colorChooser) {
        colorChooser.getAsSimpleColorChooser().displayAndWait();
    }
}

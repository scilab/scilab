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

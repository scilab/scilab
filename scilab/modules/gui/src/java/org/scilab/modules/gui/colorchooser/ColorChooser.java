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

import org.scilab.modules.gui.uielement.UIElement;

/**
 * Interface for color chooser for Scilab
 * @author Vincent COUVERT
 */
public interface ColorChooser extends UIElement {

    /**
     * Gets this Bridge component object
     * @return this Bridge component object
     */
    SimpleColorChooser getAsSimpleColorChooser();

    /**
     * Set the title of the ColorChooser
     * @param title the title to set
     */
    void setTitle(String title);

    /**
     * Retrieve the selected color, or null
     * @return the selected color
     */
    Color getSelectedColor();

    /**
     * Set the default color
     * @param color the default color
     */
    void setDefaultColor(Color color);

    /**
     * Display the font chooser and wait for a user input
     */
    void displayAndWait();
}

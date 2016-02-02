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

/**
 * Interface for SimpleColorChooser (the object associated to Scilab ColorChooser)
 * @author Vincent COUVERT
 */
public interface SimpleColorChooser {


    /**
     * Set the element id for this ColorChooser
     * @param id the id of the corresponding ColorChooser object
     */
    void setElementId(int id);

    /**
     * Get the element id for this chooser
     * @return id the id of the corresponding chooser object
     */
    int getElementId();

    /**
     * Set the title of the ColorChooser
     * @param title the title to set
     */
    void setTitle(String title);

    /**
     * Retrieve the selected Color, or null
     * @return the selected Color
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

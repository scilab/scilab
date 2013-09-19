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

/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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

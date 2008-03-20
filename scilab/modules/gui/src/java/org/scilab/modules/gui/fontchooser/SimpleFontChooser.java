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

package org.scilab.modules.gui.fontchooser;

import java.awt.Font;

/**
 * Interface for SimpleFontChooser (the object associated to Scilab FontChooser)
 * @author Vincent COUVERT
 */
public interface SimpleFontChooser {
	

	/**
	 * Set the element id for this FontChooser
	 * @param id the id of the corresponding FontChooser object
	 */
	void setElementId(int id);
	
	/**
	 * Get the element id for this chooser
	 * @return id the id of the corresponding chooser object
	 */
	int getElementId();
	
	/** 
	 * Retrieve the selected font, or null 
	 * @return the selected font
	 */
	Font getSelectedFont();

}

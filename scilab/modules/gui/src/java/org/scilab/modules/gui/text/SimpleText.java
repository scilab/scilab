/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Marouane BEN JELLOUL
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.text;

import org.scilab.modules.gui.widget.Widget;

/**
 * Interface for TextBridge the associated object to Scilab GUIs Text
 * @author Marouane BEN JELLOUL
 */
public interface SimpleText extends Widget {

	/**
	 * Sets the text of a Text Widget
	 * @param newText the text to set to the Text Widget
	 */
	void setText(String newText);

	/**
	 * Gets the text of a Text Widget
	 * @return the text of the Text Widget
	 */
	String getText();
}

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

package org.scilab.modules.gui.layout;

import org.scilab.modules.gui.bridge.ScilabBridge;

/**
 * Class for Scilab Menus in GUIs
 * @author Marouane BEN JELLOUL
 */
public class ScilabBorderLayout implements BorderLayout {
	
	/**
	 * Constructor
	 */
	protected ScilabBorderLayout() {
        throw new UnsupportedOperationException(); /* Prevents calls from subclass */
	}
	
	/**
	 * Creates a Scilab BorderLayout object
	 * @return the created BorderLayout
	 */
	public static BorderLayout createBorderLayout() {
		return ScilabBridge.createBorderLayout();
	}
}

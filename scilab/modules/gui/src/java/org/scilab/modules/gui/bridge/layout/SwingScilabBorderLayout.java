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

package org.scilab.modules.gui.bridge.layout;

import org.scilab.modules.gui.layout.BorderLayout;

/**
 * Swing implementation for Scilab BorderLayout in GUIs
 * @author Marouane BEN JELLOUL
 */
public class SwingScilabBorderLayout extends java.awt.BorderLayout implements BorderLayout {
	
	private static final long serialVersionUID = 222353824141297435L;

	/**
	 * Constructor
	 */
	public SwingScilabBorderLayout() {
		super();
	}
}

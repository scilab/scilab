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

import org.scilab.modules.gui.layout.FlowLayout;

/**
 * Swing implementation for Scilab FlowLayout in GUIs
 * @author Marouane BEN JELLOUL
 */
public class SwingScilabFlowLayout extends java.awt.FlowLayout implements FlowLayout {

	private static final long serialVersionUID = 1947906560190763378L;

	/**
	 * Constructor
	 */
	public SwingScilabFlowLayout() {
		super();
	}
}

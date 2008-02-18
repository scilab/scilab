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

package org.scilab.modules.gui.separator;

import org.scilab.modules.gui.bridge.separator.SwingScilabSeparator;

/**
 * Bridge for Scilab Separators in GUIs
 * @author Marouane BEN JELLOUL
 */
public class ScilabSeparatorBridge {

	/**
	 * Constructor
	 */
	protected ScilabSeparatorBridge() {
		throw new UnsupportedOperationException(); /* Prevents calls from subclass */
	}

	/**
	 * Creates a new Scilab Separator
	 * @return the created Separator
	 */
	public static Separator createSeparator() {
		return new SwingScilabSeparator();
	}

}

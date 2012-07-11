/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Han DONG
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.gui.uidisplaytree;

import org.scilab.modules.gui.text.SimpleText;

/**
 * Interface for ScilabUiDisplayTreeBridge the associated object to Scilab GUIs UiTable 
 * @author Han DONG
 */
public interface SimpleUiDisplayTree extends SimpleText {

	/**
	 * Sets the tree data to be added to viewport
	 * @param text the array of strings containing tree data
	 */
	void setData(String[] text);
}

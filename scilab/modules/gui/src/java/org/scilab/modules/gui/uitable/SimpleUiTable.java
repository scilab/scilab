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
package org.scilab.modules.gui.uitable;

import org.scilab.modules.gui.text.SimpleText;

/**
 * Interface for ScilabUiTableBridge the associated object to Scilab GUIs UiTable 
 * @author Han DONG
 */
public interface SimpleUiTable extends SimpleText {

	/**
	 * Sets the column name for uitable
	 */
	void setColnames(String text);

	/**
	 * Sets the row name for uitable
	 */
	void setRownames(String text);

	/**
	 * Sets the data for uitable
	 */
	void setData(String text);
}

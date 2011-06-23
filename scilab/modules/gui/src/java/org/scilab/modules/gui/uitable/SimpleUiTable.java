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
	 * Sets the column names for uitable
	 * @param names the column names
	 */
	void setColumnNames(String[] names);

	/**
	 * Sets the row name for uitable
	 * @param names the row names
	 */
	void setRowNames(String[] names);

	/**
	 * Sets the data for uitable
	 * @param text the data as string
	 */
	void setData(String[] text);
}

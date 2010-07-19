/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Allan SIMON
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


package org.scilab.modules.ui_data.variableeditor;

import org.scilab.modules.gui.tab.Tab;


/**
 * Interface for Variable editor.
 */

public interface SimpleVariableEditor extends Tab {

	/**
	 * Set data to be displayed
	 * @param data : the data displayed in JTable
	 */
	void setData(Object[][] data);

	/**
	 * Sets the value in the cell at row and col to value. 
	 * @param value : the new value
	 * @param row : the row whose value is to be changed
	 * @param col : the column whose value is to be changed
	 */
	void setValueAt(Object value, int row, int col);

}

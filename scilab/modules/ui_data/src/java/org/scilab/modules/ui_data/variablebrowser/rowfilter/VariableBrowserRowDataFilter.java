/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Sylvestre LEDRU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.ui_data.variablebrowser.rowfilter;

import java.util.HashSet;

import javax.swing.RowFilter;

import org.scilab.modules.ui_data.BrowseVar;

public class VariableBrowserRowDataFilter extends RowFilter<Object, Object>{

//	private HashSet<Boolean> filteredValues = new HashSet<Boolean>();
    private Boolean filteredValues;

	public VariableBrowserRowDataFilter() {
		super();
	}

	public VariableBrowserRowDataFilter(Boolean filteredValues) {
		this.filteredValues = filteredValues;
	}


	@Override
	public boolean include(Entry< ? extends Object, ? extends Object> entry) {
		boolean currentRowVariableclassNumber = ((Boolean) entry.getValue(BrowseVar.FROM_SCILAB_COLUMN_INDEX)).booleanValue();
        if (filteredValues != currentRowVariableclassNumber && currentRowVariableclassNumber!=true) {
                return false;
            }else{
                return true;
            }
    }

}

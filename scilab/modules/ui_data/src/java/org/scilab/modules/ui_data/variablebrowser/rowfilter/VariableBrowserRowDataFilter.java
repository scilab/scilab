/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Sylvestre LEDRU
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

package org.scilab.modules.ui_data.variablebrowser.rowfilter;

import javax.swing.RowFilter;

import org.scilab.modules.ui_data.BrowseVar;

public class VariableBrowserRowDataFilter extends RowFilter<Object, Object> {

    //	private HashSet<Boolean> filteredValues = new HashSet<Boolean>();
    private Boolean filteredValues;

    public VariableBrowserRowDataFilter() {
        super();
    }

    public VariableBrowserRowDataFilter(Boolean filteredValues) {
        this.filteredValues = filteredValues;
    }


    @Override
    public boolean include(Entry < ? extends Object, ? extends Object > entry) {
        boolean currentRowVariableclassNumber = ((Boolean) entry.getValue(BrowseVar.FROM_SCILAB_COLUMN_INDEX)).booleanValue();
        if (filteredValues != currentRowVariableclassNumber && currentRowVariableclassNumber != true) {
            return false;
        } else {
            return true;
        }
    }

}

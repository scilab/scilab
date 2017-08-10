/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Allan SIMON
 * Copyright (C) 2010 - DIGITEO - Vincent COUVERT
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

import java.util.HashSet;

import javax.swing.RowFilter;

import org.scilab.modules.ui_data.BrowseVar;
import org.scilab.modules.types.ScilabTypeEnum;

public class VariableBrowserRowTypeFilter extends RowFilter<Object, Object> {

    private HashSet<ScilabTypeEnum> filteredValues;

    public VariableBrowserRowTypeFilter() {
        super();
        filteredValues = new HashSet<ScilabTypeEnum>();
    }

    public VariableBrowserRowTypeFilter(HashSet<ScilabTypeEnum> filteredValues) {
        this.filteredValues = filteredValues;
    }

    @Override
    public boolean include(Entry < ? extends Object, ? extends Object > entry) {
        Integer currentRowVariableTypeNumber =  (Integer) entry.getValue(BrowseVar.TYPE_COLUMN_INDEX);

        try {
            if (filteredValues.contains(ScilabTypeEnum.swigToEnum(currentRowVariableTypeNumber))) {
                return false;
            }
        } catch (IllegalArgumentException e) {
            // This case should never occur
            // But there is a bug in Scilab global variables type detection
            // So we show these "unknown type" variables by default (See bug #7333)
            return true;
        }

        return true;
    }

}

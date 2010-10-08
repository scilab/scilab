/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Allan SIMON
 * Copyright (C) 2010 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.ui_data.variableeditor.celleditor;

/**
 * CellEditor for boolean type
 * @author Allan SIMON
 * @author Bruno JOFRET
 */
public class ScilabBooleanCellEditor extends ScilabGenericCellEditor {

    private static final long serialVersionUID = 3866054279955696250L;

    /**
     * Constructor
     */
    public ScilabBooleanCellEditor() {
        super();
    }

    /**
     * {@inheritDoc}
     */
    public String getDataAsScilabString(Object value) {
        String newValue = "";
        if (value != null && value instanceof Boolean) {
            if (((Boolean) value).booleanValue()) {
                newValue = "%t";
            } else {
                newValue = "%f";
            }
        } else if (value instanceof String) {
            newValue = (String) value;
        }

        return newValue;
    }
}

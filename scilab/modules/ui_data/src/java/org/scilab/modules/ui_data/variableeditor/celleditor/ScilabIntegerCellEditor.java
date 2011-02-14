/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
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
 * CellEditor for integer type
 * @author Calixte DENIZET
 */
public class ScilabIntegerCellEditor extends ScilabGenericCellEditor {

    private static final long serialVersionUID = 1L;

    /**
     * Constructor
     */
    public ScilabIntegerCellEditor() {
        super();
    }

    /**
     * {@inheritDoc}
     */
    public String getDataAsScilabString(Object value) {
        String newValue = "";
        if (value != null) {
            newValue = value.toString();
        } else if (value instanceof String) {
            newValue = (String) value;
        }

        return newValue;
    }
}

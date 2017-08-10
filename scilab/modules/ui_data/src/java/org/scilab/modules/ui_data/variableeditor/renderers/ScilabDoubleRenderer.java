/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Allan SIMON
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

package org.scilab.modules.ui_data.variableeditor.renderers;

/**
 * CellRenderer for double type
 * @author Allan SIMON
 */
@SuppressWarnings(value = { "serial" })
public class ScilabDoubleRenderer extends ScilabComplexRenderer {

    /**
     * Constructor
     */
    public ScilabDoubleRenderer() {
        super();
    }

    /**
     * {@inheritDoc}
     */
    protected void setValue(Object value) {
        if (value == null) {
            super.setValue("");
        } else if (value instanceof String) {
            super.setValue(value);
        } else {
            super.setValue(convertDouble((Double) value));
        }
    }
}

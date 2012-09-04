/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.ui_data.variableeditor.renderers;

import java.awt.Color;
import java.awt.Component;

import javax.swing.JTable;

/**
 * CellRenderer for sparse type
 * @author Calixte DENIZET
 */
@SuppressWarnings(value = { "serial" })
public class ScilabSparseRenderer extends ScilabComplexSparseRenderer {

    /**
     * Constructor
     */
    public ScilabSparseRenderer() {
        super();
    }

    /**
     * {@inheritDoc}
     */
    protected void setValue(Object value) {
        if (isNothing(value)) {
            super.setValue("");
        } else if (value instanceof String) {
            super.setValue(value);
        } else {
            super.setValue(convertDouble((Double) value));
        }
    }
}

/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Allan SIMON
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.ui_data.variableeditor.renderers;

import java.text.NumberFormat;

import javax.swing.table.DefaultTableCellRenderer;

/**
 * CellRenderer for complex type
 * @author Allan SIMON
 * @author Calixte DENIZET
 */
public class ScilabComplexRenderer extends DefaultTableCellRenderer {

    private static final NumberFormat NUMBERFORMAT = NumberFormat.getInstance();
    private static final String I = "i";
    private static final String PLUS = "+";

    /**
     * Constructor
     */
    public ScilabComplexRenderer() {
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
            super.setValue(convertComplex((Double[]) value));
        }
    }

    /**
     * @param newValue the number of digits after the dot.
     */
    public void setMaximumFractionDigits(int newValue) {
        NUMBERFORMAT.setMaximumFractionDigits(newValue);
    }

    /**
     * Convert a finite double, %nan or +/-%inf
     * @param d the double to convert
     * @return the double into String
     */
    protected static String convertDouble(Double d) {
        if (d.isNaN()) {
            return "Nan";
        } else if (d.isInfinite()) {
            if (d.doubleValue() < 0) {
                return "-Inf";
            } else {
                return "Inf";
            }
        } else {
            return NUMBERFORMAT.format(d.doubleValue());
        }
    }

    /**
     * @param comp a complex
     * @return the string representation of this complex.
     */
    private static String convertComplex(Double[] comp) {
        String re = "";
        String im = "";
        if (comp[0] == 0 && comp[1] == 0) {
            re = "0";
        } else {
            if (comp[1] != 0) {
                if (comp[1] == 1) {
                    im = "+i";
                } else if (comp[1] == -1) {
                    im = "-i";
                } else if (comp[1] < 0) {
                    im = convertDouble(comp[1]) + I;
                } else {
                    im = PLUS + convertDouble(comp[1]) + I;
                }
            }
            if (comp[0] != 0) {
                re = convertDouble(comp[0]);
            } else if (im.startsWith(PLUS)) {
                im = im.substring(1);
            }
        }

        return re + im;
    }
}

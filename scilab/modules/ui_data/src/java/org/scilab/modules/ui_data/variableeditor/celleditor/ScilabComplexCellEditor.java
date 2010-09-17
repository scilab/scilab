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
package org.scilab.modules.ui_data.variableeditor.celleditor;

/**
 * CellEditor for complex type
 * @author Allan SIMON
 * @author Calixte DENIZET
 */
public class ScilabComplexCellEditor extends ScilabGenericCellEditor {

    private static final long serialVersionUID = 3866044279955696250L;
    private static final String I = "*%i";
    private static final String PLUS = "+";

    /**
     * Constructor
     */
    public ScilabComplexCellEditor() {
        super();
    }

    /**
     * {@inheritDoc}
     */
    public String getDataAsScilabString(Object value) {
        String newValue = "";
        if (value != null && value instanceof Double[]) {
            newValue = convertComplex((Double[]) value);
        } else if (value instanceof String) {
            newValue = (String) value;
        }

        return newValue;
    }

    /**
     * Convert a finite double, %nan or +/-%inf
     * @param d the double to convert
     * @return the double in String format
     */
    protected static String convertDouble(Double d) {
        if (d.isNaN()) {
            return "%nan";
        } else if (d.isInfinite()) {
            if (d.doubleValue() < 0) {
                return "-%inf";
            } else {
                return "%inf";
            }
        } else {
            return d.toString();
        }
    }

    /**
     * Convert a complex into a Scilab expression
     * @param comp the complex
     * @return the string expression
     */
    private static String convertComplex(Double[] comp) {
        String re = "";
        String im = "";
        if (comp[0] == 0 && comp[1] == 0) {
            re = "0";
        } else {
            if (comp[1] != 0) {
                if (comp[1] == 1) {
                    im = "+%i";
                } else if (comp[1] == -1) {
                    im = "-%i";
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

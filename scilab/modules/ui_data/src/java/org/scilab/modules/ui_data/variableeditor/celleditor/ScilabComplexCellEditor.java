/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Allan SIMON
 * Copyright (C) 2010 - Calixte DENIZET
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

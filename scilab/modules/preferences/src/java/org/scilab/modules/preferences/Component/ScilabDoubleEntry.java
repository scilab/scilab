/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Clement DAVID
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
package org.scilab.modules.preferences.Component;

import java.math.BigDecimal;
import java.text.DecimalFormat;
import java.text.DecimalFormatSymbols;

import javax.swing.InputVerifier;
import javax.swing.JFormattedTextField;

import org.scilab.modules.preferences.XChooser;
import org.scilab.modules.preferences.XCommonManager;
import org.scilab.modules.preferences.XComponent;
import org.scilab.modules.preferences.XConfigManager;
import org.w3c.dom.Node;

/**
 * {@link Entry} component which only allow a Scilab single double data text.
 */
@SuppressWarnings(value = { "serial" })
public class ScilabDoubleEntry extends JFormattedTextField implements XComponent, XChooser {

    /*
     * Static things, modify the following lines in case of a format change
     */

    private static final DecimalFormatSymbols FORMAT_SYMBOL = DecimalFormatSymbols.getInstance();
    private static final DecimalFormat SCILAB_FORMAT = new DecimalFormat("0.0####E00", FORMAT_SYMBOL);
    private static final BigDecimal MAX_DOUBLE = BigDecimal.valueOf(Double.MAX_VALUE);

    /**
     * Validate the user entry and format it.
     *
     * Without formatting the entry, bug #7143 appears on jdk6.
     */
    private static final InputVerifier VALIDATE_POSITIVE_DOUBLE = new InputVerifier() {

        @Override
        public boolean verify(javax.swing.JComponent arg0) {
            boolean ret = false;
            JFormattedTextField textField = (JFormattedTextField) arg0;
            try {
                BigDecimal value = new BigDecimal(textField.getText());
                if (value.compareTo(BigDecimal.ZERO) >= 0 && value.compareTo(MAX_DOUBLE) <= 0) {
                    ret = true;
                }
            } catch (NumberFormatException e) {
                return ret;
            }
            return ret;

        };
    };

    /**
     * Initialize static final fields
     */
    static {
        FORMAT_SYMBOL.setDecimalSeparator('.');
        SCILAB_FORMAT.setDecimalFormatSymbols(FORMAT_SYMBOL);
        SCILAB_FORMAT.setParseIntegerOnly(false);
        SCILAB_FORMAT.setParseBigDecimal(true);
    }

    /**
     * Constructor.
     *
     * @param peer
     *            : associated view DOM node.
     */
    public ScilabDoubleEntry(final Node peer) {
        super(SCILAB_FORMAT);
        setValue(new BigDecimal(0.0));
        setInputVerifier(VALIDATE_POSITIVE_DOUBLE);

        refresh(peer);
    }

    /*
     * Actuators
     */

    public final String value() {
        return ((BigDecimal) getValue()).toPlainString();
    }

    public final void value(final String text) {
        try {
            setValue(new BigDecimal(text));
        } catch (NumberFormatException e) {
            // do not update
        }
    }

    /*
     * XChooser implementation
     *
     * @see org.scilab.modules.preferences.XChooser
     */

    @Override
    public Object choose() {
        return SCILAB_FORMAT.format(getValue());
    }

    /*
     * XComponent implementation
     *
     * @see org.scilab.modules.preferences.XComponent
     */

    @Override
    public String[] actuators() {
        String[] actuators = { "enable", "value", "editable" };
        return actuators;
    }

    @Override
    public void refresh(Node peer) {
        final String enable = XConfigManager.getAttribute(peer, "enable", "true");
        setEnabled(enable.equals("true"));

        final String text = XCommonManager.getAttribute(peer, "value");
        try {
            final BigDecimal value = new BigDecimal(text);
            final BigDecimal previous = (BigDecimal) getValue();
            if (!value.equals(previous)) {
                setValue(value);
            }
        } catch (NumberFormatException e) {
            // do not update
        }

        final String editable = XConfigManager.getAttribute(peer, "editable", "true");
        setEditable(editable.equals("true"));
    }
}

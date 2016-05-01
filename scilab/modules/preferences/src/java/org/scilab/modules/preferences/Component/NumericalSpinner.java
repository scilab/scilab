/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
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

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JPanel;
import javax.swing.JSpinner;
import javax.swing.SpinnerNumberModel;
import javax.swing.SwingConstants;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

import org.w3c.dom.Node;

import org.scilab.modules.preferences.XChooser;
import org.scilab.modules.preferences.XCommonManager;
import org.scilab.modules.preferences.XComponent;
import org.scilab.modules.preferences.XConfigManager;

/**
 * Implementation of Label compliant with extended management.
 *
 * @author Calixte DENIZET
 *
 */
public class NumericalSpinner extends JSpinner implements XComponent, XChooser, ChangeListener {

    /** Universal identifier for serialization.
     *
     */
    private static final long serialVersionUID = -4842434795956015958L;
    private static final Double defaultInc = new Double(1);

    private Double min = new Double(Double.NEGATIVE_INFINITY);
    private Double max = new Double(Double.POSITIVE_INFINITY);
    private SpinnerNumberModel spinnerModel;
    private ActionListener actionListener;


    /** Define the set of actuators.
     *
     * @return array of actuator names.
     */
    public final String [] actuators() {
        return new String[] {"length", "increment", "min-value", "max-value", "value", "tooltip", "enable"};
    }

    /** Constructor.
     *
     * @param peer : associated view DOM node.
     */
    public NumericalSpinner(final Node peer) {
        super();
        spinnerModel = new SpinnerNumberModel(new Double(0), min, max, defaultInc);
        setModel(spinnerModel);
        addChangeListener(this);

        String length = XConfigManager.getAttribute(peer, "length");
        length(length);

        String increment = XConfigManager.getAttribute(peer, "increment");
        increment(increment);

        String tooltip = XConfigManager.getAttribute(peer, "tooltip");
        tooltip(tooltip);

        String minvalue = XConfigManager.getAttribute(peer, "min-value");
        minvalue(minvalue);

        String maxvalue = XConfigManager.getAttribute(peer, "max-value");
        maxvalue(maxvalue);

        String value = XConfigManager.getAttribute(peer, "value");
        value(value);

        String enable = XConfigManager.getAttribute(peer, "enable", "true");
        setEnabled(enable.equals("true"));
    }

    /** Refresh the component by the use of actuators.
     *
     * @param peer the corresponding view DOM node
     */
    public void refresh(final Node peer) {
        String length = XConfigManager.getAttribute(peer, "length");
        if (!length.equals(length())) {
            length(length);
        }

        String increment = XConfigManager.getAttribute(peer, "increment");
        if (!increment.equals(increment())) {
            increment(increment);
        }

        String tooltip = XConfigManager.getAttribute(peer, "tooltip");
        if (!tooltip.equals(tooltip())) {
            tooltip(tooltip);
        }

        String minvalue = XConfigManager.getAttribute(peer, "min-value");
        if (!minvalue.equals(minvalue())) {
            minvalue(minvalue);
        }

        String maxvalue = XConfigManager.getAttribute(peer, "max-value");
        if (!maxvalue.equals(maxvalue())) {
            maxvalue(maxvalue);
        }

        String value = XConfigManager.getAttribute(peer, "value");
        if (!value.equals(value())) {
            value(value);
        }

        String enable = XConfigManager.getAttribute(peer, "enable", "true");
        setEnabled(enable.equals("true"));

        setRequestFocusEnabled(true);
        setFocusable(true);
    }

    /** Sensor for 'length' attribute.
     *
     * @return the attribute value.
     */
    public final String length() {
        return Integer.toString(((JSpinner.DefaultEditor) getEditor()).getTextField().getColumns());
    }

    /** Sensor for 'value' attribute.
     *
     * @return the attribute value.
     */
    public final String value() {
        return spinnerModel.getValue().toString();
    }

    /** Sensor for 'min-value' attribute.
     *
     * @return the attribute value.
     */
    public final String minvalue() {
        return spinnerModel.getMinimum().toString();
    }

    /** Sensor for 'max-value' attribute.
     *
     * @return the attribute value.
     */
    public final String maxvalue() {
        return spinnerModel.getMaximum().toString();
    }

    /** Sensor for 'increment' attribute.
     *
     * @return the attribute value.
     */
    public final String increment() {
        return spinnerModel.getStepSize().toString();
    }

    /** Sensor for 'tooltip' attribute.
     *
     * @return the attribute value.
     */
    public final String tooltip() {
        String tooltip = getToolTipText();
        if (tooltip == null) {
            return "";
        }
        return tooltip;
    }

    /** Actuator for 'length' attribute.
     *
     * @param text : the attribute value.
     */
    public final void length(String length) {
        if (!length.equals(XCommonManager.NAV) && !length.equals("")) {
            try {
                ((JSpinner.DefaultEditor) getEditor()).getTextField().setColumns(Integer.parseInt(length));
            } catch (NumberFormatException e) { }
        }
    }

    /** Actuator for 'tooltip' attribute.
     *
     * @param text : the attribute value.
     */
    public final void tooltip(String tooltip) {
        if (tooltip.equals(XCommonManager.NAV) || tooltip.equals("")) {
            tooltip = null;
        }
        setToolTipText(tooltip);
    }

    /** Actuator for 'value' attribute.
     *
     * @return the attribute value.
     */
    public final void value(String value) {
        if (!value.equals(XCommonManager.NAV) && !value.equals("")) {
            spinnerModel.setValue(new Double(value));
        }
    }

    /** Actuator for 'min-value' attribute.
     *
     * @return the attribute value.
     */
    public final void minvalue(String minvalue) {
        if (!minvalue.equals(XCommonManager.NAV) && !minvalue.equals("")) {
            try {
                spinnerModel.setMinimum(new Double(minvalue));
            } catch (NumberFormatException e) { }
        }
    }

    /** Actuator for 'max-value' attribute.
     *
     * @return the attribute value.
     */
    public final void maxvalue(String maxvalue) {
        if (!maxvalue.equals(XCommonManager.NAV) && !maxvalue.equals("")) {
            try {
                spinnerModel.setMaximum(new Double(maxvalue));
            } catch (NumberFormatException e) { }
        }
    }

    /** Actuator for 'max-value' attribute.
     *
     * @return the attribute value.
     */
    public final void increment(String increment) {
        if (increment.equals(XCommonManager.NAV) || increment.equals("")) {
            spinnerModel.setStepSize(defaultInc);
        } else {
            try {
                spinnerModel.setStepSize(new Double(increment));
            } catch (NumberFormatException e) { }
        }
    }

    /** Actual response read by the listener.
     *
     * @return response read by the listener.
     */
    public final Object choose() {
        return value();
    }

    /**
     * {@inheritDoc}
     */
    public void stateChanged(ChangeEvent e) {
        if (actionListener != null) {
            actionListener.actionPerformed(new ActionEvent(this, 0, "Spinner Value changed", System.currentTimeMillis(), 0));
        }
    }

    public void addActionListener(ActionListener actionListener) {
        this.actionListener = actionListener;
    }

    /** Developer serialization method.
     *
     * @return equivalent signature.
     */
    public final String toString() {
        StringBuilder signature = new StringBuilder("NumericalSpinner");

        if (!length().equals(XConfigManager.NAV)) {
            signature.append(" length='");
            signature.append(length());
            signature.append("'");
        }

        if (!value().equals(XConfigManager.NAV)) {
            signature.append(" value='");
            signature.append(value());
            signature.append("'");
        }

        if (!minvalue().equals(XConfigManager.NAV)) {
            signature.append(" min-value='");
            signature.append(minvalue());
            signature.append("'");
        }

        if (!maxvalue().equals(XConfigManager.NAV)) {
            signature.append(" max-value='");
            signature.append(maxvalue());
            signature.append("'");
        }

        if (!increment().equals(XConfigManager.NAV)) {
            signature.append(" increment='");
            signature.append(increment());
            signature.append("'");
        }

        return signature.toString();
    }
}

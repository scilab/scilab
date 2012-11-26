/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.uiwidget.components;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import javax.swing.JSpinner;
import javax.swing.SpinnerNumberModel;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

import org.scilab.modules.uiwidget.UIComponent;
import org.scilab.modules.uiwidget.UIComponentAnnotation;
import org.scilab.modules.uiwidget.UIWidgetException;
import org.scilab.modules.uiwidget.UIWidgetTools;

public class UINumberSpinner extends UIComponent {

    private static final String defaultDoublePattern = ((JSpinner.NumberEditor) new JSpinner(new SpinnerNumberModel(0., 0., 0., 0.)).getEditor()).getFormat().toPattern();
    private static final String defaultIntPattern = ((JSpinner.NumberEditor) new JSpinner(new SpinnerNumberModel(0, 0, 0, 0)).getEditor()).getFormat().toPattern();

    private JSpinner spinner;
    private ChangeListener listener;
    private String action;

    public UINumberSpinner(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    public Object newInstance() {
        spinner = new JSpinner(new SpinnerNumberModel());

        return spinner;
    }

    @UIComponentAnnotation(attributes = {"type", "value", "min", "max", "step", "unit"})
    public Object newInstance(String type, String value, String min, String max, String step, String unit) {
        Number v, mi, ma, s;

        if (type != null && type.equalsIgnoreCase("double")) {
            if (min == null || min.isEmpty()) {
                mi = Double.MIN_VALUE;
            } else {
                try {
                    mi = Double.parseDouble(min);
                } catch (NumberFormatException e) {
                    mi = Double.MIN_VALUE;
                }
            }
            if (max == null || max.isEmpty()) {
                ma = Double.MAX_VALUE;
            } else {
                try {
                    ma = Double.parseDouble(max);
                } catch (NumberFormatException e) {
                    ma = Double.MAX_VALUE;
                }
            }
            if (value == null || value.isEmpty()) {
                v = (mi.doubleValue() + ma.doubleValue()) / 2;
            } else {
                try {
                    v = Double.parseDouble(value);
                } catch (NumberFormatException e) {
                    v = (mi.doubleValue() + ma.doubleValue()) / 2;
                }
            }
            if (step == null || step.isEmpty()) {
                s = 1d;
            } else {
                try {
                    s = Double.parseDouble(step);
                } catch (NumberFormatException e) {
                    s = 1d;
                }
            }

            spinner = new JSpinner(new SpinnerNumberModel(v, (Double) mi, (Double) ma, s));
            if (unit != null && !unit.isEmpty()) {
                spinner.setEditor(new JSpinner.NumberEditor(spinner, defaultDoublePattern + " " + unit));
            }
        } else {
            if (min == null || min.isEmpty()) {
                mi = Integer.MIN_VALUE;
            } else {
                try {
                    mi = Integer.parseInt(min);
                } catch (NumberFormatException e) {
                    mi = Integer.MIN_VALUE;
                }
            }
            if (max == null || max.isEmpty()) {
                ma = Integer.MAX_VALUE;
            } else {
                try {
                    ma = Integer.parseInt(max);
                } catch (NumberFormatException e) {
                    ma = Integer.MAX_VALUE;
                }
            }
            if (value == null || value.isEmpty()) {
                v = (mi.intValue() + ma.intValue()) / 2;
            } else {
                try {
                    v = Integer.parseInt(value);
                } catch (NumberFormatException e) {
                    v = (mi.intValue() + ma.intValue()) / 2;
                }
            }
            if (step == null || step.isEmpty()) {
                s = 1;
            } else {
                try {
                    s = Integer.parseInt(step);
                } catch (NumberFormatException e) {
                    s = 1;
                }
            }
            spinner = new JSpinner(new SpinnerNumberModel(v, (Integer) mi, (Integer) ma, s));
            if (unit != null && !unit.isEmpty()) {
                spinner.setEditor(new JSpinner.NumberEditor(spinner, defaultIntPattern + " " + unit));
            }
        }

        return spinner;
    }

    public void removeListener() {
        if (listener != null) {
            spinner.removeChangeListener(listener);
            listener = null;
        }
    }

    public void remove() {
        removeListener();
        super.remove();
    }

    public void setOnchange(final String action) {
        if (this.action == null) {
            removeListener();
            listener = new ChangeListener() {
                public void stateChanged(ChangeEvent e) {
                    UIWidgetTools.execAction(UINumberSpinner.this, UINumberSpinner.this.action, spinner.getValue());
                }
            };
            spinner.addChangeListener(listener);
        }
        this.action = action;
    }
}

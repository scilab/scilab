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
import org.scilab.modules.uiwidget.callback.UICallback;

/**
 * JSpinner wrapper
 */
public class UINumberSpinner extends UIComponent {

    protected static final String defaultDoublePattern = ((JSpinner.NumberEditor) new JSpinner(new SpinnerNumberModel(0., 0., 0., 0.)).getEditor()).getFormat().toPattern();
    protected static final String defaultIntPattern = ((JSpinner.NumberEditor) new JSpinner(new SpinnerNumberModel(0, 0, 0, 0)).getEditor()).getFormat().toPattern();

    protected JSpinner spinner;
    protected SpinnerNumberModel model;
    protected ChangeListener changeListener;
    protected UICallback onchangeAction;
    protected boolean onchangeEnable = true;
    protected String type;
    protected String unit;

    /**
     * {@inheritDoc}
     */
    public UINumberSpinner(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    /**
     * {@inheritDoc}
     */
    public Object newInstance() {
        model = new SpinnerNumberModel();
        spinner = new JSpinner(model);

        return spinner;
    }

    /**
     * {@inheritDoc}
     */
    @UIComponentAnnotation(attributes = {"type", "value", "min", "max", "step", "unit"})
    public Object newInstance(String type, String value, String min, String max, String step, String unit) {
        Number v, mi, ma, s;
        this.unit = unit;

        if (type != null && type.equalsIgnoreCase("double")) {
            this.type = "double";
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

            model = new SpinnerNumberModel(v, (Double) mi, (Double) ma, s);
            spinner = new JSpinner(model);
            if (unit != null && !unit.isEmpty()) {
                spinner.setEditor(new JSpinner.NumberEditor(spinner, defaultDoublePattern + " " + unit));
            }
        } else {
            this.type = "integer";
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

            model = new SpinnerNumberModel(v, (Integer) mi, (Integer) ma, s);
            spinner = new JSpinner(model);
            if (unit != null && !unit.isEmpty()) {
                spinner.setEditor(new JSpinner.NumberEditor(spinner, defaultIntPattern + " " + unit));
            }
        }

        return spinner;
    }

    /**
     * Get the spinner type (double or integer)
     * @return the spiiner type
     */
    public String getType() {
        return type;
    }

    /**
     * Set the spinner type (double or integer)
     * @param type the spinner type
     */
    public void setType(String type) {
        if (type != null && !this.type.equalsIgnoreCase(type)) {
            if (type.equalsIgnoreCase("double")) {
                this.type = "double";
                double min = (double) ((Integer) model.getMinimum());
                double max = (double) ((Integer) model.getMaximum());
                double step = (double) ((Integer) model.getStepSize());
                double value = (double) ((Integer) model.getValue());
                model = new SpinnerNumberModel(value, min, max, step);
                spinner.setModel(model);
            } else {
                this.type = "integer";
                int min = (int) (double) ((Double) model.getMinimum());
                int max = (int) (double) ((Double) model.getMaximum());
                int step = (int) (double) ((Double) model.getStepSize());
                int value = (int) (double) ((Double) model.getValue());
                model = new SpinnerNumberModel(value, min, max, step);
                spinner.setModel(model);
            }
        }
    }

    /**
     * Set the spinner value
     * @param value the value
     */
    public void setValue(double value) {
        if (type.equals("double")) {
            spinner.setValue(value);
        } else {
            spinner.setValue(new Integer((int) value));
        }
    }

    /**
     * Get the spinner value
     * @return the value
     */
    public double getValue() {
        if (type.equals("double")) {
            return (Double) spinner.getValue();
        } else {
            return (Integer) spinner.getValue();
        }
    }

    /**
     * Get the spinner unit
     * @return the unit
     */
    public String getSpinnerUnit() {
        return unit;
    }

    /**
     * Set the spinner unit
     * @param unit the unit
     */
    public void setSpinnerUnit(String unit) {
        if (type.equals("double")) {
            if (unit != null && !unit.isEmpty()) {
                spinner.setEditor(new JSpinner.NumberEditor(spinner, defaultDoublePattern + " " + unit));
            } else {
                spinner.setEditor(new JSpinner.NumberEditor(spinner, defaultDoublePattern));
            }
        } else {
            if (unit != null && !unit.isEmpty()) {
                spinner.setEditor(new JSpinner.NumberEditor(spinner, defaultIntPattern + " " + unit));
            } else {
                spinner.setEditor(new JSpinner.NumberEditor(spinner, defaultIntPattern));
            }
        }

        this.unit = unit;
    }

    /**
     * Set the min value
     * @param value the min value
     */
    public void setMin(double value) {
        if (type.equals("double")) {
            model.setMinimum(value);
        } else {
            model.setMinimum(new Integer((int) value));
        }
    }

    /**
     * Get min value for this spinner
     * @return the min
     */
    public double getMin() {
        if (type.equals("double")) {
            return (Double) model.getMinimum();
        } else {
            return (int) (Integer) model.getMinimum();
        }
    }

    /**
     * Set the max value
     * @param value the max value
     */
    public void setMax(double value) {
        if (type.equals("double")) {
            model.setMaximum(value);
        } else {
            model.setMaximum(new Integer((int) value));
        }
    }

    /**
     * Get max value for this spinner
     * @return the max
     */
    public double getMax() {
        if (type.equals("double")) {
            return (Double) model.getMaximum();
        } else {
            return (int) (Integer) model.getMaximum();
        }
    }

    /**
     * Set the step value
     * @param value the step value
     */
    public void setStep(double value) {
        if (type.equals("double")) {
            model.setStepSize(value);
        } else {
            model.setStepSize(new Integer((int) value));
        }
    }

    /**
     * Get step value for this spinner
     * @return the step
     */
    public double getStep() {
        if (type.equals("double")) {
            return (Double) model.getStepSize();
        } else {
            return (int) (Integer) model.getStepSize();
        }
    }

    /**
     * Remove change listener
     */
    protected void removeChangeListener() {
        if (changeListener != null) {
            spinner.removeChangeListener(changeListener);
            changeListener = null;
        }
    }

    /**
     * {@inheritDoc}
     */
    public void remove() {
        removeChangeListener();
        super.remove();
    }

    /**
     * Check if the onchange is enabled
     * @return true if enabled
     */
    public boolean getOnchangeEnable() {
        return onchangeEnable;
    }

    /**
     * Set if the onchange is enabled
     * @param b true if enabled
     */
    public void setOnchangeEnable(boolean b) {
        onchangeEnable = b;
    }

    /**
     * Get the onchange action
     * @return action
     */
    public UICallback getOnchange() {
        return onchangeAction;
    }

    /**
     * Set the onchange action
     * @param action the action
     */
    public void setOnchange(final String action) {
        if (this.onchangeAction == null) {
            removeChangeListener();
            changeListener = new ChangeListener() {
                public void stateChanged(ChangeEvent e) {
                    if (onchangeEnable) {
                        UIWidgetTools.execAction(UINumberSpinner.this.onchangeAction, spinner.getValue());
                    }
                }
            };
            spinner.addChangeListener(changeListener);
        }
        this.onchangeAction = UICallback.newInstance(this, action);
    }
}

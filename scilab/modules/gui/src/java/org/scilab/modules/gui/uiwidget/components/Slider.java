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

package org.scilab.modules.gui.uiwidget.components;

import java.awt.Rectangle;
import java.awt.event.AdjustmentEvent;
import java.awt.event.AdjustmentListener;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;

import org.scilab.modules.gui.bridge.slider.SwingScilabSlider;

import org.scilab.modules.gui.uiwidget.UIComponent;
import org.scilab.modules.gui.uiwidget.UIComponentAnnotation;
import org.scilab.modules.gui.uiwidget.UIWidgetException;

public class Slider extends ScilabWidget {

    private static final int MIN_KNOB_SIZE = 40;

    private static final int MINIMUM_VALUE = 0;
    private static final int MAXIMUM_VALUE = 10000;

    protected AdjustmentListener adjListener;

    public Slider(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    /**
     * {@inheritDoc}
     */
    public Object newInstance() {
        return new SwingScilabSlider();
    }

    @UIComponentAnnotation(attributes = {"slider-step"})
    public Object newInstance(double[] sliderstep) {
        final SwingScilabSlider slider = new SwingScilabSlider();

        AdjustmentListener[] als = slider.getAdjustmentListeners();
        if (als != null && als.length > 0) {
            slider.removeAdjustmentListener(als[als.length - 1]);
        }

        slider.addComponentListener(new ComponentAdapter() {
            public void componentResized(ComponentEvent e) {
                if (slider.getWidth() > slider.getHeight()) {
                    slider.setHorizontal();
                } else {
                    slider.setVertical();
                }
                slider.revalidate();
                slider.repaint();
            }
        });

        setSliderStep(sliderstep);

        return slider;
    }

    public void setValue(double[] value) {
        SwingScilabSlider slider = (SwingScilabSlider) getModifiableComponent();
        super.setValue(value);

        if (value != null && value.length >= 1) {
            if (adjListener != null) {
                slider.removeAdjustmentListener(adjListener);
            }

            slider.setValue(MINIMUM_VALUE + (int) ((value[0] - getMin()) * (MAXIMUM_VALUE - MINIMUM_VALUE) / (getMax() - getMin())));

            if (adjListener != null) {
                slider.addAdjustmentListener(adjListener);
            }
        }
    }

    public double[] getValue() {
        SwingScilabSlider slider = (SwingScilabSlider) getModifiableComponent();
        return new double[] {min + ((slider.getValue() - MINIMUM_VALUE) * (getMax() - getMin()) / (MAXIMUM_VALUE - MINIMUM_VALUE))};
    }

    public void setMin(double min) {
        super.setMin(min);
        if (getMin() <= getMax()) {
            double[] d = getSliderStep();
            setMinorTickSpacing(d[0]);
            setMajorTickSpacing(d[1]);
        }
    }

    public void setMax(double max) {
        super.setMax(max);
        if (getMin() <= getMax()) {
            double[] d = getSliderStep();
            setMinorTickSpacing(d[0]);
            setMajorTickSpacing(d[1]);
        }
    }

    /**
     * Set the major tick spacing for a Slider
     * @param space the increment value
     */
    public void setMajorTickSpacing(double space) {
        SwingScilabSlider slider = (SwingScilabSlider) getModifiableComponent();
        if (adjListener != null) {
            slider.removeAdjustmentListener(adjListener);
        }

        slider.setBlockIncrement((int) (space * (MAXIMUM_VALUE - MINIMUM_VALUE) / (getMax() - getMin())));
        int oldMax = slider.getMaximum() - slider.getVisibleAmount();
        slider.setVisibleAmount(Math.max((int) ((MAXIMUM_VALUE - MINIMUM_VALUE) / space), MIN_KNOB_SIZE));
        slider.setMaximum(oldMax + slider.getVisibleAmount());

        if (adjListener != null) {
            slider.addAdjustmentListener(adjListener);
        }
    }

    /**
     * Set the minor tick spacing for a Slider
     * @param space the increment value
     */
    public void setMinorTickSpacing(double space) {
        SwingScilabSlider slider = (SwingScilabSlider) getModifiableComponent();
        if (adjListener != null) {
            slider.removeAdjustmentListener(adjListener);
        }

        slider.setUnitIncrement((int) (space * (MAXIMUM_VALUE - MINIMUM_VALUE) / (getMax() - getMin())));

        if (adjListener != null) {
            slider.addAdjustmentListener(adjListener);
        }
    }

    public void setSliderStep(double[] d) {
        super.setSliderStep(d);
        double[] dd = getSliderStep();
        setMinorTickSpacing(dd[0]);
        setMajorTickSpacing(dd[1]);
    }

    public void setCallback(String callback) {
        SwingScilabSlider slider = (SwingScilabSlider) getModifiableComponent();
        if (adjListener != null) {
            slider.removeAdjustmentListener(adjListener);
            adjListener = null;
        }

        super.setCallback(callback);

        if (commonCallBack != null) {
            adjListener = new AdjustmentListener() {
                public void adjustmentValueChanged(AdjustmentEvent e) {
                    if (commonCallBack != null) {
                        commonCallBack.actionPerformed(null);
                    }
                }
            };
            slider.addAdjustmentListener(adjListener);
        }
    }
}

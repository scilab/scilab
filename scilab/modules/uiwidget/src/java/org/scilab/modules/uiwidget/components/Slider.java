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

import java.awt.Rectangle;

import org.scilab.modules.gui.bridge.slider.SwingScilabSlider;

import org.scilab.modules.uiwidget.UIComponent;
import org.scilab.modules.uiwidget.UIWidgetException;

public class Slider extends ScilabWidget {

    private static final int MIN_KNOB_SIZE = 40;

    private static final int MINIMUM_VALUE = 0;
    private static final int MAXIMUM_VALUE = 10000;

    public Slider(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    public Object newInstance() {
        widget = new SwingScilabSlider();
        min = 0;
        max = 10000;
        value = 0;

        return widget;
    }

    // public void setPosition(Rectangle r) {
    // 	SwingScilabSlider slider = (SwingScilabSlider) widget;
    // 	if (r.width >= r.height) {
    // 	    slider.setHorizontal();
    // 	} else {
    // 	    slider.setVertical();
    // 	}
    // 	super.setPosition(r);
    // }

    public void setValue(double value) {
        SwingScilabSlider slider = (SwingScilabSlider) widget;
        super.setValue(value);
        slider.setValue(MINIMUM_VALUE + (int) ((value - getMin()) * (MAXIMUM_VALUE - MINIMUM_VALUE) / (getMax() - getMin())));
    }

    public void setMin(double min) {
        super.setMin(min);
        SwingScilabSlider slider = (SwingScilabSlider) widget;
        slider.setValue((int) (min + ((getValue() - MINIMUM_VALUE) * (getMax() - min) / (MAXIMUM_VALUE - MINIMUM_VALUE))));
    }

    public void setMax(double max) {
        super.setMax(max);
        SwingScilabSlider slider = (SwingScilabSlider) widget;
        slider.setValue((int) (getMin() + ((getValue() - MINIMUM_VALUE) * (max - getMin()) / (MAXIMUM_VALUE - MINIMUM_VALUE))));
    }

    public void setSliderStep(double[] d) {
        if (d.length >= 2) {
            SwingScilabSlider slider = (SwingScilabSlider) widget;
            slider.setUnitIncrement((int) (d[0] * (MAXIMUM_VALUE - MINIMUM_VALUE) / (getMax() - getMin())));

            slider.setBlockIncrement((int) (d[1] * (MAXIMUM_VALUE - MINIMUM_VALUE) / (getMax() - getMin())));
            int oldMax = slider.getMaximum() - slider.getVisibleAmount();
            slider.setVisibleAmount(Math.max((int) ((MAXIMUM_VALUE - MINIMUM_VALUE) / d[1]), MIN_KNOB_SIZE));
            slider.setMaximum(oldMax + slider.getVisibleAmount());
        }
    }
}

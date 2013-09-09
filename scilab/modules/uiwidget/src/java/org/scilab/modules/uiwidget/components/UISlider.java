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

import java.util.Map;

import javax.swing.JSlider;
import javax.swing.SwingConstants;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

import org.scilab.modules.uiwidget.UIComponent;
import org.scilab.modules.uiwidget.UIComponentAnnotation;
import org.scilab.modules.uiwidget.UIWidgetException;
import org.scilab.modules.uiwidget.UIWidgetTools;
import org.scilab.modules.uiwidget.callback.UICallback;

public class UISlider extends UIComponent {

    private JSlider slider;
    private ChangeListener listener;
    private boolean onchangeEnable = true;
    private UICallback action;

    public enum Orientation {
        HORIZONTAL (SwingConstants.HORIZONTAL),
        VERTICAL (SwingConstants.VERTICAL);

        private final int value;

        Orientation(int value) {
            this.value = value;
        }

        public int value() {
            return value;
        }
    }

    public UISlider(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    public Object newInstance() {
        slider = new JSlider();

        return slider;
    }

    @UIComponentAnnotation(attributes = {"orientation", "min", "max", "value", "font", "size"})
    public Object newInstance(Orientation orientation, int min, int max, int value, String font, int size) {
        if (min == Integer.MAX_VALUE || max == Integer.MAX_VALUE || value == Integer.MAX_VALUE || min > max || value < min || value > max) {
            slider = new JSlider(orientation.value());
        } else {
            slider = new JSlider(orientation.value(), min, max, value);
        }

        if (font != null && !font.isEmpty()) {
            slider.setFont(UITools.getFont(null, null, font, size, false, false, false, false, null));
        } else {
            slider.setFont(UITools.getFont(null, null, slider.getFont(), size, false, false, false, false, null));
        }

        return slider;
    }

    public void setLabelsIncrement(int increment) {
        slider.setLabelTable(slider.createStandardLabels(increment));
        slider.setPaintLabels(true);
        slider.setMinorTickSpacing(increment);
    }

    /**
     * Remove the onclick listener
     */
    protected void removeListener() {
        if (listener != null) {
            slider.removeChangeListener(listener);
            listener = null;
        }
    }

    /**
     * {@inheritDoc}
     */
    public void remove() {
        removeListener();
        super.remove();
    }

    public UICallback getOnchange() {
        return action;
    }

    public void setOnchange(final String action) {
        if (this.action == null) {
            removeListener();
            listener = new ChangeListener() {
                public void stateChanged(ChangeEvent e) {
                    if (onchangeEnable) {
                        UIWidgetTools.execAction(UISlider.this.action, slider.getValue());
                    }
                }
            };
            slider.addChangeListener(listener);
        }
        this.action = UICallback.newInstance(this, action);
    }

    public boolean getOnchangeEnable() {
        return onchangeEnable;
    }

    public void setOnchangeEnable(boolean b) {
        onchangeEnable = b;
    }
}

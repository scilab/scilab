/*
 * Uicontrol2 ( http://forge.scilab.org/index.php/p/uicontrol2/ ) - This file is a part of Uicontrol2
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

import org.scilab.modules.action_binding.InterpreterManagement;

public class UISlider extends UIComponent {

    private JSlider slider;
    private boolean onchangeEnable = true;
    private String action;

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

    public void setOnchange(final String action) {
        if (this.action == null) {
            slider.addChangeListener(new ChangeListener() {
                public void stateChanged(ChangeEvent e) {
                    if (onchangeEnable) {
                        InterpreterManagement.requestScilabExec(UISlider.this.action + "(\"" + UISlider.this.getUIPath() + "\"," + Integer.toString(slider.getValue()) + ")");
                    }
                }
            });
        }
        this.action = action;
    }

    public void setOnchangeEnable(boolean b) {
        onchangeEnable = b;
    }
}

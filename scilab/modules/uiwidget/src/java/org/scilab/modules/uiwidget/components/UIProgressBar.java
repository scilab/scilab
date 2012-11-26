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

import javax.swing.JProgressBar;
import javax.swing.SwingConstants;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

import org.scilab.modules.uiwidget.UIComponent;
import org.scilab.modules.uiwidget.UIComponentAnnotation;
import org.scilab.modules.uiwidget.UIWidgetException;
import org.scilab.modules.uiwidget.UIWidgetTools;

public class UIProgressBar extends UIComponent {

    private JProgressBar bar;
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

    public UIProgressBar(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    public Object newInstance() {
        bar = new JProgressBar();

        return bar;
    }

    @UIComponentAnnotation(attributes = {"orientation", "min", "max"})
    public Object newInstance(Orientation orientation, int min, int max) {
        if (min == Integer.MAX_VALUE || max == Integer.MAX_VALUE || min > max) {
            bar = new JProgressBar(orientation.value());
        } else {
            bar = new JProgressBar(orientation.value(), min, max);
        }

        return bar;
    }

    public void setOnchange(final String action) {
        if (this.action == null) {
            bar.addChangeListener(new ChangeListener() {
                public void stateChanged(ChangeEvent e) {
                    if (onchangeEnable) {
                        UIWidgetTools.execAction(UIProgressBar.this, UIProgressBar.this.action, bar.getValue());
                    }
                }
            });
        }
        this.action = action;
    }

    public void setString(String str) {
        bar.setStringPainted(true);
        if (str == null || str.isEmpty()) {
            bar.setString(null);
        } else {
            bar.setString(str);
        }
    }

    public void setOnchangeEnable(boolean b) {
        onchangeEnable = b;
    }
}

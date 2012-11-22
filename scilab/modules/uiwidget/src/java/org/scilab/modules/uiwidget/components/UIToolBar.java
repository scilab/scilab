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

import javax.swing.JToolBar;

import org.scilab.modules.uiwidget.UIComponent;
import org.scilab.modules.uiwidget.UIComponentAnnotation;
import org.scilab.modules.uiwidget.UIWidgetException;

public class UIToolBar extends UIComponent {

    private JToolBar bar;

    public enum Orientation {
        HORIZONTAL (JToolBar.HORIZONTAL),
        VERTICAL (JToolBar.VERTICAL);

        private final int value;

        Orientation(int value) {
            this.value = value;
        }

        public int value() {
            return value;
        }

        public String toString() {
            switch (this) {
                case HORIZONTAL:
                    return "horizontal";
                case VERTICAL:
                    return "vertical";
                default:
                    return "";
            }
        }
    }

    public UIToolBar(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    public Object newInstance() {
        bar = new JToolBar();

        return bar;
    }

    @UIComponentAnnotation(attributes = {"name", "orientation"})
    public Object newInstance(String name, Orientation orientation) {
        if (name == null || name.isEmpty()) {
            bar = new JToolBar(orientation.value());
        } else {
            bar = new JToolBar(name, orientation.value());
        }

        return bar;
    }

    public Orientation getOrientation() {
        switch (bar.getOrientation()) {
            case JToolBar.VERTICAL:
                return Orientation.VERTICAL;
            default:
                return Orientation.HORIZONTAL;
        }
    }
}

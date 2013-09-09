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

import javax.swing.JToolBar;

import org.scilab.modules.gui.toolbar.ScilabToolBar;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.bridge.toolbar.SwingScilabToolBar;
import org.scilab.modules.uiwidget.UIComponent;
import org.scilab.modules.uiwidget.UIComponentAnnotation;
import org.scilab.modules.uiwidget.UIWidgetException;

/**
 * ToolBar wrapper
 */
public class UIToolBar extends UIComponent {

    protected ToolBar bar;

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
        bar = ScilabToolBar.createToolBar();

        return bar.getAsSimpleToolBar();
    }

    @UIComponentAnnotation(attributes = {"name", "orientation"})
    public Object newInstance(String name, Orientation orientation) {
        bar = ScilabToolBar.createToolBar();
        SwingScilabToolBar toolbar = (SwingScilabToolBar) bar.getAsSimpleToolBar();
        if (name == null) {
            toolbar.setName(name);
        }

        toolbar.setOrientation(orientation.value());

        return toolbar;
    }

    public void add(final UIComponent comp) throws UIWidgetException {
        if (comp instanceof UISeparator) {
            SwingScilabToolBar toolbar = (SwingScilabToolBar) bar.getAsSimpleToolBar();
            toolbar.addSeparator();
        } else {
            super.add(comp);
        }
    }

    public Orientation getOrientation() {
        switch (((SwingScilabToolBar) bar.getAsSimpleToolBar()).getOrientation()) {
            case JToolBar.VERTICAL:
                return Orientation.VERTICAL;
            default:
                return Orientation.HORIZONTAL;
        }
    }
}

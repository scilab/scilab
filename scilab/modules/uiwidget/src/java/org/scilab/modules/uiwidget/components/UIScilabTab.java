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

import java.awt.Color;
import java.awt.Font;
import java.util.Map;

import javax.swing.JComponent;
import javax.swing.JPanel;

import org.scilab.modules.uiwidget.UIComponent;
import org.scilab.modules.uiwidget.UIComponentAnnotation;
import org.scilab.modules.uiwidget.UIWidgetException;

import org.scilab.modules.gui.bridge.tab.SwingScilabTab;

public class UIScilabTab extends UIComponent {

    SwingScilabTab tab;

    public UIScilabTab(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    public Object newInstance() {
        tab = new SwingScilabTab("");

        return tab;
    }

    @UIComponentAnnotation(attributes = {"name"})
    public Object newInstance(String name) {
        tab = new SwingScilabTab(name == null ? "" : name);
        //tab.setContentPane(new JPanel());

        return tab;
    }

    public void add(final UIComponent comp) throws UIWidgetException {
        if (comp.getComponent() instanceof JComponent) {
            tab.setContentPane(comp.getJComponent());
        } else {
            super.add(comp);
        }
    }

    public void closeUIComponent() {
        tab.close();
    }
}

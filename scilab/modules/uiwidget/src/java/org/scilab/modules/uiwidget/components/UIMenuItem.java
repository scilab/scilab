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

import javax.swing.Icon;
import javax.swing.JCheckBoxMenuItem;
import javax.swing.JMenuItem;
import javax.swing.JRadioButtonMenuItem;
import javax.swing.KeyStroke;

import org.scilab.modules.uiwidget.UIComponent;
import org.scilab.modules.uiwidget.UIComponentAnnotation;
import org.scilab.modules.uiwidget.UIWidgetException;

public class UIMenuItem extends UIComponent {

    private MenuType type = MenuType.NORMAL;
    private JMenuItem menu;

    public enum MenuType {
        NORMAL, RADIO, CHECK;
    }

    public UIMenuItem(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    public Object newInstance() {
        menu = new JMenuItem();

        return menu;
    }

    @UIComponentAnnotation(attributes = {"text", "icon", "accelerator", "type", "selected"})
    public Object newInstance(String text, Icon icon, KeyStroke accel, MenuType type, boolean selected) {
        this.type = type;
        switch (type) {
            case NORMAL:
                menu = new JMenuItem(text);
                break;
            case RADIO:
                menu = new JRadioButtonMenuItem(text, selected);
                break;
            case CHECK:
                menu = new JCheckBoxMenuItem(text, selected);
                break;
        }

        if (icon != null) {
            menu.setIcon(icon);
        }

        if (accel != null) {
            menu.setAccelerator(accel);
        }

        return menu;
    }
}

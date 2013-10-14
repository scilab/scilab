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

import java.util.Map;

import javax.swing.JPopupMenu;

import org.scilab.modules.gui.uiwidget.UIComponent;
import org.scilab.modules.gui.uiwidget.UIComponentAnnotation;
import org.scilab.modules.gui.uiwidget.UIWidgetException;

public class UIPopupMenu extends UIComponent {

    private JPopupMenu menu;

    public UIPopupMenu(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    public Object newInstance() {
        menu = new JPopupMenu();

        return menu;
    }

    @UIComponentAnnotation(attributes = {"label"})
    public Object newInstance(String label) {
        if (label != null && !label.isEmpty()) {
            menu = new JPopupMenu(label);
        } else {
            menu = new JPopupMenu();
        }

        return menu;
    }
}
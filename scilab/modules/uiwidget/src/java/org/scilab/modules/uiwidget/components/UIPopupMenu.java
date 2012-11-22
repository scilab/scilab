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

import javax.swing.JPopupMenu;

import org.scilab.modules.uiwidget.UIComponent;
import org.scilab.modules.uiwidget.UIComponentAnnotation;
import org.scilab.modules.uiwidget.UIWidgetException;

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
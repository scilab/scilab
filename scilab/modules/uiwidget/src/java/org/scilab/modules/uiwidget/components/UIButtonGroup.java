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

import java.awt.Component;
import java.util.Map;

import javax.swing.AbstractButton;
import javax.swing.ButtonGroup;
import javax.swing.JPanel;

import org.scilab.modules.uiwidget.UIComponent;
import org.scilab.modules.uiwidget.UIWidgetException;

public class UIButtonGroup extends UIComponent {

    ButtonGroup group;
    JPanel panel;

    public UIButtonGroup(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    public Object newInstance() {
        panel = new JPanel();
        group = new ButtonGroup();

        return panel;
    }

    public void add(Component c) {
        if (c instanceof AbstractButton) {
            panel.add(c);
            group.add((AbstractButton) c);
        }
    }
}

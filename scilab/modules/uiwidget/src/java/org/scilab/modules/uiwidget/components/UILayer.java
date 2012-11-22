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

import java.awt.BorderLayout;
import java.awt.Component;
import java.util.HashMap;
import java.util.Map;

import javax.swing.JPanel;
import javax.swing.JPopupMenu;

import org.scilab.modules.uiwidget.UIComponent;
import org.scilab.modules.uiwidget.UIComponentAnnotation;
import org.scilab.modules.uiwidget.UIWidgetException;

public class UILayer extends UIComponent {

    JPanel layer;
    UIComponent current;
    Map<String, UIComponent> children;

    public UILayer(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    public Object newInstance() {
        layer = new JPanel();
        layer.setLayout(new BorderLayout());
        children = new HashMap<String, UIComponent>();

        return layer;
    }

    public void setFront(String id) throws UIWidgetException {
        UIComponent c = children.get(id);
        if (c != null) {
            layer.remove(current.getJComponent());
            layer.add(c.getJComponent(), BorderLayout.CENTER);
            layer.doLayout();
            layer.repaint();
            current = c;
        }
    }

    public void add(final UIComponent c) throws UIWidgetException {
        Object o = c.getComponent();
        if (o instanceof Component && !(o instanceof JPopupMenu)) {
            if (current == null) {
                layer.add((Component) o, BorderLayout.CENTER);
                current = c;
            }
            String id = c.getId();
            if (id != null && !id.isEmpty()) {
                children.put(id, c);
            }
        } else {
            super.add(c);
        }
    }
}


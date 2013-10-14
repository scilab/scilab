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

import java.awt.BorderLayout;
import java.awt.Component;

import javax.swing.JPanel;
import javax.swing.JPopupMenu;

import org.scilab.modules.gui.uiwidget.UIComponent;
import org.scilab.modules.gui.uiwidget.UIComponentAnnotation;
import org.scilab.modules.gui.uiwidget.UIWidgetException;

/**
 * Wrapper for a kind of component stack
 */
public class UILayer extends UIComponent {

    protected JPanel layer;
    protected UIComponent current;

    /**
     * {@inheritDoc}
     */
    public UILayer(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    /**
     * {@inheritDoc}
     */
    public Object newInstance() {
        layer = new JPanel();
        layer.setLayout(new BorderLayout());

        return layer;
    }

    /**
     * Set a component to the front
     * @param id the component id
     */
    public void setFront(String id) throws UIWidgetException {
        UIComponent c = children.get(id);
        if (c != null && current != c) {
            layer.remove(current.getJComponent());
            layer.add(c.getJComponent(), BorderLayout.CENTER);
            layer.revalidate();
            layer.repaint();
            current = c;
        }
    }

    /**
     * Get the component which is on the front
     * @return the component id
     */
    public String getFront() {
        if (current != null) {
            return current.getID();
        }

        return null;
    }

    /**
     * {@inheritDoc}
     */
    public void add(final UIComponent c) throws UIWidgetException {
        Object o = c.getComponent();
        if (o instanceof Component && !(o instanceof JPopupMenu)) {
            if (current == null) {
                layer.add((Component) o, BorderLayout.CENTER);
                current = c;
            }
        } else {
            super.add(c);
        }
    }
}

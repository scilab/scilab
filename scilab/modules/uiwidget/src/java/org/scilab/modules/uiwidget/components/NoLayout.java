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

import java.awt.Component;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.LayoutManager2;
import java.awt.Rectangle;
import java.util.HashMap;
import java.util.Map;

/**
 * No Layout
 */
public class NoLayout implements LayoutManager2 {

    private Map<Component, Rectangle> map;

    /**
     * Default constructor
     */
    public NoLayout() {
        map = new HashMap<Component, Rectangle>();
    }

    /**
     * {@inheritDoc}
     */
    public void addLayoutComponent(Component comp, Object constraints) {
        if (constraints instanceof Rectangle) {
            map.put(comp, (Rectangle) constraints);
        }
    }

    /**
     * Get the component constraints
     * @param comp the component
     * @return the constraints
     */
    public Object getConstraints(Component comp) {
        return map.get(comp);
    }

    /**
     * {@inheritDoc}
     */
    public float getLayoutAlignmentX(Container target) {
        return 0.5f;
    }

    /**
     * {@inheritDoc}
     */
    public float getLayoutAlignmentY(Container target) {
        return 0.5f;
    }

    /**
     * {@inheritDoc}
     */
    public void invalidateLayout(Container target) {

    }

    /**
     * {@inheritDoc}
     */
    public Dimension maximumLayoutSize(Container target) {
        return new Dimension(Integer.MAX_VALUE, Integer.MAX_VALUE);
    }

    /**
     * {@inheritDoc}
     */
    public void addLayoutComponent(String name, Component comp) {

    }

    /**
     * {@inheritDoc}
     */
    public void layoutContainer(Container parent) {
        synchronized (parent.getTreeLock()) {
            int ncomponents = parent.getComponentCount();
            if (ncomponents == 0) {
                return;
            }

            for (int i = 0; i < ncomponents; i++) {
                Component c = parent.getComponent(i);
                Rectangle r = map.get(c);
                if (r != null) {
                    c.setBounds(r);
                }
            }
        }
    }

    /**
     * {@inheritDoc}
     */
    public Dimension minimumLayoutSize(Container parent) {
        return new Dimension(0, 0);
    }

    /**
     * {@inheritDoc}
     */
    public Dimension preferredLayoutSize(Container parent) {
        synchronized (parent.getTreeLock()) {
            return parent.getSize();
        }
    }

    /**
     * {@inheritDoc}
     */
    public void removeLayoutComponent(Component comp) {
        map.remove(comp);
    }
}

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
import java.awt.Container;
import java.awt.Dimension;
import java.awt.LayoutManager2;
import java.awt.Rectangle;
import java.util.HashMap;
import java.util.Map;

public class NoLayout implements LayoutManager2 {

    Map<Component, Rectangle> map;

    public NoLayout() {
        map = new HashMap<Component, Rectangle>();
    }

    public void addLayoutComponent(Component comp, Object constraints) {
        if (constraints instanceof Rectangle) {
            map.put(comp, (Rectangle) constraints);
        }
    }

    public float getLayoutAlignmentX(Container target) {
        return 0.5f;
    }

    public float getLayoutAlignmentY(Container target) {
        return 0.5f;
    }

    public void invalidateLayout(Container target) {

    }

    public Dimension maximumLayoutSize(Container target) {
        return new Dimension(Integer.MAX_VALUE, Integer.MAX_VALUE);
    }

    public void addLayoutComponent(String name, Component comp) {
    }

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

    public Dimension minimumLayoutSize(Container parent) {
        return new Dimension(0, 0);
    }

    public Dimension preferredLayoutSize(Container parent) {
        synchronized (parent.getTreeLock()) {
            return parent.getSize();
        }
    }

    public void removeLayoutComponent(Component comp) {
        map.remove(comp);
    }
}

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

import java.awt.Component;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.LayoutManager2;
import java.awt.Rectangle;
import java.awt.Toolkit;
import java.awt.geom.Rectangle2D;
import java.util.IdentityHashMap;
import java.util.Map;
import javax.swing.JComponent;

/**
 * NoLayout: Layout Manager to lay out a la Scilab
 * When a component is added with a constraint NoLayoutConstraint this last one
 * can be directly modified: it is linked with the component (via a Map).
 */
public class NoLayout implements LayoutManager2 {

    public static final int UNIT_PIXELS = 0;
    public static final int UNIT_NORMALIZED = 1;
    public static final int UNIT_POINTS = 2;

    private Map<Component, NoLayoutConstraint> map;

    /**
     * Default constructor
     */
    public NoLayout() {
        map = new IdentityHashMap<Component, NoLayoutConstraint>();
    }

    /**
     * {@inheritDoc}
     */
    public void addLayoutComponent(Component comp, Object constraints) {
        if (comp != null && constraints instanceof NoLayoutConstraint) {
            map.put(comp, (NoLayoutConstraint) constraints);
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

    private final double getResolutionScale() {
        return ((double) Toolkit.getDefaultToolkit().getScreenResolution()) / 72.0d;
    }

    /**
     * {@inheritDoc}
     */
    public void layoutContainer(Container parent) {
        synchronized (parent.getTreeLock()) {
            final int ncomponents = parent.getComponentCount();
            if (ncomponents == 0) {
                return;
            }

            final Rectangle rp = parent.getBounds();
            final Rectangle rr = new Rectangle();
            final double scale = getResolutionScale();

            for (int i = 0; i < ncomponents; i++) {
                final Component c = parent.getComponent(i);
                final NoLayoutConstraint constraint = map.get(c);
                if (constraint != null) {
                    switch (constraint.unit[0]) {
                        case UNIT_NORMALIZED: // normalized unit
                            rr.x = (int) (correct(constraint.bounds.x) * rp.width);
                            break;
                        case UNIT_POINTS: // point unit
                            rr.x = (int) (constraint.bounds.x * scale);
                            break;
                        default: // pixel unit
                            rr.x = (int) constraint.bounds.x;
                            break;
                    }

                    switch (constraint.unit[2]) {
                        case UNIT_NORMALIZED:
                            rr.width = (int) (correct(constraint.bounds.width) * rp.width);
                            break;
                        case UNIT_POINTS:
                            rr.width = (int) (constraint.bounds.width * scale);
                            break;
                        default:
                            rr.width = (int) constraint.bounds.width;
                            break;
                    }

                    switch (constraint.unit[3]) {
                        case UNIT_NORMALIZED:
                            rr.height = (int) (correct(constraint.bounds.height) * rp.height);
                            break;
                        case UNIT_POINTS:
                            rr.height = (int) (constraint.bounds.height * scale);
                            break;
                        default:
                            rr.height = (int) constraint.bounds.height;
                            break;
                    }

                    switch (constraint.unit[1]) {
                        case UNIT_NORMALIZED:
                            rr.y = (int) (rp.height * (1 - correct(constraint.bounds.y))) - rr.height;
                            break;
                        case UNIT_POINTS:
                            rr.y = (int) (rp.height - constraint.bounds.y * scale) - rr.height;
                            break;
                        default:
                            rr.y = (int) (rp.height - constraint.bounds.y) - rr.height;
                            break;
                    }

                    if (!c.getBounds().equals(rr)) {
                        c.setBounds(rr);
                        /*if (bounds.width != rr.width || bounds .height != rr.height) {
                            if (c instanceof JComponent) {
                        	((JComponent) c).revalidate();
                            } else {
                        	c.invalidate();
                        	c.validate();
                            }
                            }*/
                    }
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

    /**
     * @return a value between 0 and 1
     */
    private static double correct(double x) {
        return x >= 0 ? (x >= 1 ? 1 : x) : 0;
    }

    public static class NoLayoutConstraint {

        public Rectangle2D.Double bounds = new Rectangle2D.Double();
        public int[] unit = new int[4];

        public NoLayoutConstraint() {
        }

        public NoLayoutConstraint(double x, double y, double w, double h, int unit) {
            this(x, y, w, h, unit, unit, unit, unit);
        }

        public NoLayoutConstraint(double x, double y, double w, double h, int xUnit, int yUnit, int wUnit, int hUnit) {
            this();
            setPoint(x, y);
            setDims(w, h);
            setUnit(xUnit, yUnit, wUnit, hUnit);
        }

        public void setPoint(double x, double y) {
            bounds.x = x;
            bounds.y = y;
        }

        public void setDims(double w, double h) {
            bounds.width = w;
            bounds.height = h;
        }

        public void setUnit(int x, int y, int w, int h) {
            unit[0] = x;
            unit[1] = y;
            unit[2] = w;
            unit[3] = h;
        }


        public String toString() {
            return "NoLayoutConstraint:" + bounds + " units:" + unit[0] + ";" + unit[1] + ";" + unit[2] + ";" + unit[3];
        }
    }
}

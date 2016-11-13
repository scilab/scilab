/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2010 - DIGITEO - Pierre Lando
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 */

package org.scilab.modules.renderer.JoGLView.interaction;

import org.scilab.modules.graphic_objects.axes.Axes;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.renderer.JoGLView.DrawerVisitor;

import java.awt.Dimension;
import java.awt.Point;
import java.util.ArrayList;

/**
 * @author Pierre Lando
 */
public abstract class FigureInteraction {

    /** parent figure drawer */
    protected DrawerVisitor drawerVisitor;

    /** Enable status */
    private boolean isEnable;

    /**
     * Default constructor.
     * @param drawerVisitor parent figure drawer.
     */
    public FigureInteraction(DrawerVisitor drawerVisitor) {
        this.drawerVisitor = drawerVisitor;
        isEnable = false;
    }

    /**
     * Drawer visitor getter.
     * @return the parent figure drawer.
     */
    protected DrawerVisitor getDrawerVisitor() {
        return drawerVisitor;
    }

    /**
     * Compute the underlying {@link org.scilab.modules.graphic_objects.axes.Axes} for the given point in figure coordinates.
     * @param point given point in figure coordinates.
     * @return the underlying {@link org.scilab.modules.graphic_objects.axes.Axes} for the given point in figure coordinates.
     */
    protected Axes getUnderlyingAxes(Point point) {
        Axes underlyingAxes = null;
        Dimension size = drawerVisitor.getCanvas().getDimension();
        double x = point.getX() / size.getWidth();
        double y = point.getY() / size.getHeight();
        for (Integer childId : drawerVisitor.getFigure().getChildren()) {
            GraphicObject child = GraphicController.getController().getObjectFromId(childId);
            if (child instanceof Axes) {
                if (child.getVisible()) {
                    Double[] axesBounds = ((Axes) child).getAxesBounds();  // x y w h
                    if ((x >= axesBounds[0]) && (x <= axesBounds[0] + axesBounds[2]) && (y >= axesBounds[1]) && (y <= axesBounds[1] + axesBounds[3])) {
                        underlyingAxes = (Axes) child;
                        return underlyingAxes;
                    }
                }
            }
        }
        return underlyingAxes;
    }

    protected Axes[] getAllUnderlyingAxes(Point point) {
        ArrayList<Axes> underlyingAxes = new ArrayList<Axes>();
        Dimension size = drawerVisitor.getCanvas().getDimension();
        double x = point.getX() / size.getWidth();
        double y = point.getY() / size.getHeight();
        for (Integer childId : drawerVisitor.getFigure().getChildren()) {
            GraphicObject child = GraphicController.getController().getObjectFromId(childId);
            if (child instanceof Axes) {
                if (child.getVisible()) {
                    Double[] axesBounds = ((Axes) child).getAxesBounds();  // x y w h
                    if ((x >= axesBounds[0]) && (x <= axesBounds[0] + axesBounds[2]) && (y >= axesBounds[1]) && (y <= axesBounds[1] + axesBounds[3])) {
                        underlyingAxes.add((Axes) child);
                    }
                }
            }
        }
        Axes [] ret;
        if (underlyingAxes.size() > 0) {
            ret = new Axes[underlyingAxes.size()];
            ret = underlyingAxes.toArray(ret);
        } else {
            ret = new Axes[0];
        }
        return ret;
    }

    protected Axes[] getAllVisibleAxes(Point point) {
        ArrayList<Axes> underlyingAxes = new ArrayList<Axes>();
        Dimension size = drawerVisitor.getCanvas().getDimension();
        double x = point.getX() / size.getWidth();
        double y = point.getY() / size.getHeight();
        for (Integer childId : drawerVisitor.getFigure().getChildren()) {
            GraphicObject child = GraphicController.getController().getObjectFromId(childId);
            if (child instanceof Axes) {
                if (child.getVisible()) {
                    underlyingAxes.add((Axes) child);
                }
            }
        }
        Axes [] ret;
        if (underlyingAxes.size() > 0) {
            ret = new Axes[underlyingAxes.size()];
            ret = underlyingAxes.toArray(ret);
        } else {
            ret = new Axes[0];
        }
        return ret;
    }

    /**
     * Enable status getter.
     * @return the enable status.
     */
    public boolean isEnable() {
        return isEnable;
    }

    /**
     * Enable status setter.
     * @param isEnable the new enable status setter.
     */
    public void setEnable(boolean isEnable) {
        if (this.isEnable != isEnable) {
            this.isEnable = isEnable;
            changeEnable(isEnable);
        }
    }

    /**
     * Called when the enable status have changed.
     * @param isEnable the new enable status.
     */
    protected abstract void changeEnable(boolean isEnable);

    /**
     * Tight given bounds to axes data bounds.
     * @param axes the given axes.
     * @param zoomBounds the zoomBounds.
     * @return true if actually there is a zoom.
     */
    protected boolean tightZoomBounds(Axes axes, Double[] zoomBounds) {
        boolean zoomed = false;
        Double[] dataBounds = new Double[] { -1., 1., -1., 1., -1., 1.};
        for (int i : new int[] {0, 2, 4}) {
            if (zoomBounds[i] < dataBounds[i]) {
                zoomBounds[i] = dataBounds[i];
            } else {
                zoomed = true;
            }
        }

        for (int i : new int[] {1, 3, 5}) {
            if (zoomBounds[i] > dataBounds[i]) {
                zoomBounds[i] = dataBounds[i];
            } else {
                zoomed = true;
            }
        }

        return zoomed;
    }

    /**
     * bound the given point to canvas.
     * @param point the given point.
     * @return a point bounded in the canvas.
     */
    protected Point bound(Point point) {
        Dimension dimension = getDrawerVisitor().getCanvas().getDimension();
        return new Point(
                   Math.max(1, Math.min(point.x, dimension.width)),
                   Math.max(1, Math.min(point.y, dimension.height))
               );
    }
}

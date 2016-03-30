/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2011-2015 - Scilab Enterprises - Clement DAVID
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

package org.scilab.modules.xcos.link;

import java.awt.MouseInfo;
import java.awt.geom.Line2D;
import java.awt.geom.Point2D;
import java.util.ArrayList;
import java.util.List;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.DeleteAction;
import org.scilab.modules.gui.bridge.contextmenu.SwingScilabContextMenu;
import org.scilab.modules.gui.contextmenu.ContextMenu;
import org.scilab.modules.gui.contextmenu.ScilabContextMenu;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menu.ScilabMenu;
import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.ObjectProperties;
import org.scilab.modules.xcos.actions.EditFormatAction;
import org.scilab.modules.xcos.block.actions.BorderColorAction;
import org.scilab.modules.xcos.graph.model.XcosCell;
import org.scilab.modules.xcos.link.actions.StyleHorizontalAction;
import org.scilab.modules.xcos.link.actions.StyleStraightAction;
import org.scilab.modules.xcos.link.actions.StyleVerticalAction;
import org.scilab.modules.xcos.preferences.XcosOptions;
import org.scilab.modules.xcos.utils.XcosMessages;

import com.mxgraph.model.mxGeometry;
import com.mxgraph.model.mxICell;
import com.mxgraph.util.mxPoint;
import com.mxgraph.util.mxRectangle;

/**
 * Root base class for links.
 *
 * A link is always oriented from Input to Output or from Command to Control.
 */
// CSOFF: ClassDataAbstractionCoupling
public abstract class BasicLink extends XcosCell {
    private static final mxGeometry DEFAULT_GEOMETRY = new mxGeometry(0, 0, 80, 80);
    private static final int DETECTION_RECTANGLE_DIMENSION = 10;
    private transient int ordering;

    public BasicLink(JavaController controller, long uid, Kind kind, Object value, mxGeometry geometry, String style, String id, int linkKind) {
        super(controller, uid, kind, value, geometry, style == null ? XcosOptions.getEdition().getEdgeStyle() : style, id);

        this.vertex = false;
        this.edge = true;

        controller.setObjectProperty(getUID(), getKind(), ObjectProperties.KIND, linkKind);
    }

    /**
     * @param ordering
     *            a unique order number per instance
     */
    public void setOrdering(int ordering) {
        this.ordering = ordering;
    }

    /**
     * @return the unique order number per instance
     */
    public int getOrdering() {
        return ordering;
    }

    /**
     * @param index
     *            the point index to be removed
     */
    public void removePoint(int index) {
        if (getGeometry() == null || getGeometry().getPoints() == null) {
            return;
        }
        if (index < getGeometry().getPoints().size()) {
            getGeometry().getPoints().remove(index);
        }
    }

    /** Remove all the points */
    private void removePoints() {
        getGeometry().setPoints(new ArrayList<mxPoint>());
    }

    /**
     * Get all the points
     *
     * @param index
     *            the start index
     * @param fromStart
     *            if true, get the 0 - index range; get the index - max range
     *            otherwise
     * @return The points
     */
    public List<mxPoint> getPoints(int index, boolean fromStart) {

        if (getGeometry() == null || getGeometry().getPoints() == null) {
            return new ArrayList<mxPoint>();
        }

        final List<mxPoint> points = getGeometry().getPoints();
        if (fromStart) {
            return new ArrayList<mxPoint>(points.subList(0, Math.min(points.size(), index)));
        } else {
            if (index < points.size()) {
                return new ArrayList<mxPoint>(points.subList(index, points.size()));
            } else {
                return new ArrayList<mxPoint>();
            }
        }
    }

    /** @return the number of points in this link */
    public int getPointCount() {
        if (getGeometry() == null || getGeometry().getPoints() == null) {
            return 0;
        }
        return getGeometry().getPoints().size();
    }

    /**
     * Find the nearest link point of the point
     *
     * @param point
     *            The base point
     * @return the nearest point index in the point list.
     */
    public int findNearestSegment(mxPoint point) {

        if (getGeometry() == null || getGeometry().getPoints() == null) {
            return 0;
        }

        double startX = (getSource().getParent().getGeometry().getX() + getSource().getGeometry().getX());
        double startY = (getSource().getParent().getGeometry().getY() + getSource().getGeometry().getY());

        double endX = (getTarget().getParent().getGeometry().getX() + getTarget().getGeometry().getX());
        double endY = (getTarget().getParent().getGeometry().getY() + getTarget().getGeometry().getY());

        double saveDist = -1;
        int findPos = 0;

        for (int i = 0; i < getGeometry().getPoints().size() + 1; i++) {
            Point2D.Double point1 = null;
            Point2D.Double point2 = null;

            if (i == 0) { // first block
                point1 = new Point2D.Double(startX, startY);
            } else {
                point1 = new Point2D.Double((int) (getGeometry().getPoints().get(i - 1)).getX(), (int) (getGeometry().getPoints().get(i - 1)).getY());
            }

            if (i == getGeometry().getPoints().size()) {
                point2 = new Point2D.Double(endX, endY);
            } else {
                point2 = new Point2D.Double((int) (getGeometry().getPoints().get(i)).getX(), (int) (getGeometry().getPoints().get(i)).getY());
            }

            Point2D.Double addPoint = new Point2D.Double(point.getX(), point.getY());
            Line2D.Double line = new Line2D.Double(point1, point2);

            if (saveDist == -1) {
                saveDist = line.ptSegDist(addPoint);
                findPos = i;
            } else {
                double dist = line.ptSegDist(addPoint);
                if (dist < saveDist) {
                    saveDist = dist;
                    findPos = i;
                }
            }
        }
        return findPos;
    }

    /**
     * Add a point at the position
     *
     * @param x
     *            X coordinate
     * @param y
     *            Y coordinate
     */
    public void addPoint(double x, double y) {
        mxPoint point = new mxPoint(x, y);
        if (getGeometry().getPoints() == null) {
            getGeometry().setPoints(new ArrayList<mxPoint>());
        }
        getGeometry().getPoints().add(point);
    }

    /**
     * Insert point on the nearest link
     *
     * @param point
     *            the point to add
     */
    public void insertPoint(mxPoint point) {

        // if it is a loop link, change coordinate origin to block instead of
        // diagram
        if (isLoopLink()) {
            mxGeometry geo = getSource().getParent().getGeometry();
            point.setX(point.getX() - geo.getX());
            point.setY(point.getY() - geo.getY());
        }

        if (getGeometry() == null) {
            setGeometry(DEFAULT_GEOMETRY);
        }

        if (getGeometry().getPoints() == null) {
            getGeometry().setPoints(new ArrayList<mxPoint>());
            getGeometry().getPoints().add(point);
        } else {
            // check to delete an old point before try to insert
            for (int i = 0; i < getGeometry().getPoints().size(); i++) {
                mxPoint oldPoint = getGeometry().getPoints().get(i);
                mxRectangle rect = new mxRectangle(oldPoint.getX() - (DETECTION_RECTANGLE_DIMENSION / 2),
                                                   oldPoint.getY() - (DETECTION_RECTANGLE_DIMENSION / 2), DETECTION_RECTANGLE_DIMENSION, DETECTION_RECTANGLE_DIMENSION);
                if (rect.contains(point.getX(), point.getY())) {
                    getGeometry().getPoints().remove(i);
                    return;
                }
            }

            int insertPos = findNearestSegment(point);
            getGeometry().getPoints().add(insertPos, point);
        }
    }

    /** @return True if the link is on the same block, false otherwise */
    private boolean isLoopLink() {
        if (getSource() != null && getTarget() != null) {
            if (getSource().getParent() == getParent() && getTarget().getParent() == getParent()) {
                return true;
            }
        }
        return false;
    }

    /** @return The scicos color and type values */
    public abstract double[][] getColorAndType();

    /** the scicos kind */
    public abstract int getLinkKind();

    /**
     * Open the contextual menu of the link
     *
     * @param graph
     *            The associated graph
     */
    public void openContextMenu(ScilabGraph graph) {
        ContextMenu menu = ScilabContextMenu.createContextMenu();
        menu.add(DeleteAction.createMenu(graph));
        /*--- */
        menu.getAsSimpleContextMenu().addSeparator();
        /*--- */
        Menu format = ScilabMenu.createMenu();
        format.setText(XcosMessages.FORMAT);
        format.add(BorderColorAction.createMenu(graph));
        format.add(EditFormatAction.createMenu(graph));
        menu.add(format);
        /*--- */
        menu.getAsSimpleContextMenu().addSeparator();
        /*--- */
        Menu linkStyle = ScilabMenu.createMenu();
        linkStyle.setText(XcosMessages.LINK_STYLE);
        linkStyle.add(StyleHorizontalAction.createMenu(graph));
        linkStyle.add(StyleStraightAction.createMenu(graph));
        linkStyle.add(StyleVerticalAction.createMenu(graph));

        menu.add(linkStyle);

        ((SwingScilabContextMenu) menu.getAsSimpleContextMenu()).setLocation(MouseInfo.getPointerInfo().getLocation().x, MouseInfo.getPointerInfo()
                .getLocation().y);

        menu.setVisible(true);
    }

    /** Invert the source and target of the link */
    public void invertDirection() {
        // invert source and destination and all points.
        mxICell linkSource = getSource();
        mxICell linkTarget = getTarget();
        List<mxPoint> points = getGeometry().getPoints();

        setSource(linkTarget);
        setTarget(linkSource);

        if (points != null) {
            removePoints();
            for (int i = points.size() - 1; i >= 0; i--) {
                addPoint(points.get(i).getX(), points.get(i).getY());
            }
        }
    }

    /*
     * Overriden methods from jgraphx
     */

    /**
     * @return always true
     * @see com.mxgraph.model.mxCell#isConnectable()
     * @see org.scilab.modules.xcos.graph.XcosDiagram#isValidSource(Object)
     */
    @Override
    public boolean isConnectable() {
        return true;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String toString() {
        final StringBuilder str = new StringBuilder();
        final String linkSep = " -> ";

        str.append(source);

        str.append(linkSep);
        if (getChildCount() > 0) {
            // append the label
            str.append(getChildAt(0).getValue());
        } else {
            str.append(getClass().getSimpleName());
        }
        str.append(linkSep);

        str.append(target).append("\n");

        return str.toString();
    }
}
// CSON: ClassDataAbstractionCoupling

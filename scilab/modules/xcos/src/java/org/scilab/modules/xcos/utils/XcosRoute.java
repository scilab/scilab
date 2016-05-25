/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015 - Chenfeng ZHU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.utils;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.SplitBlock;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.link.BasicLink;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.Orientation;
import org.scilab.modules.xcos.port.command.CommandPort;
import org.scilab.modules.xcos.port.control.ControlPort;
import org.scilab.modules.xcos.port.input.InputPort;
import org.scilab.modules.xcos.port.output.OutputPort;

import com.mxgraph.model.mxGeometry;
import com.mxgraph.model.mxGraphModel;
import com.mxgraph.model.mxICell;
import com.mxgraph.util.mxPoint;
import com.mxgraph.view.mxCellState;

public class XcosRoute {

    private List<mxPoint> listRoute = new ArrayList<mxPoint>(0);
    // if it is true, the position of the port will not be changed.
    private boolean lockPortPosition = false;

    public  List<mxPoint> getList() {
        return listRoute;
    }

    /**
     * Update the Edge.
     *
     * @param link
     * @param allCells
     * @param graph
     */
    public void updateRoute(BasicLink link, Object[] allCells, XcosDiagram graph) {
        this.lockPortPosition = false;
        this.updateRoute(link, allCells, graph, lockPortPosition);
    }

    /**
     * Update the Edge.
     *
     * @param link
     * @param allCells
     * @param graph
     * @param lockPort
     *            if it is true, there is no need to calculate the orientation.
     */
    public void updateRoute(BasicLink link, Object[] allCells, XcosDiagram graph, boolean lockPort) {
        this.lockPortPosition = lockPort;
        mxICell sourceCell = link.getSource();
        mxICell targetCell = link.getTarget();
        Object[] allOtherCells = getAllOtherCells(allCells, link, sourceCell, targetCell);
        if (sourceCell != null && targetCell != null) {
            boolean isGetRoute = this.computeRoute(sourceCell, targetCell, allOtherCells, graph);
            if (isGetRoute) {
                List<mxPoint> list = this.getNonRedundantPoints();
                mxGeometry geometry = new mxGeometry();
                geometry.setPoints(list);
                ((mxGraphModel) (graph.getModel())).setGeometry(link, geometry);
                listRoute.clear();
            } else if (!lockPort) {
                // if it cannot get the route, change it to straight.
            }
        }
    }

    /**
     * Get the turning points for the optimal route. If the straight route is the optimal route,
     * return null.
     *
     * @param sourceCell the source port
     * @param targetCell the target port
     * @param allCells
     * @return list of turning points
     */
    protected boolean computeRoute(mxICell sourceCell, mxICell targetCell, Object[] allCells, XcosDiagram graph) {
        listRoute.clear();
        // if the link is not connected with BasicPort.
        if (!(sourceCell instanceof BasicPort) || !(targetCell instanceof BasicPort)) {
            return false;
        }
        Orientation sourcePortOrien = null;
        Orientation targetPortOrien = null;
        double srcx = 0;
        double srcy = 0;
        double tgtx = 0;
        double tgty = 0;
        mxPoint sourcePoint = new mxPoint(srcx, srcy);
        mxPoint targetPoint = new mxPoint(tgtx, tgty);
        // if source is a port, get a new start point.
        if (sourceCell instanceof BasicPort) {
            mxCellState state = graph.getView().getState(sourceCell);
            if (state != null) {
                srcx = state.getCenterX();
                srcy = state.getCenterY();
                BasicPort sourcePort = (BasicPort) sourceCell;
                sourcePortOrien = getPortRelativeOrientation(sourcePort, graph);
            }
        }
        // if target is a port, get a new end point.
        if (targetCell instanceof BasicPort) {
            mxCellState state = graph.getView().getState(targetCell);
            if (state != null) {
                tgtx = state.getCenterX();
                tgty = state.getCenterY();
                BasicPort targetPort = (BasicPort) targetCell;
                targetPortOrien = getPortRelativeOrientation(targetPort, graph);
            }
        }
        // if source belongs to a SplitBlock
        if (sourceCell.getParent() instanceof SplitBlock) {
            srcx = sourceCell.getParent().getGeometry().getCenterX();
            srcy = sourceCell.getParent().getGeometry().getCenterY();
        }
        // if target is a SplitBlock
        if (targetCell.getParent() instanceof SplitBlock) {
            tgtx = targetCell.getParent().getGeometry().getCenterX();
            tgty = targetCell.getParent().getGeometry().getCenterY();
        }
        // if two points are coincident, use straignt route.
        boolean isSplitBlock = (sourceCell.getParent() instanceof SplitBlock) && (targetCell.getParent() instanceof SplitBlock);
        if (XcosRouteUtils.isPointCoincident(srcx, srcy, tgtx, tgty, isSplitBlock)) {
            return true;
        }
        // if two ports are aligned and there are no blocks between them, use straight route.
        if (XcosRouteUtils.isStrictlyAligned(srcx, srcy, tgtx, tgty)
                && !XcosRouteUtils.checkObstacle(srcx, srcy, tgtx, tgty, allCells)
                && !XcosRouteUtils.isOrientationParallel(srcx, srcy, tgtx, tgty, sourcePortOrien, targetPortOrien)) {
            return true;
        }
        // re-calculate the orientation for the SplitBlock.
        if (sourceCell.getParent() instanceof SplitBlock) {
            if (lockPortPosition) { // if it is locked, use the default orientation.
                sourcePortOrien = ((BasicPort) sourceCell).getOrientation();
            } else {
                sourcePortOrien = this.getNewOrientation(sourceCell, srcx, srcy, targetCell, tgtx, tgty, graph);
            }
        }
        if (targetCell.getParent() instanceof SplitBlock) {
            if (lockPortPosition) { // if it is locked, use the default orientation.
                targetPortOrien = ((BasicPort) targetCell).getOrientation();
            } else {
                targetPortOrien = this.getNewOrientation(targetCell, tgtx, tgty, sourceCell, srcx, srcy, graph);
            }
        }
        sourcePoint = getPointAwayPort(sourceCell, srcx, srcy, sourcePortOrien, allCells, graph);
        targetPoint = getPointAwayPort(targetCell, tgtx, tgty, targetPortOrien, allCells, graph);
        allCells = Arrays.copyOf(allCells, allCells.length + 2);
        allCells[allCells.length - 2] = sourceCell;
        allCells[allCells.length - 1] = targetCell;
        List<mxPoint> list = XcosRouteUtils.getSimpleRoute(sourcePoint, sourcePortOrien, targetPoint,
                targetPortOrien, allCells);
        if (list != null && list.size() > 0) {
            listRoute.addAll(list);
            return true;
        } else {
            list = XcosRouteUtils.getComplexRoute(sourcePoint, sourcePortOrien, targetPoint, targetPortOrien,
                    allCells, XcosRouteUtils.TRY_TIMES);
            if (list != null && list.size() > 0) {
                listRoute.addAll(list);
                return true;
            }
        }
        return false;
    }

    /**
     * Remove the redundancy points from the route.
     *
     * @return a list without non-redundant points
     */
    protected List<mxPoint> getNonRedundantPoints() {
        List<mxPoint> list = new ArrayList<mxPoint>(0);
        if (listRoute.size() > 2) {
            list.add(listRoute.get(0));
            for (int i = 1; i < listRoute.size() - 1; i++) {
                mxPoint p1 = list.get(list.size() - 1);
                mxPoint p2 = listRoute.get(i);
                mxPoint p3 = listRoute.get(i + 1);
                if (XcosRouteUtils.pointInLineSegment(p2.getX(), p2.getY(), p1.getX(), p1.getY(), p3.getX(), p3.getY())) {
                    // if p2 is in the line segment between p1 and p3, remove it.
                    continue;
                } else {
                    list.add(p2);
                }
            }
            list.add(listRoute.get(listRoute.size() - 1));
        } else {
            // if the route has less than 3 points, there is no redundancy.
            list.addAll(listRoute);
        }
        return list;
    }

    /**
     * According to the relative position (orientation) of the port, get a point which is
     * XcosRoute.BEAUTY_DISTANCE away from the port and out of block.
     *
     * @param port
     * @param portX
     * @param portY
     * @param graph
     * @return
     */
    private mxPoint getPointAwayPort(mxICell port, double portX, double portY, Orientation orien,
            Object[] allCells, XcosDiagram graph) {
        mxPoint point = new mxPoint(portX, portY);
        double distance = XcosRouteUtils.BEAUTY_AWAY_DISTANCE;
        if (port.getParent() instanceof SplitBlock) {
            distance = XcosRouteUtils.SPLITBLOCK_AWAY_DISTANCE;
        }
        switch (orien) {
        case EAST:
            point.setX(point.getX() + distance);
            while (Math.abs(point.getX() - portX) > XcosRouteUtils.BEAUTY_AWAY_REVISION
                    && (XcosRouteUtils.checkObstacle(portX, portY, point.getX(), point.getY(), allCells)
                            || XcosRouteUtils.checkPointInBlocks(point.getX(), point.getY(), allCells))) {
                point.setX(point.getX() - XcosRouteUtils.BEAUTY_AWAY_REVISION);
            }
            break;
        case SOUTH:
            point.setY(point.getY() + distance);
            while (Math.abs(point.getY() - portY) > XcosRouteUtils.BEAUTY_AWAY_REVISION
                    && (XcosRouteUtils.checkObstacle(portX, portY, point.getX(), point.getY(), allCells)
                            || XcosRouteUtils.checkPointInBlocks(point.getX(), point.getY(), allCells))) {
                point.setY(point.getY() - XcosRouteUtils.BEAUTY_AWAY_REVISION);
            }
            break;
        case WEST:
            point.setX(point.getX() - distance);
            while (Math.abs(point.getX() - portX) > XcosRouteUtils.BEAUTY_AWAY_REVISION
                    && (XcosRouteUtils.checkObstacle(portX, portY, point.getX(), point.getY(), allCells)
                            || XcosRouteUtils.checkPointInBlocks(point.getX(), point.getY(), allCells))) {
                point.setX(point.getX() + XcosRouteUtils.BEAUTY_AWAY_REVISION);
            }
            break;
        case NORTH:
            point.setY(point.getY() - distance);
            while (Math.abs(point.getY() - portY) > XcosRouteUtils.BEAUTY_AWAY_REVISION
                    && (XcosRouteUtils.checkObstacle(portX, portY, point.getX(), point.getY(), allCells)
                            || XcosRouteUtils.checkPointInBlocks(point.getX(), point.getY(), allCells))) {
                point.setY(point.getY() + XcosRouteUtils.BEAUTY_AWAY_REVISION);
            }
            break;
        }
        return point;
    }

    /**
     * As BasicPort.getOrientation is the default orientation, the Orientation is not correct when
     * the block is mirrored or flipped. This method could get the current Orientation of the port.
     *
     * @param port
     * @param graph
     * @return
     */
    private Orientation getPortRelativeOrientation(BasicPort port, XcosDiagram graph) {
        // the coordinate (x,y) for the port.
        double portx = graph.getView().getState(port).getCenterX();
        double porty = graph.getView().getState(port).getCenterY();
        // the coordinate (x,y) and the width-height for the parent block
        mxICell parent = port.getParent();
        double blockx = graph.getView().getState(parent).getCenterX();
        double blocky = graph.getView().getState(parent).getCenterY();
        double blockw = parent.getGeometry().getWidth();
        double blockh = parent.getGeometry().getHeight();
        // calculate relative coordinate based on the center of parent block.
        portx -= blockx;
        porty -= blocky;
        Orientation orientation = port.getOrientation();
        if ((portx) >= blockw * Math.abs(porty) / blockh) { // x>=w*|y|/h
            orientation = Orientation.EAST;
        } else if (porty >= blockh * Math.abs(portx) / blockw) { // y>=h*|x|/w
            orientation = Orientation.SOUTH;
        } else if (portx <= -blockw * Math.abs(porty) / blockh) { // x<=-w*|y|/h
            orientation = Orientation.WEST;
        } else if (porty <= -blockh * Math.abs(portx) / blockw) { // y<=-h*|x|/w
            orientation = Orientation.NORTH;
        }
        return orientation;
    }

    /**
     * Special use to calculate the orientation when
     * <ol>
     * <li>it is a SplitBlock.</li>
     * <li>there is no connection.</li>
     * <ol>
     *
     * @param cell
     * @param cx
     * @param cy
     * @param otherCell
     * @param ox
     * @param oy
     * @param graph
     * @return
     */
    private Orientation getNewOrientation(mxICell cell, double cx, double cy, mxICell otherCell, double ox,
            double oy, XcosDiagram graph) {
        Orientation orientation = Orientation.EAST;
        if (cell.getParent() instanceof SplitBlock) {
            SplitBlock block = (SplitBlock) cell.getParent();
            Orientation inputOrien = this.getSplitBlockInputOrientation(block, graph);
            if ((cell instanceof InputPort) || (cell instanceof ControlPort)) {
                // if it is the InputPort/ControlPort of the SplitBlock,
                orientation = inputOrien;
            } else if ((cell instanceof OutputPort) || (cell instanceof CommandPort)) {
                // if it is one of the OutputPorts/CommandPorts of the SplitBlock,
                // it depends on the Orientation of InputPort and the other OutputPort.
                BasicPort port = (BasicPort) cell;
                BasicPort otherPort = null;
                if (port == block.getOut1()) {
                    otherPort = block.getOut2();
                } else {
                    otherPort = block.getOut1();
                }
                mxICell otherLink = otherPort.getEdgeAt(0);
                if (otherLink instanceof BasicLink) {
                    mxICell otherTarget = ((BasicLink) otherLink).getTarget();
                    mxPoint otgtPoint = this.getCenterPoint(otherTarget, graph);
                    double otx = otgtPoint.getX();
                    double oty = otgtPoint.getY();
                    double otcx = cx - otx;
                    double otcy = cy - oty;
                    double otox = ox - otx;
                    double otoy = oy - oty;
                    double value = (otcx * otoy) - (otox * otcy);
                    if (ox >= cx && oy <= cy) {
                        // when target is on NORTHEAST to source
                        if (value >= 0 && inputOrien != Orientation.NORTH) {
                            // other OutputPort is on the right
                            orientation = Orientation.NORTH;
                        } else if (inputOrien != Orientation.EAST) {
                            // other OutputPort is on the left
                            orientation = Orientation.EAST;
                        } else {
                            orientation = Orientation.NORTH;
                        }
                    } else if (ox >= cx && oy >= cy) {
                        // when target is on SOUTHEAST to source
                        if (value >= 0 && inputOrien != Orientation.EAST) {
                            // other OutputPort is on the right
                            orientation = Orientation.EAST;
                        } else if (inputOrien != Orientation.SOUTH) {
                            // other OutputPort is on the left
                            orientation = Orientation.SOUTH;
                        } else {
                            orientation = Orientation.EAST;
                        }
                    } else if (ox <= cx && oy <= cy) {
                        // when target is on NORTHWEST to source
                        if (value >= 0 && inputOrien != Orientation.WEST) {
                            // other OutputPort is on the right
                            orientation = Orientation.WEST;
                        } else if (inputOrien != Orientation.NORTH) {
                            // other OutputPort is on the left
                            orientation = Orientation.NORTH;
                        } else {
                            orientation = Orientation.WEST;
                        }
                    } else if (ox <= cx && oy >= cy) {
                        // when target is on SOUTHWEST to source
                        if (value >= 0 && inputOrien != Orientation.SOUTH) {
                            // other OutputPort is on the right
                            orientation = Orientation.SOUTH;
                        } else if (inputOrien != Orientation.WEST) {
                            // other OutputPort is on the left
                            orientation = Orientation.WEST;
                        } else {
                            orientation = Orientation.SOUTH;
                        }
                    }
                }
            }
        }
        return orientation;
    }

    /**
     * Get the Orientation of the InputPort of a SplitBlock.
     *
     * @param block
     *            the SplitBlock
     * @return the orientation of its InputPort.
     */
    private Orientation getSplitBlockInputOrientation(SplitBlock block, XcosDiagram graph) {
        Orientation orientation = Orientation.EAST;
        BasicPort inp = block.getIn();
        // the position of the Input
        double inX = block.getGeometry().getCenterX();
        double inY = block.getGeometry().getCenterY();
        mxICell link = inp.getEdgeAt(0);
        // the position of the source
        double srcX = 0;
        double srcY = 0;
        if (link instanceof BasicLink) {
            mxICell source = ((BasicLink) link).getSource();
            mxPoint srcPoint = this.getCenterPoint(source, graph);
            srcX = srcPoint.getX();
            srcY = srcPoint.getY();
        }
        // the relative position of source to Input
        double dx = srcX - inX;
        double dy = srcY - inY;
        if (dx >= Math.abs(dy)) { // x>=|y|
            orientation = Orientation.EAST; // source is on the east of Input.
        } else if (dy >= Math.abs(dx)) { // y>=|x|
            orientation = Orientation.SOUTH;
        } else if (dx <= -Math.abs(dy)) { // x<=-|y|
            orientation = Orientation.WEST;
        } else if (dy <= -Math.abs(dx)) { // y<=-|x|
            orientation = Orientation.NORTH;
        }
        return orientation;
    }

    /**
     * Get the position of a cell.
     *
     * @param cell
     * @return
     */
    private mxPoint getCenterPoint(mxICell cell, XcosDiagram graph) {
        mxPoint point = new mxPoint(0, 0);
        if (cell instanceof BasicPort) {
            BasicPort port = (BasicPort) cell;
            mxICell parent = port.getParent();
            if (parent instanceof SplitBlock) {
                // if it belongs to a SplitBlock, use SplitBlock's geometry.
                point.setX(parent.getGeometry().getCenterX());
                point.setY(parent.getGeometry().getCenterY());
            } else {
                mxCellState state = graph.getView().getState(cell);
                if (state != null) {
                    point.setX(state.getCenterX());
                    point.setY(state.getCenterY());
                }
            }
        } else if (cell instanceof BasicBlock) {
            // if it is a block, use its geometry.
            BasicBlock block = (BasicBlock) cell;
            point.setX(block.getGeometry().getCenterX());
            point.setY(block.getGeometry().getCenterY());
        }
        return point;
    }

    /**
     * Remove the selves from the array of all.<br/>
     * except for selves, SplitBlock and the ports of links in selves. add other ports of blocks.
     *
     * @param all
     * @param self
     *            If self's source port or target port belongs to a SpliBlock, remove this
     *            SplitBlock, too.
     * @return a new array of all objects excluding selves
     */
    protected Object[] getAllOtherCells(Object[] all, Object... self) {
        List<Object> listNotObs = new ArrayList<Object>(0);
        listNotObs.addAll(Arrays.asList(self));
        for (Object obj : self) {
            // if self contains a link
            if (obj instanceof BasicLink) {
                BasicLink link = (BasicLink) obj;
                // in these selves, if the source/target of this link belongs to a SplitBlock,
                // remove it.
                if (link.getSource() != null && link.getSource().getParent() instanceof SplitBlock) {
                    listNotObs.add(link.getSource().getParent());
                }
                if (link.getTarget() != null && link.getTarget().getParent() instanceof SplitBlock) {
                    listNotObs.add(link.getTarget().getParent());
                }
            }
        }
        List<Object> listnew = new ArrayList<Object>(0);
        for (Object o : all) {
            // if it does not belongs to self,
            if (!listNotObs.contains(o) && !(o instanceof SplitBlock)) { // && !(o instanceof TextBlock)
                // only add normal Blocks excluding SplitBlock or TextBlock.
                if (!listnew.contains(o)) {
                    listnew.add(o);
                }
                // add the ports of the block.
                if (o instanceof BasicBlock) {
                    BasicBlock block = (BasicBlock) o;
                    for (int i = 0; i < block.getChildCount(); i++) {
                        if (!listNotObs.contains(block.getChildAt(i))) {
                            listnew.add(block.getChildAt(i));
                        }
                    }
                }
            }
        }
        Object[] newAll = listnew.toArray();
        return newAll;
    }
}

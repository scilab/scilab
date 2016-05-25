/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2016 - Chenfeng ZHU
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
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.xcos.block.SplitBlock;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.graph.swing.handler.SelectionCellsHandler;
import org.scilab.modules.xcos.link.BasicLink;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.Orientation;

import com.mxgraph.model.mxCell;
import com.mxgraph.model.mxGeometry;
import com.mxgraph.model.mxICell;
import com.mxgraph.util.mxConstants;
import com.mxgraph.util.mxPoint;

/**
 * Provide methods to set the new position for SplitBlock.
 */
public abstract class BlockAutoPositionUtils {

    /**
     * Change the position of the SplitBlocks including their links.
     *
     * @param graph
     * @param cells
     *            all selected cells
     */
    public static void changeSplitBlocksPosition(XcosDiagram graph, Object[] cells) {
        Object[] all = graph.getChildCells(graph.getDefaultParent());
        Object[] selectedCells = selectRootSplitBlock(graph, cells);
        for (Object o : selectedCells) {
            if (o instanceof SplitBlock) {
                SplitBlock cell = (SplitBlock) o;
                if (getSplitBlockNumber(cell) == 1) {
                    changeSplitBlockPosition(cell, all, graph);
                } else {
                    changeSplitBlockPositionMulti(cell, all, graph);
                }
            }
        }
    }

    /**
     * Only select the root Split Block.
     *
     * @param graph
     * @param cells
     *            all selected cells
     * @return all the root Split Blocks
     */
    private static Object[] selectRootSplitBlock(XcosDiagram graph, Object[] cells) {
        List<Object> list = new ArrayList<>(0);
        for (Object o : cells) {
            if (o instanceof SplitBlock) {
                SplitBlock cell = getRootSplitBlock((SplitBlock) o);
                if (!list.contains(cell) && !isListContainsCell(list, cell)) {
                    list.add(cell);
                }
            }
        }
        return list.toArray();
    }

    /**
     * Get the first(root) Split Block in the link where the Split Block is.
     *
     * @param splitblock the Split Block
     * @return the first Split Block
     */
    private static SplitBlock getRootSplitBlock(SplitBlock splitblock) {
        mxICell port = getSplitInLinkPort(splitblock);
        while ((port != null) && (port.getParent() instanceof SplitBlock)) {
            port = getSplitInLinkPort(((SplitBlock) port.getParent()));
        }
        mxICell edge = port.getEdgeAt(0);
        mxICell cell = ((mxCell) edge).getTarget();
        return ((SplitBlock) cell.getParent());
    }

    /**
     * Check whether the list contains one split block in the link where the
     * Split Block is.
     *
     * @param list
     *            a list of Cells
     * @param splitBlock
     *            the Split Block
     * @return <b>true</b> if the list contains the split block.
     */
    private static boolean isListContainsCell(List<Object> list, SplitBlock splitBlock) {
        for (Object o : list) {
            if (o instanceof SplitBlock) {
                SplitBlock split = (SplitBlock) o;
                List<mxICell> listSplit = getAllChildrenSplitBlockByLevel(split);
                if (listSplit.contains(splitBlock)) {
                    return true;
                }
            }
        }
        return false;
    }

    /**
     * Get the number of Split Blocks in the link where the Split Block is.
     *
     * @param splitblock
     *            the Split Block
     * @return the number of Split Blocks
     */
    private static int getSplitBlockNumber(SplitBlock splitblock) {
        SplitBlock root = getRootSplitBlock(splitblock);
        List<mxICell> list = getAllChildrenSplitBlockByLevel(root);
        return list.size() + 1;
    }

    /**
     * Change the position of the SplitBlock including its links.
     *
     * @param splitblock
     *            the Split Block
     * @param all
     *            all cells in graph
     * @param graph
     */
    protected static void changeSplitBlockPosition(SplitBlock splitblock, Object[] all, XcosDiagram graph) {
        mxICell sourcePort = getSplitInLinkPort(splitblock);
        mxICell targetPort1 = getSplitLinkPort(splitblock, splitblock.getOut1());
        mxICell targetPort2 = getSplitLinkPort(splitblock, splitblock.getOut2());

        // get the optimal routes for 2 target ports.
        Object[] allObstacles = getObstacles(splitblock, all);
        List<mxPoint> list1 = getRoute(splitblock, sourcePort, targetPort1, allObstacles, graph);
        List<mxPoint> list2 = getRoute(splitblock, sourcePort, targetPort2, allObstacles, graph);

        // adjust the routes.
        List<mxICell> listPorts = new ArrayList<>(0);
        listPorts.add(sourcePort);
        listPorts.add(targetPort1);
        listPorts.add(targetPort2);
        adjustRoutes(list1, list2, allObstacles, listPorts);

        // get the position according to the routes.
        mxPoint point = getSplitPoint(list1, list2);
        if (point == null) {
            // keep it in the original position.
            return;
        }
        updatePortOrientation(splitblock, list1, list2, point);

        // update split block's position and update the corresponding link.
        mxGeometry splitGeo = (mxGeometry) graph.getModel().getGeometry(splitblock).clone();
        splitGeo.setX(point.getX() - splitGeo.getWidth() / 2);
        splitGeo.setY(point.getY() - splitGeo.getHeight() / 2);
        graph.getModel().setGeometry(splitblock, splitGeo);
        updateSplitLink(splitblock, allObstacles, graph);
    }

    /**
     * Change the position for multiple SplitBlocks including their links.
     *
     * @param splitblock
     *            the Split Block
     * @param all
     *            all cells in graph
     * @param graph
     */
    protected static void changeSplitBlockPositionMulti(SplitBlock splitblock, Object[] all, XcosDiagram graph) {
        adjustSplitBlock(splitblock);
        mxICell sourcePort = getSplitInLinkPort(splitblock);
        List<mxICell> listTargetPorts = getSplitAllTargetPort(splitblock);
        List<mxICell> listSplitBlocks = new ArrayList<>(0);
        listSplitBlocks.add(splitblock);
        listSplitBlocks.addAll(getAllChildrenSplitBlockByLevel(splitblock));

        // get all optimal routes for source to each target including the source and target.
        Map<mxICell, List<mxPoint>> mapRoutes = new HashMap<>(0);
        List<List<mxPoint>> listRoutes = new ArrayList<>(0);
        Object[] allObstacles = getObstacles(splitblock, all);
        XcosRoute util = new XcosRoute();
        for (mxICell targetPort : listTargetPorts) {
            List<mxPoint> list = new ArrayList<mxPoint>(0);
            if (sourcePort != null) {
                list.add(getPortPosition(sourcePort));
            }
            boolean flag = util.computeRoute(sourcePort, targetPort, allObstacles, graph);
            if (flag) {
                for (mxPoint point : util.getNonRedundantPoints()) {
                    // add all points in the route
                    list.add(new mxPoint(Math.round(point.getX()), Math.round(point.getY())));
                }
            } else {
                // keep it in the original position.
                return;
            }
            if (targetPort != null) {
                list.add(getPortPosition(targetPort));
            }
            mapRoutes.put(targetPort, list);
            listRoutes.add(list);
        }

        // adjust the routes.
        List<mxICell> listPorts = new ArrayList<>(0);
        listPorts.add(sourcePort);
        listPorts.addAll(listTargetPorts);
        adjustRoutes(listRoutes, allObstacles, listPorts);

        // set the new position for each Split Block according to all existing
        // optimal routes.
        for (int i = 0; i < listSplitBlocks.size(); i++) {
            mxICell cell = listSplitBlocks.get(i);
            SplitBlock split = (SplitBlock) cell;
            List<mxICell> listTargets = null;
            if (i == 0) {
                listTargets = getSplitAllTargetPort(split);
            } else {
                for (int j = 0; j < i; j++) {
                    SplitBlock temp = (SplitBlock) listSplitBlocks.get(j);
                    // get its connected previous split block.
                    if (isSplitBlocksConnected(split, temp) != null) {
                        listTargets = getSplitAllTargetPort(split, temp);
                        break;
                    }
                }
            }
            List<List<mxPoint>> listTargetRoutes = new ArrayList<>(0);
            for (mxICell target : listTargets) {
                listTargetRoutes.add(mapRoutes.get(target));
            }
            mxPoint splitPoint = getSplitPoint(listTargetRoutes);
            if (splitPoint == null) {
                // keep it in the original position.
                return;
            }
            mxGeometry splitGeo = (mxGeometry) graph.getModel().getGeometry(split).clone();
            splitGeo.setX(splitPoint.getX() - splitGeo.getWidth() / 2);
            splitGeo.setY(splitPoint.getY() - splitGeo.getHeight() / 2);
            graph.getModel().setGeometry(split, splitGeo);
        }

        // update the orientation for all ports of each Split Block based on
        // corresponding routes and position.
        for (int i = 0; i < listSplitBlocks.size(); i++) {
            mxICell cell = listSplitBlocks.get(i);
            SplitBlock split = (SplitBlock) cell;
            List<mxICell> listTargets = null;
            BasicPort inPort = null;
            if (i == 0) {
                listTargets = getSplitAllTargetPort(split);
            } else {
                for (int j = 0; j < i; j++) {
                    SplitBlock temp = (SplitBlock) listSplitBlocks.get(j);
                    // get its previous split block.
                    if (isSplitBlocksConnected(split, temp) != null) {
                        listTargets = getSplitAllTargetPort(split, temp);
                        inPort = isSplitBlocksConnected(split, temp);
                        break;
                    }
                }
            }
            List<List<mxPoint>> listTargetRoutes = new ArrayList<>(0);
            for (mxICell target : listTargets) {
                listTargetRoutes.add(mapRoutes.get(target));
            }
            updatePortOrientation(split, listTargetRoutes, graph, inPort);
        }

        // update the relative links
        for (mxICell cell : listSplitBlocks) {
            SplitBlock split = (SplitBlock) cell;
            updateSplitLink(split, allObstacles, graph);
        }
    }

    /**
     * Get the linked Port of a SplitBlock according to its Input.
     *
     * @param splitblock
     *            the Split Block
     * @return
     */
    private static mxICell getSplitInLinkPort(SplitBlock splitblock) {
        mxICell cell = null;
        BasicPort in = splitblock.getIn();
        mxICell edge = in.getEdgeAt(0);
        if (edge != null && edge instanceof mxCell) {
            cell = ((mxCell) edge).getSource();
            // sometimes the input port of a split block might be source instead of a target.
            if (cell == in) {
                cell = ((mxCell) edge).getTarget();
            }
        }
        return cell;
    }

    /**
     * Get the linked Port of a SplitBlock according to its Port.
     *
     * @param splitblock
     *            the Split Block
     * @param port
     *            the port in the Split Block
     * @return
     */
    private static mxICell getSplitLinkPort(SplitBlock splitblock, BasicPort port) {
        mxICell cell = null;
        mxICell edge = port.getEdgeAt(0);
        if (edge != null && edge instanceof mxCell) {
            cell = ((mxCell) edge).getTarget();
            if (cell == port) {
                cell = ((mxCell) edge).getSource();
            }
        }
        return cell;
    }

    /**
     * Check whether two split blocks are connected.
     *
     * @param split1
     *            Split Block 1
     * @param split2
     *            Split Block 2
     * @return the connected port of split1
     */
    private static BasicPort isSplitBlocksConnected(SplitBlock split1, SplitBlock split2) {
        BasicPort in1 = split1.getIn();
        BasicPort out11 = split1.getOut1();
        BasicPort out12 = split1.getOut2();
        BasicPort in2 = split2.getIn();
        BasicPort out21 = split2.getOut1();
        BasicPort out22 = split2.getOut2();
        if ((in1.getEdgeAt(0) == in2.getEdgeAt(0))
                || (in1.getEdgeAt(0) == out21.getEdgeAt(0))
                || (in1.getEdgeAt(0) == out22.getEdgeAt(0))) {
            return in1;
        } else if ((out11.getEdgeAt(0) == in2.getEdgeAt(0))
                || (out11.getEdgeAt(0) == out21.getEdgeAt(0))
                || (out11.getEdgeAt(0) == out22.getEdgeAt(0))) {
            return out11;
        } else if ((out12.getEdgeAt(0) == in2.getEdgeAt(0))
                || (out12.getEdgeAt(0) == out21.getEdgeAt(0))
                || (out12.getEdgeAt(0) == out22.getEdgeAt(0))) {
            return out12;
        }
        return null;
    }

    /**
     * See {@link #getSplitAllTargetPort(SplitBlock, SplitBlock)}.
     *
     * @param splitblock
     *            the Split Block
     * @return
     */
    private static List<mxICell> getSplitAllTargetPort(SplitBlock splitblock) {
        return getSplitAllTargetPort(splitblock, null);
    }

    /**
     * Get all the final target Ports of a root SplitBlock. Add them in an order
     * based on split blocks' order.
     *
     * @param splitblock
     *            the Split Block
     * @param previous
     *            the previous Split Block
     * @return
     */
    private static List<mxICell> getSplitAllTargetPort(SplitBlock splitblock, SplitBlock previous) {
        List<mxICell> list = new ArrayList<>(0);
        BasicPort out1 = splitblock.getOut1();
        BasicPort out2 = splitblock.getOut2();
        mxICell sourcePort = getSplitInLinkPort(splitblock);
        mxICell targetPort1 = getSplitLinkPort(splitblock, out1);
        mxICell targetPort2 = getSplitLinkPort(splitblock, out2);

        // if its linked block was a normal block, add it firstly
        if (previous != null && !(sourcePort.getParent() instanceof SplitBlock)) {
            list.add(sourcePort);
        }
        if (!(targetPort1.getParent() instanceof SplitBlock)) {
            list.add(targetPort1);
        }
        if (!(targetPort2.getParent() instanceof SplitBlock)) {
            list.add(targetPort2);
        }

        // if it was a split block, add its final targets.
        if (previous != null && sourcePort.getParent() instanceof SplitBlock && previous != sourcePort.getParent()) {
            list.addAll(getSplitAllTargetPort((SplitBlock) sourcePort.getParent(), splitblock));
        }
        if (targetPort1.getParent() instanceof SplitBlock && previous != targetPort1.getParent()) {
            list.addAll(getSplitAllTargetPort((SplitBlock) targetPort1.getParent(), splitblock));
        }
        if (targetPort2.getParent() instanceof SplitBlock && previous != targetPort2.getParent()) {
            list.addAll(getSplitAllTargetPort((SplitBlock) targetPort2.getParent(), splitblock));
        }

        return list;
    }

    /**
     * See {@link #getAllChildrenSplitBlockByLevel(SplitBlock, List)}
     *
     * @param splitblock
     *            the Split Block
     * @return
     */
    private static List<mxICell> getAllChildrenSplitBlockByLevel(SplitBlock splitblock) {
        return getAllChildrenSplitBlockByLevel(splitblock, null);
    }

    /**
     * Get all the children Split Blocks of a Split Block excluding itself. Add
     * them in an order according to their level.
     *
     * @param splitblock
     *            the Split Block
     * @param previous
     *            the previous Split Block
     * @return
     */
    private static List<mxICell> getAllChildrenSplitBlockByLevel(SplitBlock splitblock, SplitBlock previous) {
        List<mxICell> listCells = new ArrayList<>(0);
        BasicPort out1 = splitblock.getOut1();
        BasicPort out2 = splitblock.getOut2();
        mxICell sourcePort = getSplitInLinkPort(splitblock);
        mxICell targetPort1 = getSplitLinkPort(splitblock, out1);
        mxICell targetPort2 = getSplitLinkPort(splitblock, out2);

        // if its connected block was a Split Block, add these Split Blocks in this level.
        if ((sourcePort.getParent() instanceof SplitBlock) && (previous != sourcePort.getParent())) {
            listCells.add(sourcePort.getParent());
        }
        if ((targetPort1.getParent() instanceof SplitBlock) && (previous != targetPort1.getParent())) {
            listCells.add(targetPort1.getParent());
        }
        if ((targetPort2.getParent() instanceof SplitBlock) && (previous != targetPort2.getParent())) {
            listCells.add(targetPort2.getParent());
        }

        // then add its children.
        if ((sourcePort.getParent() instanceof SplitBlock) && (previous != sourcePort.getParent())) {
            listCells.addAll(getAllChildrenSplitBlockByLevel((SplitBlock) sourcePort.getParent(), splitblock));
        }
        if ((targetPort1.getParent() instanceof SplitBlock) && (previous != targetPort1.getParent())) {
            listCells.addAll(getAllChildrenSplitBlockByLevel((SplitBlock) targetPort1.getParent(), splitblock));
        }
        if ((targetPort2.getParent() instanceof SplitBlock) && (previous != targetPort2.getParent())) {
            listCells.addAll(getAllChildrenSplitBlockByLevel((SplitBlock) targetPort2.getParent(), splitblock));
        }

        return listCells;
    }

    /**
     * Get all links on this Split Block.<br/>
     * Each Split Block has an IN link and two OUT links which would be also the
     * IN link for its child split block.
     *
     * @param splitblock
     *            the Split Block
     * @return
     */
    private static List<mxICell> getAllLinksOnSplitBlock(SplitBlock splitblock) {
        List<mxICell> listLinks = new ArrayList<>(0);

        // get all split blocks in this link.
        List<mxICell> listSplitBlocks = new ArrayList<>(0);
        listSplitBlocks.add(splitblock);
        listSplitBlocks.addAll(getAllChildrenSplitBlockByLevel(splitblock));

        // add the 3 links of each split block without duplication.
        for (mxICell block : listSplitBlocks) {
            SplitBlock split = (SplitBlock) block;
            mxICell link1 = split.getIn().getEdgeAt(0);
            mxICell link2 = split.getOut1().getEdgeAt(0);
            mxICell link3 = split.getOut2().getEdgeAt(0);
            if (!listLinks.contains(link1)) {
                listLinks.add(link1);
            }
            if (!listLinks.contains(link2)) {
                listLinks.add(link2);
            }
            if (!listLinks.contains(link3)) {
                listLinks.add(link3);
            }
        }
        return listLinks;
    }

    /**
     * Adjust the Blocks aligned linked to the split block. Only let SplitBlock
     * aligned to normal Block.
     *
     * @param splitblock
     *            the Split Block
     */
    private static void adjustSplitBlock(SplitBlock splitblock) {
        BasicPort out1 = splitblock.getOut1();
        BasicPort out2 = splitblock.getOut2();
        mxICell sourcePort = getSplitInLinkPort(splitblock);
        mxICell targetPort1 = getSplitLinkPort(splitblock, out1);
        mxICell targetPort2 = getSplitLinkPort(splitblock, out2);
        if (sourcePort.getParent() instanceof SplitBlock) {
            // if it is a Split Block
            if (!(targetPort1.getParent() instanceof SplitBlock)) {
                adjustCell(sourcePort, targetPort1);
            }
            if (!(targetPort2.getParent() instanceof SplitBlock)) {
                adjustCell(sourcePort, targetPort2);
            }
        }
        if (targetPort1 instanceof SplitBlock) {
            if (!(sourcePort.getParent() instanceof SplitBlock)) {
                adjustCell(targetPort1, sourcePort);
            }
        }
        if (targetPort2 instanceof SplitBlock) {
            if (!(sourcePort.getParent() instanceof SplitBlock)) {
                adjustCell(targetPort2, sourcePort);
            }
        }
    }

    /**
     * Adjust the cell position align to the base one only if their difference
     * are less than XcosRouteUtils.ALIGN_STRICT_ERROR.
     *
     * @param cell
     *            the cell should be moved
     * @param cellBase
     *            the based cell to be aligned to
     */
    private static void adjustCell(mxICell cell, mxICell cellBase) {
        double error = XcosRouteUtils.ALIGN_STRICT_ERROR;
        mxPoint cellPoint = getPortPosition(cell);
        mxGeometry cellGeo = cell.getParent().getGeometry();
        mxPoint cellBasePoint = getPortPosition(cellBase);
        if (Math.abs(cellPoint.getX() - cellBasePoint.getX()) <= error) {
            cellGeo.setX(cellBasePoint.getX() - cellGeo.getWidth() / 2);
        }
        if (Math.abs(cellPoint.getY() - cellBasePoint.getY()) <= error) {
            cellGeo.setY(cellBasePoint.getY() - cellGeo.getHeight() / 2);
        }
    }

    /**
     * Adjust the optimal routes and make them aligned with each other when they
     * are parallel.
     *
     * @param list1
     *            the first route
     * @param list2
     *            the second route
     * @param allObstacles
     *            all obstacles
     */
    private static void adjustRoutes(List<mxPoint> list1, List<mxPoint> list2, Object[] allObstacles, List<mxICell> listPorts) {
        List<List<mxPoint>> listRoutes = new ArrayList<>(0);
        listRoutes.add(list1);
        listRoutes.add(list2);
        adjustRoutes(listRoutes, allObstacles, listPorts);
    }

    /**
     * Adjust the optimal routes and make them aligned with each other when they
     * are parallel.
     *
     * @param listRoutes
     *            a list of all routes
     * @param allObstacles
     *            all obstacles
     * @param listPorts
     *            all the source and target ports
     */
    private static void adjustRoutes(List<List<mxPoint>> listRoutes, Object[] allObstacles, List<mxICell> listPorts) {
        // the geometry of all ports.
        List<mxGeometry> listGeo = new ArrayList<>(0);
        for (mxICell port : listPorts) {
            mxGeometry geometry = getPortGeometry(port);
            listGeo.add(geometry);
        }

        // compare with every 2 routes. in every routes, there are several segments.
        // the first two loop is to compare 2 routes
        // the next two loops is to compare every segments in 2 routes
        for (int i = 0; i < listRoutes.size() - 1; i++) {
            List<mxPoint> list1 = listRoutes.get(i);
            for (int j = i + 1; j < listRoutes.size(); j++) {
                List<mxPoint> list2 = listRoutes.get(j);
                for (int m = 0; m < list1.size() - 1; m++) {
                    mxPoint p11 = list1.get(m);
                    mxPoint p12 = list1.get(m + 1);
                    double x11 = p11.getX();
                    double y11 = p11.getY();
                    double x12 = p12.getX();
                    double y12 = p12.getY();
                    for (int n = 0; n < list2.size() - 1; n++) {

                        // if both are source/target, two lines are not movable.
                        if ((m == 0 && n == 0)
                                || ((m == list1.size() - 2) && (n == list2.size() - 2))
                                || (m == 0 && (n == list2.size() - 2))
                                || (n == 0 && (m == list1.size() - 2))) {
                            continue;
                        }

                        mxPoint p21 = list2.get(n);
                        mxPoint p22 = list2.get(n + 1);
                        double x21 = p21.getX();
                        double y21 = p21.getY();
                        double x22 = p22.getX();
                        double y22 = p22.getY();
                        // if they are already aligned or they are not parallel,
                        if ((x11 == x12 && x21 == x22 && x11 == x21)
                                || (y11 == y12 && y21 == y22 && y11 == y21)
                                || !(XcosRouteUtils.isLineParallel(x11, y11, x12, y12, x21, y21, x22, y22, true))) {
                            continue;
                        }

                        if (m == 0 || m == list1.size() - 2) {
                            // if it is source point or target, line1 is not movable.
                            mxPoint p20 = list2.get(n - 1);
                            mxPoint p23 = list2.get(n + 2);
                            if (x11 == x12) { // the segment is vertical
                                boolean flag2 = !XcosRouteUtils.checkObstacle(p20.getX(), p20.getY(), x11, y21, allObstacles)
                                        && !XcosRouteUtils.checkObstacle(x11, y21, x11, y22, allObstacles)
                                        && !XcosRouteUtils.checkObstacle(x11, y22, p23.getX(), p23.getY(), allObstacles);
                                // if the new points of the segment was in one of the ports
                                for (mxGeometry geometry : listGeo) {
                                    if (XcosRouteUtils.checkPointInGeometry(x11, y21, geometry)
                                            || XcosRouteUtils.checkPointInGeometry(x11, y22, geometry)) {
                                        flag2 = false;
                                        break;
                                    }
                                }
                                if (flag2) {
                                    p21.setX(x11);
                                    p22.setX(x11);
                                }
                            } else if (y11 == y12) { // the segment is horizontal
                                boolean flag2 = !XcosRouteUtils.checkObstacle(p20.getX(), p20.getY(), x21, y11, allObstacles)
                                        && !XcosRouteUtils.checkObstacle(x21, y11, x22, y11, allObstacles)
                                        && !XcosRouteUtils.checkObstacle(x22, y11, p23.getX(), p23.getY(), allObstacles);
                                if (flag2) {
                                    p21.setY(y11);
                                    p22.setY(y11);
                                }
                            }
                        } else if (n == 0 || n == list2.size() - 2) {
                            // if it is source point or target, line2 is not movable.
                            mxPoint p10 = list1.get(m - 1);
                            mxPoint p13 = list1.get(m + 2);
                            if (x11 == x12) { // the segment is vertical
                                boolean flag1 = !XcosRouteUtils.checkObstacle(p10.getX(), p10.getY(), x21, y11, allObstacles)
                                        && !XcosRouteUtils.checkObstacle(x21, y11, x21, y12, allObstacles)
                                        && !XcosRouteUtils.checkObstacle(x21, y12, p13.getX(), p13.getY(), allObstacles);
                                for (mxGeometry geometry : listGeo) {
                                    if (XcosRouteUtils.checkPointInGeometry(x21, y11, geometry)
                                            || XcosRouteUtils.checkPointInGeometry(x21, y12, geometry)) {
                                        flag1 = false;
                                        break;
                                    }
                                }
                                if (flag1) {
                                    p11.setX(x21);
                                    p12.setX(x21);
                                }
                            } else if (y11 == y12) { // the segment is horizontal
                                boolean flag1 = !XcosRouteUtils.checkObstacle(p10.getX(), p10.getY(), x11, y21, allObstacles)
                                        && !XcosRouteUtils.checkObstacle(x11, y21, x12, y21, allObstacles)
                                        && !XcosRouteUtils.checkObstacle(x12, y21, p13.getX(), p13.getY(), allObstacles);
                                if (flag1) {
                                    p11.setY(y21);
                                    p12.setY(y21);
                                }
                            }
                        } else {
                            // both are movable.
                            mxPoint p20 = list2.get(n - 1);
                            mxPoint p23 = list2.get(n + 2);
                            mxPoint p10 = list1.get(m - 1);
                            mxPoint p13 = list1.get(m + 2);
                            if (x11 == x12) { // the segment is vertical
                                boolean flag2 = !XcosRouteUtils.checkObstacle(p20.getX(), p20.getY(), x11, y21, allObstacles)
                                        && !XcosRouteUtils.checkObstacle(x11, y21, x11, y22, allObstacles)
                                        && !XcosRouteUtils.checkObstacle(x11, y22, p23.getX(), p23.getY(), allObstacles);
                                boolean flag1 = !XcosRouteUtils.checkObstacle(p10.getX(), p10.getY(), x21, y11, allObstacles)
                                        && !XcosRouteUtils.checkObstacle(x21, y11, x21, y12, allObstacles)
                                        && !XcosRouteUtils.checkObstacle(x21, y12, p13.getX(), p13.getY(), allObstacles);
                                for (mxGeometry geometry : listGeo) {
                                    if (XcosRouteUtils.checkPointInGeometry(x11, y21, geometry)
                                            || XcosRouteUtils.checkPointInGeometry(x11, y22, geometry)) {
                                        flag2 = false;
                                    }
                                    if (XcosRouteUtils.checkPointInGeometry(x21, y11, geometry)
                                            || XcosRouteUtils.checkPointInGeometry(x21, y12, geometry)) {
                                        flag1 = false;
                                    }
                                    if (!flag1 && !flag2) {
                                        break;
                                    }
                                }
                                if (flag2) {
                                    p21.setX(x11);
                                    p22.setX(x11);
                                } else if (flag1) {
                                    p11.setX(x21);
                                    p12.setX(x21);
                                }
                            } else if (y11 == y12) { // the segment is horizontal
                                boolean flag2 = !XcosRouteUtils.checkObstacle(p20.getX(), p20.getY(), x21, y11, allObstacles)
                                        && !XcosRouteUtils.checkObstacle(x21, y11, x22, y11, allObstacles)
                                        && !XcosRouteUtils.checkObstacle(x22, y11, p23.getX(), p23.getY(), allObstacles);
                                boolean flag1 = !XcosRouteUtils.checkObstacle(p10.getX(), p10.getY(), x11, y21, allObstacles)
                                        && !XcosRouteUtils.checkObstacle(x11, y21, x12, y21, allObstacles)
                                        && !XcosRouteUtils.checkObstacle(x12, y21, p13.getX(), p13.getY(), allObstacles);
                                for (mxGeometry geometry : listGeo) {
                                    if (XcosRouteUtils.checkPointInGeometry(x21, y11, geometry)
                                            || XcosRouteUtils.checkPointInGeometry(x22, y11, geometry)) {
                                        flag2 = false;
                                    }
                                    if (XcosRouteUtils.checkPointInGeometry(x11, y21, geometry)
                                            || XcosRouteUtils.checkPointInGeometry(x12, y21, geometry)) {
                                        flag1 = false;
                                    }
                                    if (!flag1 && !flag2) {
                                        break;
                                    }
                                }
                                if (flag2) {
                                    p21.setY(y11);
                                    p22.setY(y11);
                                } else if (flag1) {
                                    p11.setY(y21);
                                    p12.setY(y21);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    /**
     * Get the route for the source and the target ignoring the SplitBlock.
     *
     * @param source
     *            the source port
     * @param target
     *            the target port
     * @param all
     *            all cells in graph
     * @param graph
     * @return all turning points in a route including the start and end points
     */
    private static List<mxPoint> getRoute(SplitBlock splitblock, mxICell source, mxICell target, Object[] all, XcosDiagram graph) {
        XcosRoute util = new XcosRoute();
        // get all obstacles, excluding splitblock itself or its relative link.
        mxICell link1 = splitblock.getIn().getEdgeAt(0);
        mxICell link2 = splitblock.getOut1().getEdgeAt(0);
        mxICell link3 = splitblock.getOut2().getEdgeAt(0);
        Object[] allOtherCells = util.getAllOtherCells(all, source, target, source.getEdgeAt(0), target.getEdgeAt(0), link1, link2, link3);

        // get the route and add all points in the route including source and target.
        List<mxPoint> list = new ArrayList<mxPoint>(0);
        if (source != null) {
            list.add(getPortPosition(source));
        }
        if (util.computeRoute(source, target, allOtherCells, graph)) {
            for (mxPoint point : util.getNonRedundantPoints()) {
                list.add(new mxPoint(Math.round(point.getX()), Math.round(point.getY())));
            }
        }
        if (target != null) {
            list.add(getPortPosition(target));
        }
        return list;
    }

    /**
     * Get all obstacle blocks.
     *
     * @param splitblock
     *            the split block
     * @param all
     *            all cells in graph
     * @return
     */
    private static Object[] getObstacles(SplitBlock splitblock, Object[] all) {
        List<mxICell> listTargetPorts = getSplitAllTargetPort(splitblock);

        // get all split blocks
        List<mxICell> listSplitBlocks = new ArrayList<>(0);
        listSplitBlocks.add(splitblock);
        listSplitBlocks.addAll(getAllChildrenSplitBlockByLevel(splitblock));

        // all split blocks, all relative links and it source port and all
        // target ports should not be considered as obstacles.
        List<mxICell> listLinks = getAllLinksOnSplitBlock(splitblock);
        List<mxICell> listNotObstable = new ArrayList<>(0);
        listNotObstable.add(getSplitInLinkPort(splitblock));
        listNotObstable.addAll(listTargetPorts);
        listNotObstable.addAll(listSplitBlocks);
        listNotObstable.addAll(listLinks);
        Object[] notObstacles = listNotObstable.toArray();

        XcosRoute util = new XcosRoute();
        Object[] allObstacles = util.getAllOtherCells(all, notObstacles);
        return allObstacles;
    }

    /**
     * Get a port's geometry.
     *
     * @param port
     *            the port
     * @return
     */
    private static mxGeometry getPortGeometry(mxICell port) {
        mxGeometry geometry = new mxGeometry();
        if (port == null) {
            return null;
        }

        if (port.getParent() instanceof SplitBlock) {
            // if the port belongs to a split block
            SplitBlock cell = (SplitBlock) port.getParent();
            geometry.setX(cell.getGeometry().getX());
            geometry.setY(cell.getGeometry().getY());
            geometry.setWidth(cell.getGeometry().getWidth());
            geometry.setHeight(cell.getGeometry().getHeight());
        } else {
            // if the port belongs to a normal block
            mxGeometry portGeo = port.getGeometry();
            double portX = portGeo.getX();
            double portY = portGeo.getY();
            mxICell parent = port.getParent();
            mxGeometry parentGeo = parent.getGeometry();
            if (portGeo.isRelative()) {
                portX *= parentGeo.getWidth();
                portY *= parentGeo.getHeight();
            }
            geometry.setX(parentGeo.getX() + portX);
            geometry.setY(parentGeo.getY() + portY);
            geometry.setWidth(portGeo.getWidth());
            geometry.setHeight(portGeo.getHeight());
        }
        return geometry;
    }


    /**
     * Get the position of a port.
     *
     * @param port
     *            the port
     * @return
     */
    private static mxPoint getPortPosition(mxICell port) {
        mxPoint point = new mxPoint();
        if (port == null) {
            return null;
        }

        if (port.getParent() instanceof SplitBlock) {
            // if the port belongs to a split block, position is the center of the split block,
            SplitBlock cell = (SplitBlock) port.getParent();
            point.setX(cell.getGeometry().getCenterX());
            point.setY(cell.getGeometry().getCenterY());
        } else {
            // if the port belongs to a normal block, the position is relative to its parent.
            mxGeometry portGeo = port.getGeometry();
            double portX = portGeo.getX();
            double portY = portGeo.getY();
            double portW = portGeo.getWidth();
            double portH = portGeo.getHeight();
            mxICell parent = port.getParent();
            mxGeometry parentGeo = parent.getGeometry();
            if (portGeo.isRelative()) {
                portX *= parentGeo.getWidth();
                portY *= parentGeo.getHeight();
            }
            point.setX(parentGeo.getX() + portX + portW / 2);
            point.setY(parentGeo.getY() + portY + portH / 2);
        }

        // update the point.
        point.setX(Math.round(point.getX()));
        point.setY(Math.round(point.getY()));
        return point;
    }

    /**
     * Get the split point for two routes.
     *
     * @param list1
     *            the first route
     * @param list2
     *            the second route
     * @return
     */
    private static mxPoint getSplitPoint(List<mxPoint> list1, List<mxPoint> list2) {
        mxPoint point = null;
        int num = Math.min(list1.size(), list2.size());
        if (num <= 1 || !list1.get(0).equals(list2.get(0))) {
            return null;
        }

        // check the last intersection of two links
        int iList1 = 1;
        int iList2 = 1;
        for (int i = iList1; i < list1.size(); i++) {
            for (int j = iList2; j < list2.size(); j++) {
                mxPoint p1 = list1.get(i - 1);
                mxPoint p2 = list1.get(i);
                mxPoint p3 = list2.get(j - 1);
                mxPoint p4 = list2.get(j);
                mxPoint p0 = XcosRouteUtils.getIntersection(p1.getX(), p1.getY(), p2.getX(), p2.getY(), p3.getX(), p3.getY(), p4.getX(), p4.getY());
                if (p0 != null) {
                    iList1 = i;
                    iList2 = j;
                    point = (mxPoint) p0.clone();
                }
            }
        }
        return point;
    }

    /**
     * Get the split point for multiple routes.
     *
     * @param listRoutes
     *            the list of routes
     * @return
     */
    private static mxPoint getSplitPoint(List<List<mxPoint>> listRoutes) {
        List<mxPoint> listAllSplitPoints = new ArrayList<>(0);

        // get all intersections of every 2 routes.
        for (int i = 0; i < listRoutes.size() - 1; i++) {
            List<mxPoint> list1 = listRoutes.get(i);
            for (int j = i + 1; j < listRoutes.size(); j++) {
                List<mxPoint> list2 = listRoutes.get(j);
                mxPoint point = getSplitPoint(list1, list2);
                if (point == null || listAllSplitPoints.contains(point)) {
                    continue;
                }
                listAllSplitPoints.add(point);
            }
        }

        // choose the one point which every route will go through.
        mxPoint splitPoint = null;
        for (mxPoint point : listAllSplitPoints) {
            double x = point.getX();
            double y = point.getY();
            for (int i = 0; i < listRoutes.size(); i++) {
                if (!XcosRouteUtils.pointInLink(x, y, listRoutes.get(i))) {
                    // if there is one route doesn't go through the point
                    break;
                }
                if (i == listRoutes.size() - 1) {
                    splitPoint = new mxPoint();
                    splitPoint.setX(x);
                    splitPoint.setY(y);
                }
            }
        }
        return splitPoint;
    }

    /**
     * Update port orientation.
     *
     * @param splitblock
     *            the Split Block
     * @param list1
     *            the first optimal route
     * @param list2
     *            the second optimal route
     * @param splitPoint
     *            the position for the split block
     */
    private static void updatePortOrientation(SplitBlock splitblock, List<mxPoint> list1, List<mxPoint> list2, mxPoint splitPoint) {
        Orientation orientationIn = getInportOrientation(list1, list2, splitPoint);
        if (orientationIn != null) {
            splitblock.getIn().setOrientation(orientationIn);
        }
        Orientation orientationOut1 = getPortOrientation(list1, splitPoint);
        if (orientationOut1 != null) {
            splitblock.getOut1().setOrientation(orientationOut1);
        }
        Orientation orientationOut2 = getPortOrientation(list2, splitPoint);
        if (orientationOut2 != null) {
            splitblock.getOut2().setOrientation(orientationOut2);
        }
    }

    /**
     * Get the orientation for the Input Port of a Split Block.<br/>
     * See {@link #getInputOrientation(List, mxPoint, mxPoint)}.
     *
     * @param list1
     *            the first optimal route including start Port and end Port
     * @param list2
     *            the second optimal route including start Port and end Port
     * @param splitPoint
     *            the new position for the Split Block
     * @return
     */
    private static Orientation getInportOrientation(List<mxPoint> list1, List<mxPoint> list2, mxPoint splitPoint) {
        List<List<mxPoint>> list = new ArrayList<>(0);
        list.add(list1);
        list.add(list2);
        mxPoint startPoint = list1.get(0);
        return getInputOrientation(list, startPoint, splitPoint);
    }

    /**
     * Get the orientation for the Input Port of a Split Block.<br/>
     * There are multiple routes which can contain go through the point. Choose
     * the one with least turning point and then use it to decide the
     * orientation of the real income port.
     *
     * @param list
     *            the list of all optimal routes
     * @param startPoint
     *            the start point
     * @param splitPoint
     *            the new position for the Split Block
     * @return
     */
    private static Orientation getInputOrientation(List<List<mxPoint>> list, mxPoint startPoint, mxPoint splitPoint) {
        int[] turning = new int[list.size()];
        Orientation[] orientation = new Orientation[list.size()];
        double x = splitPoint.getX();
        double y = splitPoint.getY();
        double xStart = startPoint.getX();
        double yStart = startPoint.getY();

        for (int p = 0; p < list.size(); p++) {
            List<mxPoint> list1 = list.get(p);
            int num1 = list1.size();
            if (num1 <= 1) {
                continue;
            }

            // calculate the number of turning points before the route goes through the point.
            for (int i = 1, count = 0; i < num1; i++, count++) {
                mxPoint p0 = list1.get(i - 1);
                mxPoint p1 = list1.get(i);
                double x0 = p0.getX();
                double y0 = p0.getY();
                double x1 = p1.getX();
                double y1 = p1.getY();

                // start from the start point,
                if (XcosRouteUtils.pointInLineSegment(xStart, yStart, x0, y0, x1, y1)) {
                    count = 0;
                }

                // if the point is in this segment,
                if (XcosRouteUtils.pointInLineSegment(x, y, x0, y0, x1, y1)) {
                    turning[p] = count;
                    if (x1 == x0 && y1 > y0) { // segment: south
                        orientation[p] = Orientation.NORTH;
                    } else if (x1 == x0 && y1 < y0) { // segment: north
                        orientation[p] = Orientation.SOUTH;
                    } else if (y1 == y0 && x1 > x0) { // segment: east
                        orientation[p] = Orientation.WEST;
                    } else if (y1 == y0 && x1 < x0) { // segment: west
                        orientation[p] = Orientation.EAST;
                    }
                    break;
                }
            }
        }

        // choose the one with least turning point.
        int index = 0;
        int tmp = turning[0];
        for (int i = 1; i < turning.length; i++) {
            if (turning[i] < tmp) {
                tmp = turning[i];
                index = i;
            }
        }
        return orientation[index];
    }

    /**
     * Get the orientation for the one Port of a Split Block according to
     * its optimal route.
     *
     * @param list
     *            the optimal route including start Port and end Port
     * @param splitPoint
     *            the new position for the Split Block
     * @return
     */
    private static Orientation getPortOrientation(List<mxPoint> list, mxPoint splitPoint) {
        double x = splitPoint.getX();
        double y = splitPoint.getY();
        int num = list.size();
        if (num <= 1) {
            return null;
        }

        for (int i = 1; i < num; i++) {
            mxPoint p0 = list.get(i - 1);
            mxPoint p1 = list.get(i);
            double x0 = p0.getX();
            double y0 = p0.getY();
            double x1 = p1.getX();
            double y1 = p1.getY();

            // if the point is in this segment,
            if (XcosRouteUtils.pointInLineSegment(x, y, x0, y0, x1, y1)) {
                // if the point is in the next turning point,
                if (x == x1 && y == y1 && i + 1 != num) {
                    mxPoint p2 = list.get(i + 1);
                    double x2 = p2.getX();
                    double y2 = p2.getY();
                    if (x == x2 && y < y2) { // segment: south
                        return Orientation.SOUTH;
                    } else if (x == x2 && y > y2) { // segment: north
                        return Orientation.NORTH;
                    } else if (y == y2 && x < x2) { // segment: east
                        return Orientation.EAST;
                    } else if (y == y2 && x > x2) { // segment: west
                        return Orientation.WEST;
                    }
                }

                if (x1 == x0 && y1 > y0) { // segment: south
                    return Orientation.SOUTH;
                } else if (x1 == x0 && y1 < y0) { // segment: north
                    return Orientation.NORTH;
                } else if (y1 == y0 && x1 > x0) { // segment: east
                    return Orientation.EAST;
                } else if (y1 == y0 && x1 < x0) { // segment: west
                    return Orientation.WEST;
                }
                break;
            }
        }
        return null;
    }

    /**
     * Update SplitBlock's input port and output ports.
     *
     * @param split
     *            the Split Block
     * @param listRoutes
     *            the list of routes
     * @param graph
     * @param input
     *            the input port of the Split Block
     */
    private static void updatePortOrientation(SplitBlock split, List<List<mxPoint>> listRoutes, XcosDiagram graph, BasicPort input) {
        mxGeometry splitGeo = graph.getModel().getGeometry(split);
        mxPoint splitPoint = new mxPoint(splitGeo.getCenterX(), splitGeo.getCenterY());

        // get the positions of the ports which connect to the corresponding ports of split block.
        BasicPort inport = split.getIn();
        BasicPort outport1 = split.getOut1();
        BasicPort outport2 = split.getOut2();
        mxPoint inPosition = getPortPosition(getSplitLinkPort(split, inport));
        mxPoint out1Position = getPortPosition(getSplitLinkPort(split, outport1));
        mxPoint out2Position = getPortPosition(getSplitLinkPort(split, outport2));

        // set the orientation for each ports according to which is the real income port.
        if (input == null || input == inport) {
            // if the previous split block connects to the input port or it is the starting split block
            // get Input Port Orientation
            inport.setOrientation(getInputOrientation(listRoutes, inPosition, splitPoint));
            // get OutPut Port Orientation
            for (int p = 0; p < listRoutes.size(); p++) {
                List<mxPoint> list1 = listRoutes.get(p);
                if (XcosRouteUtils.pointInLink(out1Position.getX(), out1Position.getY(), list1)) {
                    outport1.setOrientation(getPortOrientation(list1, splitPoint));
                }
                if (XcosRouteUtils.pointInLink(out2Position.getX(), out2Position.getY(), list1)) {
                    outport2.setOrientation(getPortOrientation(list1, splitPoint));
                }
            }
        } else if (input == outport1) {
            // if the previous split block connects to the output port 1
            outport1.setOrientation(getInputOrientation(listRoutes, out1Position, splitPoint));
            for (int p = 0; p < listRoutes.size(); p++) {
                List<mxPoint> list1 = listRoutes.get(p);
                if (XcosRouteUtils.pointInLink(inPosition.getX(), inPosition.getY(), list1)) {
                    inport.setOrientation(getPortOrientation(list1, splitPoint));
                }
                if (XcosRouteUtils.pointInLink(out2Position.getX(), out2Position.getY(), list1)) {
                    outport2.setOrientation(getPortOrientation(list1, splitPoint));
                }
            }
        } else if (input == outport2) {
            // if the previous split block connects to the output port 2
            outport2.setOrientation(getInputOrientation(listRoutes, out2Position, splitPoint));
            for (int p = 0; p < listRoutes.size(); p++) {
                List<mxPoint> list1 = listRoutes.get(p);
                if (XcosRouteUtils.pointInLink(out1Position.getX(), out1Position.getY(), list1)) {
                    outport1.setOrientation(getPortOrientation(list1, splitPoint));
                }
                if (XcosRouteUtils.pointInLink(inPosition.getX(), inPosition.getY(), list1)) {
                    inport.setOrientation(getPortOrientation(list1, splitPoint));
                }
            }
        }
    }

    /**
     * Update the links' style of a SplitBlock.
     *
     * @param splitblock
     *            the Split Block
     * @param all
     *            all the cells in graph
     * @param graph
     */
    private static void updateSplitLink(SplitBlock splitblock, Object[] all, XcosDiagram graph) {
        XcosRoute route = new XcosRoute();
        BasicLink linkIn = (BasicLink) splitblock.getIn().getEdgeAt(0);
        BasicLink linkOut1 = (BasicLink) splitblock.getOut1().getEdgeAt(0);
        BasicLink linkOut2 = (BasicLink) splitblock.getOut2().getEdgeAt(0);
        boolean lockPort = true;
        reset(graph, linkIn);
        reset(graph, linkOut1);
        reset(graph, linkOut2);
        graph.setCellStyles(mxConstants.STYLE_NOEDGESTYLE, "1", new BasicLink[] { linkIn, linkOut1, linkOut2 });
        route.updateRoute(linkIn, all, graph, lockPort);
        route.updateRoute(linkOut1, all, graph, lockPort);
        route.updateRoute(linkOut2, all, graph, lockPort);
    }

    /**
     * Reset the link.
     *
     * @param graph
     * @param edge
     */
    private static void reset(final ScilabGraph graph, final Object edge) {
        final SelectionCellsHandler selectionCellsHandler = (SelectionCellsHandler) graph.getAsComponent()
                .getSelectionCellsHandler();
        graph.resetEdge(edge);
        selectionCellsHandler.clearCellHandler(edge);
    }

}

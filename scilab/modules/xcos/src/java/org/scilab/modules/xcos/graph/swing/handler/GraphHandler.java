/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010-2010 - DIGITEO - Clement DAVID <clement.david@scilab.org>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.graph.swing.handler;

import java.awt.datatransfer.DataFlavor;
import java.awt.event.MouseEvent;
import java.util.ArrayList;
import java.util.List;
import java.util.logging.Logger;

import javax.swing.SwingUtilities;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.BlockFactory;
import org.scilab.modules.xcos.block.BlockFactory.BlockInterFunction;
import org.scilab.modules.xcos.block.TextBlock;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.graph.swing.GraphComponent;
import org.scilab.modules.xcos.link.BasicLink;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.utils.XcosMessages;

import com.mxgraph.model.mxGeometry;
import com.mxgraph.model.mxIGraphModel;
import com.mxgraph.swing.handler.mxGraphHandler;
import com.mxgraph.swing.util.mxGraphTransferable;
import com.mxgraph.util.mxPoint;
import com.mxgraph.view.mxGraph;

/**
 * Handle double click operation on the graph.
 */
public class GraphHandler extends mxGraphHandler {

    /**
     * Reduce the dropped data to JVM local transferable.
     */
    static {
        try {
            mxGraphTransferable.dataFlavor = new DataFlavor(DataFlavor.javaJVMLocalObjectMimeType + "; class=com.mxgraph.swing.util.mxGraphTransferable");
        } catch (ClassNotFoundException e) {
            Logger.getLogger(GraphHandler.class.getName()).severe(e.toString());
        }
    }

    /**
     * Default constructor
     *
     * @param graphComponent
     *            the graph
     */
    public GraphHandler(GraphComponent graphComponent) {
        super(graphComponent);
    }

    /**
     * Handle double click on :
     * <ul>
     * <li>An empty area: create a new {@link TextBlock}</li>
     * <li>An edge: create a new point on the edge</li>
     * <li>A block: open the block settings</li>
     * </ul>
     *
     * Handle right click on :
     * <ul>
     * <li>An empty area: set up the default context menu</li>
     * <li>An edge: create a new point on the edge</li>
     * <li>A block: open the block settings</li>
     * </ul>
     *
     * @param e
     *            the mouse event
     * @see com.mxgraph.swing.util.mxMouseAdapter#mouseClicked(java.awt.event.MouseEvent)
     */
    // CSOFF: CyclomaticComplexity
    @Override
    public void mouseClicked(MouseEvent e) {
        if (graphComponent.isEnabled() && isEnabled() && !e.isConsumed() && !graphComponent.isForceMarqueeEvent(e)) {

            cell = graphComponent.getCellAt(e.getX(), e.getY(), false);

            /*
             * Double click
             */
            if (e.getClickCount() >= 2 && SwingUtilities.isLeftMouseButton(e)) {
                if (cell instanceof BasicLink) {
                    clickOnLink(e, (BasicLink) cell);
                } else if (cell instanceof BasicBlock) {
                    openBlock(e, (BasicBlock) cell);
                } else if (cell instanceof BasicPort) {
                    // translated to the parent
                    openBlock(e, (BasicBlock) ((BasicPort) cell).getParent());
                } else if (cell == null) {
                    createTextBlock(e);
                }

                /*
                 * Right click
                 */
            } else if ((e.getClickCount() == 1 && SwingUtilities.isRightMouseButton(e)) || e.isPopupTrigger() || XcosMessages.isMacOsPopupTrigger(e)) {
                if (cell == null) {
                    ((GraphComponent) graphComponent).displayContextMenu();
                } else if (cell instanceof BasicLink) {
                    ((BasicLink) cell).openContextMenu((ScilabGraph) graphComponent.getGraph());
                } else if (cell instanceof BasicBlock) {
                    ((BasicBlock) cell).openContextMenu((ScilabGraph) graphComponent.getGraph());
                }

                e.consume();
            }
        }

        // fallback to the parent method
        if (!e.isConsumed()) {
            super.mouseClicked(e);
        }
    }

    // CSON: CyclomaticComplexity

    /**
     * Create a new text block at the point.
     *
     * @param e
     *            the current point
     */
    private void createTextBlock(MouseEvent e) {
        // allocate
        final TextBlock textBlock = (TextBlock) BlockFactory.createBlock(BlockInterFunction.TEXT_f);

        // set the position of the block
        final mxPoint pt = graphComponent.getPointForEvent(e);
        final mxGeometry geo = textBlock.getGeometry();
        geo.setX(pt.getX() - (geo.getWidth() / 2));
        geo.setY(pt.getY() - (geo.getHeight() / 2));

        // add the block to the graph
        final mxGraph graph = graphComponent.getGraph();
        graph.getModel().beginUpdate();
        try {
            graph.addCell(textBlock);
        } finally {
            graph.getModel().endUpdate();
        }

        e.consume();
    }

    /**
     * Add a point to a link.
     *
     * @param e
     *            the event
     * @param cell
     *            the link
     */
    private void clickOnLink(MouseEvent e, BasicLink cell) {
        // getting the point list
        List<mxPoint> points = graphComponent.getGraph().getCellGeometry(cell).getPoints();
        if (points == null) {
            points = new ArrayList<mxPoint>();
            cell.getGeometry().setPoints(points);
        }

        // get the point
        final mxPoint pt = graphComponent.getPointForEvent(e);

        // translate the point if it is a loop link
        if (cell.getSource() != null && cell.getTarget() != null && cell.getSource().getParent() == cell.getTarget().getParent()
                && cell.getSource().getParent() != null) {
            final mxGeometry parent = cell.getSource().getParent().getGeometry();
            pt.setX(pt.getX() - parent.getX());
            pt.setY(pt.getY() - parent.getY());
        }

        // add or remove the point to the list and fire event
        final mxIGraphModel model = graphComponent.getGraph().getModel();
        model.beginUpdate();
        try {
            final int index = cell.findNearestSegment(pt);
            if (index < points.size() && points.get(index).getPoint().distanceSq(pt.getPoint()) == 0) {
                points.remove(index);
            } else {
                points.add(index, pt);
            }
            model.setGeometry(cell, (mxGeometry) cell.getGeometry().clone());
        } finally {
            model.endUpdate();
        }

        e.consume();
    }

    /**
     * Open a block
     *
     * @param e
     *            the mouse event
     * @param cell
     *            the block
     */
    private void openBlock(MouseEvent e, BasicBlock cell) {
        cell.openBlockSettings(((XcosDiagram) graphComponent.getGraph()).getContext());

        e.consume();
    }
}

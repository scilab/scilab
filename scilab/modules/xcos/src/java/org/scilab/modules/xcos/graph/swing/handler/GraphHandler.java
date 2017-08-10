/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010-2010 - DIGITEO - Clement DAVID <clement.david@scilab.org>
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

package org.scilab.modules.xcos.graph.swing.handler;

import java.awt.datatransfer.DataFlavor;
import java.awt.event.MouseEvent;
import java.util.ArrayList;
import java.util.List;
import java.util.logging.Logger;

import javax.swing.SwingUtilities;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.base.GraphActionManager;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.TextBlock;
import org.scilab.modules.xcos.block.actions.BlockParametersAction;
import org.scilab.modules.xcos.graph.model.BlockInterFunction;
import org.scilab.modules.xcos.graph.model.XcosCellFactory;
import org.scilab.modules.xcos.graph.swing.GraphComponent;
import org.scilab.modules.xcos.link.BasicLink;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.utils.XcosMessages;

import com.mxgraph.model.mxGeometry;
import com.mxgraph.model.mxIGraphModel;
import com.mxgraph.swing.mxGraphComponent;
import com.mxgraph.swing.handler.mxGraphHandler;
import com.mxgraph.swing.util.mxGraphTransferable;
import com.mxgraph.util.mxPoint;
import com.mxgraph.view.mxGraph;
import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement;

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
                    openBlock(graphComponent, e, (BasicBlock) cell);
                } else if (cell instanceof BasicPort) {
                    // translated to the parent
                    openBlock(graphComponent, e, (BasicBlock) ((BasicPort) cell).getParent());
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
        final TextBlock textBlock;
        try {
            textBlock = (TextBlock) XcosCellFactory.createBlock(BlockInterFunction.TEXT_f);
        } catch (ScilabInterpreterManagement.InterpreterException ex) {
            // this is unexpected report : Scilab might not be accessible at all
            throw new RuntimeException(ex);
        }


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
     * @param comp the component
     * @param e
     *            the mouse event
     * @param cell
     *            the block
     */
    private void openBlock(mxGraphComponent comp, MouseEvent e, BasicBlock cell) {
        BlockParametersAction action = GraphActionManager.getInstance((ScilabGraph) comp.getGraph(), BlockParametersAction.class);
        action.actionPerformed();

        e.consume();
    }
}

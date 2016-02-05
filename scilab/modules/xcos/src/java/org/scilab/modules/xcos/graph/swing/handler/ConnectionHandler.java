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

import java.awt.Color;
import java.awt.event.MouseEvent;
import java.util.ArrayList;
import java.util.List;

import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.graph.swing.GraphComponent;

import com.mxgraph.model.mxICell;
import com.mxgraph.model.mxIGraphModel;
import com.mxgraph.swing.handler.mxCellMarker;
import com.mxgraph.swing.handler.mxConnectPreview;
import com.mxgraph.swing.handler.mxConnectionHandler;
import com.mxgraph.util.mxEvent;
import com.mxgraph.util.mxPoint;
import com.mxgraph.view.mxCellState;
import com.mxgraph.view.mxGraph;

/**
 * Connection handler used to handle multi point links.
 */
@SuppressWarnings(value = { "serial" })
public class ConnectionHandler extends mxConnectionHandler {
    private boolean multiPointLinkStarted;

    /**
     * Default constructor.
     *
     * @param graphComponent
     *            the component
     */
    public ConnectionHandler(GraphComponent graphComponent) {
        super(graphComponent);

        /*
         * Same default settings as mxConnectionHandler plus get first free
         * input port in case of a simple flow connection.
         */

        marker = new mxCellMarker(graphComponent) {
            // Overrides to return cell at location only if valid (so that
            // there is no highlight for invalid cells that have no error
            // message when the mouse is released)
            @Override
            protected Object getCell(MouseEvent e) {
                Object cell = super.getCell(e);

                if (isConnecting()) {
                    if (source != null) {
                        cell = getFirstValidPort(cell);
                        error = validateConnection(source.getCell(), cell);

                        if (error != null && error.length() == 0) {
                            cell = null;

                            // Enables create target inside groups
                            if (createTarget) {
                                error = null;
                            }
                        }
                    }
                } else if (!isValidSource(cell)) {
                    cell = null;
                }

                return cell;
            }

            // Sets the highlight color according to isValidConnection
            @Override
            protected boolean isValidState(mxCellState state) {
                if (isConnecting()) {
                    return error == null;
                } else {
                    return super.isValidState(state);
                }
            }

            // Overrides to use marker color only in highlight mode or for
            // target selection
            @Override
            protected Color getMarkerColor(MouseEvent e, mxCellState state, boolean isValid) {
                return (isHighlighting() || isConnecting()) ? super.getMarkerColor(e, state, isValid) : null;
            }

            // Overrides to use hotspot only for source selection otherwise
            // intersects always returns true when over a cell
            @Override
            protected boolean intersects(mxCellState state, MouseEvent e) {
                if (!isHighlighting() || isConnecting()) {
                    return true;
                }

                return super.intersects(state, e);
            }

            private Object getFirstValidPort(Object o) {
                if (!(o instanceof BasicBlock)) {
                    return o;
                }
                final BasicBlock block = (BasicBlock) o;

                for (int i = 0; i < block.getChildCount(); i++) {
                    final Object cell = block.getChildAt(i);

                    final String err = validateConnection(source.getCell(), cell);
                    if (err == null) {
                        return cell;
                    }
                }

                return o;
            }

            @Override
            public void reset() {
                if (markedState != null) {
                    this.graphComponent.getGraph().addSelectionCell(markedState.getCell());
                }

                super.reset();
            }
        };
    }

    /**
     * @return {@link ConnectPreview} instance
     * @see com.mxgraph.swing.handler.mxConnectionHandler#createConnectPreview()
     */
    @Override
    protected mxConnectPreview createConnectPreview() {
        return new ConnectPreview((GraphComponent) graphComponent);
    }

    /**
     * Enable or disable the reset handler which reset any action on graph
     * modification.
     *
     * @param status
     *            the enable status
     */
    protected void setResetEnable(boolean status) {
        final mxIGraphModel model = graphComponent.getGraph().getModel();

        if (status) {
            model.addListener(mxEvent.CHANGE, resetHandler);
        } else {
            model.removeListener(resetHandler, mxEvent.CHANGE);
        }
    }

    /*
     * mxMouseAdapter specific reimplementation
     */

    /**
     * Handle first release and click on the empty background during connection.
     *
     * @param e
     *            the event
     * @see com.mxgraph.swing.handler.mxConnectionHandler#mouseReleased(java.awt.event.MouseEvent)
     */
    @Override
    public void mouseReleased(MouseEvent e) {
        final boolean isEventValid = error != null && error.isEmpty() && !e.isConsumed();
        final boolean hasValidState = first != null && connectPreview.isActive() && !marker.hasValidState();

        if (isEventValid && hasValidState) {
            final mxGraph graph = graphComponent.getGraph();
            final double x = graph.snap(e.getX());
            final double y = graph.snap(e.getY());

            // we are during a link creation on an invalid area
            mxICell cell = (mxICell) connectPreview.getPreviewState().getCell();

            // allocate points if applicable
            List<mxPoint> points = cell.getGeometry().getPoints();
            if (points == null) {
                points = new ArrayList<mxPoint>();
                cell.getGeometry().setPoints(points);
            }

            // scale and set the point
            // extracted from mxConnectPreview#transformScreenPoint
            final mxPoint tr = graph.getView().getTranslate();
            final double scale = graph.getView().getScale();
            points.add(new mxPoint(graph.snap(x / scale - tr.getX()), graph.snap(y / scale - tr.getY())));

            // update the preview and set the flag
            connectPreview.update(e, null, x, y);
            multiPointLinkStarted = true;

            e.consume();
        } else {
            if (marker.hasValidState() && connectPreview.getPreviewState() != null) {
                final mxGraph graph = graphComponent.getGraph();
                final double x = graph.snap(e.getX());
                final double y = graph.snap(e.getY());

                // We are ending a link creation on an valid port,
                // so sync the points coordinates with the model
                mxICell cell = (mxICell) connectPreview.getPreviewState().getCell();
                cell.setGeometry(cell.getGeometry());
            }

            multiPointLinkStarted = false;
            super.mouseReleased(e);
        }
    }

    /**
     * Only chain up when the multi point link feature is disable, drag
     * otherwise.
     *
     * @param e
     *            the event
     * @see com.mxgraph.swing.handler.mxConnectionHandler#mouseMoved(java.awt.event.MouseEvent)
     */
    @Override
    public void mouseMoved(MouseEvent e) {
        if (multiPointLinkStarted) {
            mouseDragged(e);
        } else {
            super.mouseMoved(e);
        }
    }

    /**
     * Only chain up when multi point link feature is disable.
     *
     * This will not update the first point on multi point link creation.
     *
     * @param e
     *            the mouse event
     * @param state
     *            the marker valid state
     * @see com.mxgraph.swing.handler.mxConnectionHandler#start(java.awt.event.MouseEvent,
     *      com.mxgraph.view.mxCellState)
     */
    @Override
    public void start(MouseEvent e, mxCellState state) {
        if (!multiPointLinkStarted) {
            super.start(e, state);
        }
    }
}

/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
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

package org.scilab.modules.graph;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.GraphicsEnvironment;
import java.awt.event.MouseEvent;

import com.mxgraph.model.mxICell;
import com.mxgraph.model.mxIGraphModel;
import com.mxgraph.swing.mxGraphComponent;
import com.mxgraph.util.mxEvent;
import com.mxgraph.util.mxEventObject;
import com.mxgraph.util.mxEventSource;
import com.mxgraph.util.mxRectangle;
import com.mxgraph.view.mxCellState;
import com.mxgraph.view.mxGraphView;

/**
 * Implement the default component for the {@link ScilabGraph}.
 */
@SuppressWarnings(value = { "serial" })
public class ScilabComponent extends mxGraphComponent {
    /**
     * Color use to mask the graph when the graph is locked
     */
    private static final Color MASK_COLOR = new Color(240, 240, 240, 100);

    private static final double SCALE_MULTIPLIER = 1.1;

    /**
     * Construct the component with the associated graph
     *
     * @param graph
     *            The associated graph
     */
    public ScilabComponent(ScilabGraph graph) {
        super(graph);
    }

    /**
     * @return the associated graph control
     * @see com.mxgraph.swing.mxGraphComponent#createGraphControl()
     */
    @Override
    protected mxGraphControl createGraphControl() {
        return new ScilabGraphControl();
    }

    /**
     * Create the associated canvas
     *
     * @return the canvas
     */
    @Override
    public ScilabCanvas createCanvas() {
        return new ScilabCanvas();
    }

    /**
     * Zoom the whole graph and center the view on it.
     *
     * @param cells
     *            the cells to center on
     */
    public void zoomAndCenterToCells(final Object[] cells) {
        final mxRectangle preference = zoomBounds(cells);
        final Dimension actual = getViewport().getSize();

        final double newScale;
        final double heightScale = actual.getHeight() / preference.getHeight();
        final double widthScale = actual.getWidth() / preference.getWidth();

        if (heightScale > 1.0) {
            if (widthScale > 1.0) {
                // We need to zoom in (the max applicable zoom is the lowest)
                newScale = Math.min(heightScale, widthScale);
            } else {
                // we need to zoom out (only widthScale is < 1.0)
                newScale = widthScale;
            }
        } else {
            if (widthScale > 1.0) {
                // we need to zoom out (only heightScale is < 1.0)
                newScale = heightScale;
            } else {
                // We need to zoom out (the max applicable zoom is the lowest)
                newScale = Math.min(heightScale, widthScale);
            }
        }

        // do not apply small zoom values
        if (Math.abs(1.0 - newScale) < 0.2) {
            getGraphControl().scrollRectToVisible(zoomBounds(cells).getRectangle(), true);
            return;
        }

        zoom(newScale / SCALE_MULTIPLIER);
        getGraphControl().scrollRectToVisible(zoomBounds(cells).getRectangle(), true);
    }

    private final mxRectangle zoomBounds(final Object[] cells) {
        final mxRectangle preference;
        final Object[] c;
        if (cells == null || cells.length == 0) {
            c = graph.getChildCells(graph.getDefaultParent());
        } else {
            c = cells;
        }
        preference = getChildrenBounds(c);

        return preference;
    }

    /**
     * Get the children bound for the cells
     *
     * @param cells
     *            the root of the graph
     * @return the rectangle or null if not applicable
     */
    private mxRectangle getChildrenBounds(final Object[] cells) {
        mxRectangle result = null;

        if (cells != null && cells.length > 0) {
            final mxGraphView view = graph.getView();
            final mxIGraphModel model = graph.getModel();

            for (int i = 0; i < cells.length; i++) {
                if (model.isVertex(cells[i]) || model.isEdge(cells[i])) {
                    final mxICell parent = ((mxICell) cells[i]);
                    final int childCount = parent.getChildCount();

                    for (int j = 0; j < childCount; j++) {
                        final mxICell child = parent.getChildAt(j);

                        result = updateRectangle(result, view, child);
                    }

                    result = updateRectangle(result, view, parent);
                }
            }
        }

        return result;
    }

    /**
     * Update the rectangle parameter with the cell status
     *
     * @param result
     *            the previous result
     * @param view
     *            the current view
     * @param child
     *            the child we have to work on
     * @return the updated rectangle
     */
    private mxRectangle updateRectangle(mxRectangle result, final mxGraphView view, final mxICell child) {
        final mxCellState state = view.getState(child);
        mxRectangle rect = result;

        if (state != null) {
            if (rect == null) {
                rect = new mxRectangle(state);
            } else {
                rect.add(state);
            }
        }
        return rect;
    }

    /**
     * Implement a graph control which paint a foreground on top of the view
     * when the graph is locked.
     */
    @SuppressWarnings(value = { "serial" })
    public class ScilabGraphControl extends mxGraphControl {

        /**
         * Default constructor
         */
        public ScilabGraphControl() {
            super();

            // Paint the foreground color after the real paint
            addListener(mxEvent.AFTER_PAINT, new mxEventSource.mxIEventListener() {
                @Override
                public void invoke(Object sender, mxEventObject evt) {

                    Graphics g = (Graphics) evt.getProperty("g");
                    if (getGraph().isCellsLocked()) {
                        g.setColor(MASK_COLOR);

                        Dimension b = getGraphControl().getSize();

                        g.fillRect(0, 0, b.width, b.height);
                    }
                }
            });
        }

        /**
         * @see javax.swing.JComponent#processMouseMotionEvent(java.awt.event.MouseEvent)
         *
         * Overloaded to filter out any cursor update if the graph is locked
         */
        @Override
        protected void processMouseMotionEvent(MouseEvent e) {
            if (!getGraph().isCellsLocked()) {
                super.processMouseMotionEvent(e);
            }
        }
    }

    /*
     * Disable some handlers in case of an headless env.
     */

    @Override
    protected void createHandlers() {
        if (GraphicsEnvironment.isHeadless()) {
            return;
        }

        super.createHandlers();
    }
}

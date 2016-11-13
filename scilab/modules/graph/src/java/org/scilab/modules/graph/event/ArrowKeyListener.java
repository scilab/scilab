/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Clement DAVID
 * Copyright (C) 2012 - Scilab Enterprises - Clement DAVID
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

package org.scilab.modules.graph.event;

import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.util.List;

import javax.swing.Timer;

import com.mxgraph.model.mxGeometry;
import com.mxgraph.model.mxIGraphModel;
import com.mxgraph.swing.mxGraphComponent;
import com.mxgraph.util.mxPoint;
import com.mxgraph.view.mxGraph;

/**
 * Move cells with the arrow keys.
 */
public final class ArrowKeyListener implements KeyListener {

    private static final int DEFAULT_PIXEL_MOVE = 1;
    private static final int MODIFIER_FACTOR = 5;
    private static final int DEFAULT_DELAY = 800; // milliseconds

    /* Configuration variables */
    private final int pixelMove = DEFAULT_PIXEL_MOVE;
    private int delay = DEFAULT_DELAY;

    /* Runtime variables */
    private double xIncrement;
    private double yIncrement;
    private mxGraph graph;

    private final Timer repetitionTimer;
    private final ActionListener doMove = new ActionListener() {
        @Override
        public void actionPerformed(ActionEvent arg0) {
            if (graph != null) {

                final mxIGraphModel model = graph.getModel();
                model.beginUpdate();
                try {
                    for (Object cell : graph.getSelectionCells()) {
                        // first snap
                        final mxGeometry geom = model.getGeometry(cell);
                        snap(graph, geom);

                        // then move and transaction
                        graph.translateCell(cell, xIncrement, yIncrement);
                    }
                } finally {
                    model.endUpdate();
                }

            }
        }
    };

    private static final mxGeometry snap(final mxGraph graph, final mxGeometry rect) {
        final double x = graph.snap(rect.getX());
        final double y = graph.snap(rect.getY());

        final double width = graph.snap(rect.getWidth() - x + rect.getX());
        final double height = graph.snap(rect.getHeight() - y + rect.getY());

        final List<mxPoint> points = rect.getPoints();
        if (points != null) {
            for (final mxPoint p : points) {
                p.setX(graph.snap(p.getX()));
                p.setY(graph.snap(p.getY()));
            }
        }

        final mxGeometry snappedGeom = new mxGeometry(x, y, width, height);
        snappedGeom.setPoints(points);

        return snappedGeom;
    }

    /**
     * Constructor
     */
    public ArrowKeyListener() {
        repetitionTimer = new Timer(delay, doMove);
        repetitionTimer.setInitialDelay(0);
    }

    /**
     * @param delay
     *            the delay to set
     */
    public void setDelay(int delay) {
        this.delay = delay;
        repetitionTimer.setDelay(delay);
    }

    /**
     * @return the delay
     */
    public int getDelay() {
        return delay;
    }

    /**
     * Get the action parameters and start the action timer.
     *
     * @param e
     *            key event
     */
    @Override
    public void keyPressed(KeyEvent e) {
        if (e.isConsumed()) {
            return;
        }

        int realMove;
        boolean mustMove = true;

        mxGraphComponent sourceDiagram = (mxGraphComponent) e.getSource();
        graph = sourceDiagram.getGraph();

        if (graph.isGridEnabled()) {
            realMove = graph.getGridSize();
        } else {
            realMove = pixelMove;
        }

        if (e.getModifiers() == Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()) {
            realMove *= MODIFIER_FACTOR;
        }

        switch (e.getKeyCode()) {
            case KeyEvent.VK_UP:
                yIncrement = -realMove;
                break;

            case KeyEvent.VK_DOWN:
                yIncrement = realMove;
                break;

            case KeyEvent.VK_RIGHT:
                xIncrement = realMove;
                break;

            case KeyEvent.VK_LEFT:
                xIncrement = -realMove;
                break;

            default:
                mustMove = false;
                break;
        }

        if (!mustMove) {
            return;
        }

        if (!graph.isGridEnabled()) {
            xIncrement *= sourceDiagram.getZoomFactor();
            yIncrement *= sourceDiagram.getZoomFactor();
        }

        repetitionTimer.start();

        e.consume();
    }

    /**
     * Stop the action timer and clear parameters
     *
     * @param e
     *            key event
     */
    @Override
    public void keyReleased(KeyEvent e) {
        if (e.isConsumed()) {
            return;
        }

        repetitionTimer.stop();
        yIncrement = 0;
        xIncrement = 0;
        graph = null;

        e.consume();
    }

    /**
     * Not used there
     *
     * @param e
     *            Not used
     */
    @Override
    public void keyTyped(KeyEvent e) {
    }
}

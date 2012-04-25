/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.graph.event;

import java.awt.Rectangle;
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;

import javax.swing.Timer;

import com.mxgraph.swing.mxGraphComponent;
import com.mxgraph.util.mxRectangle;
import com.mxgraph.view.mxGraph;

/**
 * Move cells with the arrow keys.
 */
public final class ArrowKeyListener implements KeyListener {

    private static final int DEFAULT_PIXEL_MOVE = 1;
    private static final int MODIFIER_FACTOR = 5;
    private static final int DEFAULT_DELAY = 800; // milliseconds

    /* Configuration variables */
    private int pixelMove = DEFAULT_PIXEL_MOVE;
    private int delay = DEFAULT_DELAY;

    /* Runtime variables */
    private int xIncrement;
    private int yIncrement;
    private mxGraph graph;

    private Timer repetitionTimer;
    private ActionListener doMove = new ActionListener() {
        public void actionPerformed(ActionEvent arg0) {
            if (graph != null) {
                graph.getModel().beginUpdate();
                try {
                    for (Object cell : graph.getSelectionCells()) {
                        final Rectangle rect = graph.getModel().getGeometry(cell).getRectangle();

                        // first increment
                        rect.x = rect.x + xIncrement;
                        rect.y = rect.y + yIncrement;

                        // then align
                        int x = (int) graph.snap(rect.x);
                        int y = (int) graph.snap(rect.y);
                        rect.width = (int) graph.snap(rect.width - x + rect.x);
                        rect.height = (int) graph.snap(rect.height - y + rect.y);
                        rect.x = x;
                        rect.y = y;

                        graph.resizeCell(cell, new mxRectangle(rect));
                    }
                } finally {
                    graph.getModel().endUpdate();
                }

            }
        }
    };

    /**
     * Constructor
     */
    public ArrowKeyListener() {
        repetitionTimer = new Timer(delay, doMove);
        repetitionTimer.setInitialDelay(0);
    }

    /**
     * @param delay the delay to set
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
    public void keyTyped(KeyEvent e) {
    }
}

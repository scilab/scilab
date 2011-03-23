/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Clément DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.graph.event;

import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;

import javax.swing.Timer;

import com.mxgraph.swing.mxGraphComponent;
import com.mxgraph.view.mxGraph;

/**
 * Move cells with the arrow keys.
 */
public final class ArrowKeyListener implements KeyListener {

	private static final double DEFAULT_PIXEL_MOVE = 1;
	private static final double MODIFIER_FACTOR = 5;
	private static final int DEFAULT_DELAY = 800; // milliseconds

	/* Configuration variables */
	private double pixelMove = DEFAULT_PIXEL_MOVE;
	private int delay = DEFAULT_DELAY;
	
	/* Runtime variables */
	private double xIncrement;
	private double yIncrement;
	private mxGraph graph;

	private Timer repetitionTimer;
	private ActionListener doMove = new ActionListener() {
		public void actionPerformed(ActionEvent arg0) {
			if (graph != null) {
				graph.moveCells(graph.getSelectionCells(), xIncrement, yIncrement, false);
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
	 * @param pixelMove the pixelMove to set
	 */
	public void setPixelMove(double pixelMove) {
		this.pixelMove = pixelMove;
	}

	/**
	 * @return the pixelMove
	 */
	public double getPixelMove() {
		return pixelMove;
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
		
		double realMove;
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

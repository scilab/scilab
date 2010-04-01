/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clément DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.graph;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Rectangle;

import com.mxgraph.model.mxICell;
import com.mxgraph.model.mxIGraphModel;
import com.mxgraph.swing.mxGraphComponent;
import com.mxgraph.util.mxRectangle;
import com.mxgraph.view.mxCellState;
import com.mxgraph.view.mxGraphView;

/**
 * Implement the default component for the {@link ScilabGraph}.
 */
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
	public ScilabCanvas createCanvas() {
		return new ScilabCanvas();
	}

	/**
	 * Zoom the whole graph and center the view on it.
	 */
	public void zoomAndCenterToCells() {
		mxGraphView view = graph.getView();
		Rectangle preference = (Rectangle) getChildrenBounds(
				graph.getChildCells(graph.getDefaultParent())).getRectangle();

		Dimension actual = getViewport().getSize();

		double newScale;
		double heightScale = actual.getHeight() / preference.getHeight();
		double widthScale = actual.getWidth() / preference.getWidth();

		if (heightScale > 1.0) {
			if (widthScale > 1.0) {
				// We need to zoom in (the max applicable zoom is the lowest)
				newScale = Math.min(heightScale, widthScale)
						* (1.0 - (SCALE_MULTIPLIER - 1.0));
			} else {
				// we need to zoom out (only widthScale is < 1.0)
				newScale = widthScale * SCALE_MULTIPLIER;
			}
		} else {
			if (widthScale > 1.0) {
				// we need to zoom out (only heightScale is < 1.0)
				newScale = heightScale * SCALE_MULTIPLIER;
			} else {
				// We need to zoom out (the max applicable zoom is the lowest)
				newScale = Math.min(heightScale, widthScale) * SCALE_MULTIPLIER;
			}
		}

		zoom(newScale);

		view.revalidate();
		Rectangle orig = (Rectangle) getChildrenBounds(
				graph.getChildCells(graph.getDefaultParent())).getRectangle();
		getGraphControl().scrollRectToVisible(orig);
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
	private mxRectangle updateRectangle(mxRectangle result,
			final mxGraphView view, final mxICell child) {
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
	public class ScilabGraphControl extends mxGraphControl {
		
		/**
		 * Default constructor 
		 */
		public ScilabGraphControl() {
			super();
		}
		
		/**
		 * Paint the component and the foreground color
		 * @param g the current graphic context
		 * @see com.mxgraph.swing.mxGraphComponent.mxGraphControl#paintComponent(java.awt.Graphics)
		 */
		@Override
		public void paintComponent(Graphics g) {
			super.paintComponent(g);
			
			if (getGraph().isCellsLocked()) {
				g.setColor(MASK_COLOR);
				
				Rectangle b = getBounds();
				
				g.fillRect(b.x, b.y, b.width, b.height);
			}
		}
	}
}

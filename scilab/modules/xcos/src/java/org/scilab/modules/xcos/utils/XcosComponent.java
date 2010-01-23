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

package org.scilab.modules.xcos.utils;

import java.awt.Dimension;
import java.awt.Rectangle;
import java.awt.event.ComponentEvent;
import java.awt.event.ComponentListener;

import org.scilab.modules.xcos.graph.PaletteDiagram;

import com.mxgraph.model.mxICell;
import com.mxgraph.model.mxIGraphModel;
import com.mxgraph.swing.mxGraphComponent;
import com.mxgraph.util.mxRectangle;
import com.mxgraph.view.mxCellState;
import com.mxgraph.view.mxGraph;
import com.mxgraph.view.mxGraphView;

/**
 * Customize the default component
 * @see java.awt.Component
 */
public class XcosComponent extends mxGraphComponent {

    private static final long serialVersionUID = 1592816691330208090L;
    private static final double SCALE_MULTIPLIER = 1.1;

    /**
     * Construct the component with the associated graph
     * @param graph The associated graph
     */
    public XcosComponent(mxGraph graph) {
	super(graph);
	
	addComponentListener(new ComponentListener() {
	    
	    public void componentShown(ComponentEvent arg0) {
		if (getGraph() instanceof PaletteDiagram) {
		    ((PaletteDiagram) getGraph()).updateDiagram(getSize().getWidth());
		}		
	    }
	    
	    public void componentResized(ComponentEvent arg0) {
		if (getGraph() instanceof PaletteDiagram) {
		    ((PaletteDiagram) getGraph()).updateDiagram(getSize().getWidth());
		}		
	    }
	    
	    public void componentMoved(ComponentEvent arg0) {
	    }
	    
	    public void componentHidden(ComponentEvent arg0) {
	    }
	});
    }

    /**
     * Create the associated canvas
     * @return the canvas
     */
    public XcosCanvas createCanvas() {
	return new XcosCanvas();
    }
    
    /**
     * @return The filename
     */
    public String toString() {
	if (getGraph() instanceof PaletteDiagram) {
	    String fileName = ((PaletteDiagram) getGraph()).getName();
	    return fileName.substring(0, fileName.lastIndexOf('.'));
	} else {
	    return super.toString();
	}
    }
    
    /**
     * Zoom the whole graph and center the view on it.
     */
    public void zoomAndCenterToCells() {
	mxGraphView view = graph.getView();
	Rectangle preference = (Rectangle) getChildrenBounds(graph.getChildCells(graph.getDefaultParent())).getRectangle();
	
	Dimension actual = getViewport().getSize();

	double newScale;
	double heightScale = actual.getHeight() / preference.getHeight();
	double widthScale = actual.getWidth() / preference.getWidth();
	
	if (heightScale > 1.0) {
	    if (widthScale > 1.0) {
		// We need to zoom in (the max applicable zoom is the lowest)
		newScale = Math.min(heightScale, widthScale) * (1.0 - (SCALE_MULTIPLIER - 1.0));
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
	Rectangle orig = (Rectangle) getChildrenBounds(graph.getChildCells(graph.getDefaultParent())).getRectangle();
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
}

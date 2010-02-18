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

package org.scilab.modules.graph.view;

import java.awt.Component;
import java.util.Map;

import javax.swing.Icon;

import org.scilab.modules.graph.utils.MathMLRenderUtils;
import org.scilab.modules.graph.utils.ScilabGraphUtils;
import org.scilab.modules.gui.messagebox.ScilabModalDialog;
import org.xml.sax.SAXException;

import com.mxgraph.model.mxCell;
import com.mxgraph.util.mxConstants;
import com.mxgraph.util.mxRectangle;
import com.mxgraph.util.mxUtils;
import com.mxgraph.view.mxCellState;
import com.mxgraph.view.mxGraph;
import com.mxgraph.view.mxGraphView;

/**
 * Implement specific method to render a graph
 */
public class ScilabGraphView extends mxGraphView {
	/**
	 * Empty area real size between bounding-box and paint area. 
	 */
	private static final double LABEL_BORDER = 8.0;

	/**
	 * Default constructor
	 * @param graph the associated graph
	 */
	public ScilabGraphView(mxGraph graph) {
		super(graph);
	}
	
	/**
	 * Updates the label bounds in the given state.
	 * @param state the cell visible state
	 */
	public void updateLabelBounds(mxCellState state) {
		Object cell = state.getCell();
		Map<String, Object> style = state.getStyle();
		String label = graph.getLabel(cell);
		
		SupportedLabelType type = SupportedLabelType.getFromHTML(label);
		mxRectangle labelBounds;
		double w;
		double h;
		
		switch (type) {
		case Latex:
			/*
			 * As we render text as an image, the label bounds are set to the
			 * scaled generated image values.
			 */
			try {
				Icon icon = ScilabGraphUtils.getTexIcon(label);
				w = icon.getIconWidth();
				h = icon.getIconHeight();
				
				state.setWidth(((w + LABEL_BORDER) * scale) + (2 * mxConstants.LABEL_INSET));
				state.setHeight(((h + LABEL_BORDER) * scale) + (2 * mxConstants.LABEL_INSET));
				labelBounds = state;
			} catch (Exception e) {
				// popup an error
				// FIXME: use a ScilabGraphTab instead of null there
				ScilabModalDialog.show(null, e.getLocalizedMessage());
				
				// Set a non-rendering label on the model
				// label will be printed (state contains the value)
				mxCell c = (mxCell) cell;
				c.setValue(((String) c.getValue()).substring(1));
				labelBounds = getDefaultBounds(state, cell, style, label);
			}
			break;
			
		case MathML:
			/*
			 * As we render text as an image, the label bounds are set to the
			 * scaled generated image values.
			 */
			try {
				Component comp = MathMLRenderUtils.getMathMLComponent(label);
				w = comp.getWidth();
				h = comp.getHeight();
				
				state.setWidth(((w + LABEL_BORDER) * scale) + (2 * mxConstants.LABEL_INSET));
				state.setHeight(((h + LABEL_BORDER) * scale) + (2 * mxConstants.LABEL_INSET));
				labelBounds = state;
			} catch (SAXException e) {
				// popup an error
				// FIXME: use a ScilabGraphTab instead of null there
				ScilabModalDialog.show(null, e.getLocalizedMessage());
				
				// Set a non-rendering label on the model
				// label will be printed (state contains the value)
				mxCell c = (mxCell) cell;
				c.setValue(((String) c.getValue()).substring(1));
				labelBounds = getDefaultBounds(state, cell, style, label);
			}
			break;
			
		default:
			labelBounds = getDefaultBounds(state, cell, style, label);
			break;
		}
		
		state.setLabelBounds(labelBounds);
	}

	/**
	 * Return the default bounds calculated as if the label were text.
	 * @param state the cell state
	 * @param cell the current cell 
	 * @param style the cell style
	 * @param label the current text
	 * @return the calculated bounds
	 */
	private mxRectangle getDefaultBounds(mxCellState state, Object cell,
			Map<String, Object> style, String label) {
		mxRectangle labelBounds;
		mxRectangle vertexBounds;
		
		if (!graph.getModel().isEdge(cell)) {
			vertexBounds = state;
		} else {
			vertexBounds = null;
		}

		labelBounds = mxUtils.getLabelPaintBounds(label, style, graph
				.isHtmlLabel(cell), state.getAbsoluteOffset(), vertexBounds,
				scale);
		return labelBounds;
	}
}

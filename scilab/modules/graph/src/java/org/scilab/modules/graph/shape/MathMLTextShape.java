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

package org.scilab.modules.graph.shape;

import java.awt.Color;
import java.awt.Component;
import java.awt.Graphics2D;
import java.awt.Rectangle;
import java.awt.geom.AffineTransform;
import java.util.Map;

import javax.swing.CellRendererPane;

import org.apache.commons.logging.LogFactory;
import org.scilab.modules.graph.utils.MathMLRenderUtils;
import org.xml.sax.SAXException;

import com.mxgraph.canvas.mxGraphics2DCanvas;
import com.mxgraph.shape.mxITextShape;
import com.mxgraph.util.mxConstants;
import com.mxgraph.util.mxUtils;
import com.mxgraph.view.mxCellState;

/**
 * Implement a text shape that can draw MathML text. 
 *
 */
public class MathMLTextShape implements mxITextShape {

	/**
	 * Painter
	 * 
	 * @param canvas the current canvas
	 * @param text the text to render
	 * @param state the current state
	 * @param style the current style
	 * @see com.mxgraph.shape.mxITextShape#paintShape(com.mxgraph.canvas.mxGraphics2DCanvas, java.lang.String, com.mxgraph.util.mxRectangle, java.util.Map)
	 */
	@Override
	public void paintShape(mxGraphics2DCanvas canvas, String text,
			mxCellState state, Map<String, Object> style) {
		
		CellRendererPane rendererPane = canvas.getRendererPane();
		Rectangle rect = state.getRectangle();
		Graphics2D g = canvas.getGraphics();
		
		if (rendererPane != null
				&& (g.getClipBounds() == null || g.getClipBounds().intersects(
						rect))) {
			double scale = canvas.getScale();
			int x = rect.x;
			int y = rect.y;
			int w = rect.width;
			int h = rect.height;
			
			if (g.hitClip(x, y, w, h)) {
				AffineTransform at = g.getTransform();

				g.scale(scale, scale);
				Color textColor = mxUtils.getColor(style, mxConstants.STYLE_FONTCOLOR, Color.BLACK);
				rendererPane.setForeground(textColor);
				
				try {
				Component comp = MathMLRenderUtils.getMathMLComponent(text);
				rendererPane.paintComponent(g, comp, rendererPane,
						(int) (x / scale) + mxConstants.LABEL_INSET,
						(int) (y / scale) + mxConstants.LABEL_INSET,
						(int) (w / scale), (int) (h / scale), true);

				// Restores the previous transformation
				g.setTransform(at);
				} catch (SAXException e) {
					LogFactory.getLog(MathMLTextShape.class).error("Invalid text rendering");
				}
			}
		}
	}

}

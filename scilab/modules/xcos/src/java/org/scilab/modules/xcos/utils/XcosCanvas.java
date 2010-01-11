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

import java.awt.AlphaComposite;
import java.awt.Color;
import java.awt.Composite;
import java.awt.Paint;
import java.awt.Stroke;
import java.awt.geom.AffineTransform;
import java.util.Hashtable;

import com.mxgraph.swing.view.mxInteractiveCanvas;
import com.mxgraph.util.mxConstants;
import com.mxgraph.util.mxUtils;

/**
 * Painter for each vertex and edge
 */
public class XcosCanvas extends mxInteractiveCanvas {

	/** Default constructor */
	public XcosCanvas() { }
	
	/**
	 * Draw the vertex
	 * 
	 * @param x horizontal coordinate
	 * @param y vertical coordinate
	 * @param w width
	 * @param h height
	 * @param style All the style of the associated vertex
	 * @return always null
	 */
	@Override
	public Object drawVertex(int x, int y, int w, int h,
			Hashtable<String, Object> style) {

		if (g != null) {
			x += translate.x;
			y += translate.y;

			// Applies the rotation on the graphics object and stores
			// the previous transform so that it can be restored
			AffineTransform saveTransform = g.getTransform();
			g.translate(x + (w / 2.0), y + (h / 2.0));

			double rotation = mxUtils.getDouble(style,
					mxConstants.STYLE_ROTATION, 0);

			if (rotation != 0) {
				g.rotate(Math.toRadians(rotation));
				if (rotation == 90 || rotation == 270) {
					// x - h / 2, y - w / 2, h, w
					x = x + (w / 2) - (h / 2);
					y = y + (h / 2) - (w / 2);
					int hh = h;
					h = w;
					w = hh;
				}
			}

			String flip = mxUtils.getString(style, XcosConstants.STYLE_FLIP,
					"false");
			String mirror = mxUtils.getString(style,
					XcosConstants.STYLE_MIRROR, "false");

			// scale, 1st flip, 2nd mirror
			if (flip.compareTo("true") == 0) {
				if (mirror.compareTo("true") == 0) {
					g.scale(-1, -1); // T / T
				} else {
					g.scale(-1, 1); // T / F
				}
			} else {
				if (mirror.compareTo("true") == 0) {
					g.scale(1, -1); // F / T
				} else {
					g.scale(1, 1); // F / F
				}
			}

			g.translate(-(x + (w / 2.0)), -(y + (h / 2.0)));

			Composite composite = null;
			float opacity = mxUtils.getFloat(style, mxConstants.STYLE_OPACITY,
					100);

			// Applies the opacity to the graphics object
			if (opacity != 100) {
				composite = g.getComposite();
				g.setComposite(AlphaComposite.getInstance(
						AlphaComposite.SRC_OVER, opacity / 100));
			}

			// Saves the stroke
			Stroke stroke = g.getStroke();

			// Draws a swimlane if start is > 0
			int start = mxUtils.getInt(style, mxConstants.STYLE_STARTSIZE);

			if (start == 0) {
				drawShape(x, y, w, h, style);
			} else {
				start = (int) Math.round(start * scale);

				// Removes some styles to draw the content area
				Hashtable<String, Object> cloned = new Hashtable<String, Object>(
						style);
				cloned.remove(mxConstants.STYLE_FILLCOLOR);
				cloned.remove(mxConstants.STYLE_ROUNDED);

				if (mxUtils.isTrue(style, mxConstants.STYLE_HORIZONTAL, true)) {
					drawShape(x, y, w, start, style);
					drawShape(x, y + start, w, h - start, cloned);
				} else {
					drawShape(x, y, start, h, style);
					drawShape(x + start, y, w - start, h, cloned);
				}
			}

			// Restores the stroke
			g.setStroke(stroke);

			// Restores the composite rule on the graphics object
			if (composite != null) {
				g.setComposite(composite);
			}

			// Restores the affine transformation
			if (saveTransform != null) {
				g.setTransform(saveTransform);
			}
		}

		return null;
	}

	/**
	 * Draws a rectangle for the given parameters.
	 * 
	 * @param x X-coordinate of the shape.
	 * @param y Y-coordinate of the shape.
	 * @param w Width of the shape.
	 * @param h Height of the shape.
	 * @param fillColor Optional fill color of the shape.
	 * @param fillPaint Optional paint of the shape.
	 * @param penColor Optional stroke color.
	 * @param shadow Boolean indicating if a shadow should be painted.
	 * @param rounded Boolean indicating if the rectangle is rounded.
	 */
	public void drawRect(int x, int y, int w, int h, Color fillColor,
			Paint fillPaint, Color penColor, boolean shadow, boolean rounded) {
		super.drawRect(x, y, w, h, fillColor, fillPaint, penColor, shadow,
				rounded);
	}
}

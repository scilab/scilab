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

package org.scilab.modules.graph;

import java.awt.AlphaComposite;
import java.awt.Composite;
import java.awt.Stroke;
import java.awt.geom.AffineTransform;
import java.util.Hashtable;

import java.util.Map;

import org.scilab.modules.graph.utils.ScilabConstants;

import com.mxgraph.swing.view.mxInteractiveCanvas;
import com.mxgraph.util.mxConstants;
import com.mxgraph.util.mxUtils;

/**
 * Painter for each vertex and edge
 * 
 * This is tightly coupled to jgraphx internals.
 */
public class ScilabCanvas extends mxInteractiveCanvas {

	/** The rotation step of the clockwise and anticlockwise rotation */
	public static final int ROTATION_STEP = 90;
	/** The max valid rotation value (always 360°) */
	public static final int MAX_ROTATION = 360;
	
	private static final int OPACITY_MAX = 100;
	
	/** Default constructor */
	public ScilabCanvas() { }
	
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
			Map<String, Object> style) {

		int xx = x;
		int yy = y;
		int hh = h;
		int ww = w;
		
		if (g != null) {
			xx += translate.x;
			yy += translate.y;

			// Applies the rotation on the graphics object and stores
			// the previous transform so that it can be restored
			AffineTransform saveTransform = g.getTransform();
			g.translate(xx + (ww / 2.0), yy + (hh / 2.0));

			double rotation = mxUtils.getDouble(style,
					mxConstants.STYLE_ROTATION, 0);

			if (rotation != 0) {
				g.rotate(Math.toRadians(rotation));
				if (isNearHorizontalSide(rotation)) {
					// x - h / 2, y - w / 2, h, w
					xx = xx + (ww / 2) - (hh / 2);
					yy = yy + (hh / 2) - (ww / 2);
					
					ww = h;
					hh = w;
				}
			}

			applyFlipAndMirror(style);

			g.translate(-(xx + (ww / 2.0)), -(yy + (hh / 2.0)));

			Composite composite = null;
			float opacity = mxUtils.getFloat(style, mxConstants.STYLE_OPACITY,
					OPACITY_MAX);

			// Applies the opacity to the graphics object
			if (opacity != OPACITY_MAX) {
				composite = g.getComposite();
				g.setComposite(AlphaComposite.getInstance(
						AlphaComposite.SRC_OVER, opacity / OPACITY_MAX));
			}

			// Saves the stroke
			Stroke stroke = g.getStroke();

			// Draws a swimlane if start is > 0
			drawSwimline(xx, yy, ww, hh, style);

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
	 * Scale the graphic context depending on the "flip and "mirror" properties
	 * @param style Style contents
	 */
	private void applyFlipAndMirror(Map<String, Object> style) {
		String flip = mxUtils.getString(style, ScilabConstants.STYLE_FLIP,
				Boolean.FALSE.toString());
		String mirror = mxUtils.getString(style, ScilabConstants.STYLE_MIRROR,
				Boolean.FALSE.toString());

		// scale, 1st flip, 2nd mirror
		if (Boolean.parseBoolean(flip)) {
			if (Boolean.parseBoolean(mirror)) {
				g.scale(-1, -1); // T / T
			} else {
				g.scale(-1, 1); // T / F
			}
		} else {
			if (Boolean.parseBoolean(mirror)) {
				g.scale(1, -1); // F / T
			} else {
				g.scale(1, 1); // F / F
			}
		}
	}

	/**
	 * Draws a swimlane if start is > 0
	 * 
	 * @param x Horizontal coordinate
	 * @param y Vertical coordinate
	 * @param w Width
	 * @param h Height
	 * @param style The associated style
	 */
	private void drawSwimline(int x, int y, int w, int h,
			Map<String, Object> style) {
		int start = mxUtils.getInt(style, mxConstants.STYLE_STARTSIZE);

		if (start == 0) {
			drawShape(x, y, w, h, style);
		} else {
			start = (int) Math.round(start * scale);

			// Removes some styles to draw the content area
			Map<String, Object> cloned = new Hashtable<String, Object>(
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
	}
	
    /**
     * test if the angle correspond to the NORTH or SOUTH sides.
     * @param angle The rotation value
     * @return true if the angle is NORTH or SOUTH side value, false otherwise.
     */
    private static boolean isNearHorizontalSide(double angle) {
    	return ((angle - ROTATION_STEP) % (MAX_ROTATION / 2)) == 0;
    }
}


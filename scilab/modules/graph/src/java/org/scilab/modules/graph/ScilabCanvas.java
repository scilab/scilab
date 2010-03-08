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
import java.awt.Color;
import java.awt.Component;
import java.awt.Composite;
import java.awt.Image;
import java.awt.Stroke;
import java.awt.geom.AffineTransform;
import java.awt.geom.Rectangle2D;
import java.io.File;
import java.net.URL;
import java.util.Hashtable;
import java.util.Map;

import javax.swing.Icon;

import org.apache.batik.gvt.GraphicsNode;
import org.scilab.modules.graph.utils.MathMLRenderUtils;
import org.scilab.modules.graph.utils.ScilabConstants;
import org.scilab.modules.graph.utils.ScilabGraphUtils;
import org.scilab.modules.graph.view.SupportedLabelType;
import org.xml.sax.SAXException;

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
	
	/** The border size between the background image and the icon image */
	private static final int BORDER_SIZE = 4;
	
	private URL svgBackgroundImage; 
	
	/** Default constructor */
	public ScilabCanvas() { }
	
	/**
	 * @param svgBackgroundImage the svgBackgroundImage to set
	 */
	public void setSvgBackgroundImage(URL svgBackgroundImage) {
		this.svgBackgroundImage = svgBackgroundImage;
	}

	/**
	 * @return the svgBackgroundImage
	 */
	public URL getSvgBackgroundImage() {
		return svgBackgroundImage;
	}

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

	/**
	 * Draws the specified markup.
	 * 
	 * @param text
	 *            Markup to be painted.
	 * @param x
	 *            X-coordinate of the text.
	 * @param y
	 *            Y-coordinate of the text.
	 * @param w
	 *            Width of the text.
	 * @param h
	 *            Height of the text.
	 * @param style
	 *            Style to be used for painting the text.
	 */
    @Override
    protected void drawHtmlText(String text, int x, int y, int w, int h,
    		Map<String, Object> style) {

    	SupportedLabelType type = SupportedLabelType.getFromText(text);
    	
    	switch (type) {
		case Latex:
			try {
				drawLatexText(ScilabGraphUtils.getTexIcon(text), x, y, w, h, style);
	    	} catch (RuntimeException e) {
				super.drawHtmlText(text, x, y, w, h, style);
	    	}
			break;
			
		case MathML:
			try {
				drawMathMLText(MathMLRenderUtils.getMathMLComponent(text), x, y, w, h, style);
			} catch (SAXException e) {
				super.drawHtmlText(text, x, y, w, h, style);
			}
			break;

		default:
			super.drawHtmlText(text, x, y, w, h, style);
			break;
		}
    }

	/**
	 * Draws the specified Latex markup
	 * 
	 * @param icon Latex icon to be painted.
	 * @param x X-coordinate of the text.
	 * @param y Y-coordinate of the text.
	 * @param w Width of the text.
	 * @param h Height of the text.
	 * @param style Style to be used for painting the text.
	 */
	protected void drawLatexText(Icon icon, int x, int y, int w, int h,
			Map<String, Object> style) {
		if (rendererPane != null) {
			if (g.hitClip(x, y, w, h)) {
				AffineTransform at = g.getTransform();

				int sx = (int) (x / scale) + mxConstants.LABEL_INSET;
				int sy = (int) (y / scale) + mxConstants.LABEL_INSET;
				g.scale(scale, scale);
				Color text = mxUtils.getColor(style, mxConstants.STYLE_FONTCOLOR, Color.BLACK);
				rendererPane.setForeground(text);
				icon.paintIcon(rendererPane, g, sx, sy);

				// Restores the previous transformation
				g.setTransform(at);
			}
		}
	}
	
	/**
	 * Draws the specified MathML markup
	 * 
	 * @param comp the component to be painted.
	 * @param x X-coordinate of the text.
	 * @param y Y-coordinate of the text.
	 * @param w Width of the text.
	 * @param h Height of the text.
	 * @param style Style to be used for painting the text.
	 */
	protected void drawMathMLText(Component comp, int x, int y, int w, int h,
			Map<String, Object> style) {

		if (rendererPane != null) {
			if (g.hitClip(x, y, w, h)) {
				AffineTransform at = g.getTransform();
				
				g.scale(scale, scale);
				Color text = mxUtils.getColor(style, mxConstants.STYLE_FONTCOLOR, Color.BLACK);
				rendererPane.setForeground(text);
				rendererPane.paintComponent(g, comp, rendererPane,
						(int) (x / scale) + mxConstants.LABEL_INSET,
						(int) (y / scale) + mxConstants.LABEL_INSET,
						(int) (w / scale), (int) (h / scale), true);
				
				// Restores the previous transformation
				g.setTransform(at);
			}
		}
	}

	/**
	 * Draws an image for the given parameters. This function handle all the awt
	 * supported {@link Image} plus the SVG format.
	 * 
	 * Painting an SVG file, first paint the background image (
	 * {@link #setSvgBackgroundImage(File)}) and then paint the icon image.
	 * 
	 * @param x
	 *            X-coordinate of the image.
	 * @param y
	 *            Y-coordinate of the image.
	 * @param w
	 *            Width of the image.
	 * @param h
	 *            Height of the image.
	 * @param image
	 *            URL of the image.
	 * @see com.mxgraph.canvas.mxGraphics2DCanvas#drawImage(int, int, int, int,
	 *      java.lang.String)
	 */
	@Override
	protected void drawImage(int x, int y, int w, int h, String image) {
		if (image.endsWith(".svg")) {
			// Translate from (0,0) to icon base point.
			g.translate(x, y);
			
			// Paint the background image if applicable
			if (svgBackgroundImage != null) {
				paintSvgBackgroundImage(w, h);
			}
			
			paintSvgForegroundImage(w, h, image);
		} else {
			super.drawImage(x, y, w, h, image);
		}
	}

	/**
	 * Paint the background image.
	 * 
	 * @param w background width
	 * @param h background height
	 */
	private void paintSvgBackgroundImage(int w, int h) {
		GraphicsNode background = ScilabGraphUtils
				.getSVGComponent(new File(svgBackgroundImage.toString()));
		
		if (background == null) {
			return;
		}
		
		// Scale to the bounds
		Rectangle2D bounds = background.getBounds();
		
		double sh = h / bounds.getHeight();
		double sw = w / bounds.getWidth();
		
		AffineTransform scaleTransform = new AffineTransform(new double[] {
		          sw,   0.0,
		         0.0,     sh
		});
		
		background.setTransform(scaleTransform);
		
		// Paint
		background.paint(g);
	}
	
	/**
	 * Paint the foreground image.
	 * 
	 * This method paint an iso-scaled and centered image.
	 * 
	 * @param w the width
	 * @param h the height
	 * @param image the current image
	 */
	private void paintSvgForegroundImage(int w, int h, String image) {
		File f = new File(image);
		GraphicsNode icon = ScilabGraphUtils.getSVGComponent(f);
		
		if (icon == null || icon.getBounds() == null) {
			return;
		}
		
		// Iso scale to the bounds - border size
		Rectangle2D bounds = icon.getBounds();
		
		double sh = h / (bounds.getHeight() + (2 * BORDER_SIZE));
		double sw = w / (bounds.getWidth() + (2 * BORDER_SIZE));
		
		double ratio;
		double tx;
		double ty;
		if (sh > sw) {
			ratio = sw;
			tx = BORDER_SIZE * ratio;
			ty = (h - (bounds.getHeight() * ratio)) / 2;
		} else {
			ratio = sh;
			tx = (w - (bounds.getWidth() * ratio)) / 2;
			ty = BORDER_SIZE * ratio;
		}
		
		AffineTransform isoScaleTransform = new AffineTransform(new double[] {
		       ratio,   0.0,
		         0.0,  ratio
		});
		icon.setTransform(isoScaleTransform);
		
		// Translate from base point to centered base point
		g.translate(tx, ty);
		
		// Paint
		icon.paint(g);
	}
}


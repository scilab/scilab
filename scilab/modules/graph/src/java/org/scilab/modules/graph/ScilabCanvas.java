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

import java.awt.Color;
import java.awt.Graphics2D;
import java.awt.Image;
import java.awt.Rectangle;
import java.awt.RenderingHints;
import java.awt.geom.AffineTransform;
import java.awt.geom.Dimension2D;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.Map;

import org.apache.batik.ext.awt.RenderingHintsKeyExt;
import org.apache.batik.gvt.GraphicsNode;
import org.apache.commons.logging.LogFactory;
import org.scilab.modules.graph.shape.LatexTextShape;
import org.scilab.modules.graph.shape.MathMLTextShape;
import org.scilab.modules.graph.utils.MathMLRenderUtils;
import org.scilab.modules.graph.utils.ScilabGraphConstants;
import org.scilab.modules.graph.utils.ScilabGraphUtils;
import org.scilab.modules.graph.view.SupportedLabelType;
import org.xml.sax.SAXException;

import com.mxgraph.shape.mxITextShape;
import com.mxgraph.swing.view.mxInteractiveCanvas;
import com.mxgraph.util.mxConstants;
import com.mxgraph.util.mxRectangle;
import com.mxgraph.util.mxUtils;
import com.mxgraph.view.mxCellState;

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

	/** The border ratio between the background image and the icon image */
	private static final double BORDER_RATIO = 0.9;
	
	static {
		putTextShape(SupportedLabelType.Latex.name(), new LatexTextShape());
		putTextShape(SupportedLabelType.MathML.name(), new MathMLTextShape());
	}
	
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
	 * Get the text shape associated with the text
	 * 
	 * @param text the associated text
	 * @param style the current style
	 * @param html true, if the text is html formatted, false otherwise.
	 * @return the associated text shape
	 */
	public mxITextShape getTextShape(String text, Map<String, Object> style,
			boolean html) {
		final mxITextShape ret;
		
		final SupportedLabelType type;
		if (html) {
			type = SupportedLabelType.getFromHTML(text);
		} else {
			type = SupportedLabelType.getFromText(text);
		}
		
		switch (type) {
		case Latex:
			try {
				// parse the text and cache it if valid. Will throw an exception
				// if the text is not valid.
				ScilabGraphUtils.getTexIcon(text);
				
				ret = textShapes.get(type.name());
			} catch (RuntimeException e) {
				return super.getTextShape(style, html);
			}
			break;

		case MathML:
			try {
				// parse the text and cache it if valid. Will throw an exception
				// if the text is not valid.
				MathMLRenderUtils.getMathMLComponent(text);
				
				ret = textShapes.get(type.name());
			} catch (SAXException e) {
				return super.getTextShape(style, html);
			}
			break;

		default:
			ret = super.getTextShape(style, html);
		break;
		}
		
		return ret;
	}
	
	/**
	 * Scale the graphic context depending on the "flip and "mirror" properties
	 * @param temporaryGraphics the current graphic surface
	 * @param style Style contents
	 * @param bounds the current bounds
	 */
	private void applyFlipAndMirror(Graphics2D temporaryGraphics, Map<String, Object> style, mxRectangle bounds) {
		if (bounds == null) {
			return;
		}
		
		final boolean flip = mxUtils.isTrue(style, ScilabGraphConstants.STYLE_FLIP, false);
		final boolean mirror = mxUtils.isTrue(style, ScilabGraphConstants.STYLE_MIRROR, false);
		
		final double x = bounds.getCenterX();
		final double y = bounds.getCenterY();

		temporaryGraphics.translate(x, y);
		
		// scale, 1st flip, 2nd mirror
		// The scale operation concatenate AffineTransforms.
		
		if (flip) {
			temporaryGraphics.scale(1.0, -1.0);
		}
		
		if (mirror) {
			temporaryGraphics.scale(-1.0, 1.0);
		}
		
		temporaryGraphics.translate(-x, -y);
	}
	
	/**
	 * Allocate a new graphic surface and set some properties on it.
	 * 
	 * This method handle the flip and the mirror properties.
	 * 
	 * @param style the current style
	 * @param opacity the opacity
	 * @param bounds the bounds
	 * @return a graphic surface
	 * @see com.mxgraph.canvas.mxGraphics2DCanvas#createTemporaryGraphics(java.util.Map, float, com.mxgraph.util.mxRectangle)
	 */
	@Override
	protected Graphics2D createTemporaryGraphics(Map<String, Object> style,
			float opacity, mxRectangle bounds) {
		Graphics2D temporaryGraphics = super.createTemporaryGraphics(style, opacity, bounds);
		
		applyFlipAndMirror(temporaryGraphics, style, bounds);
		
		return temporaryGraphics;
	}

	/**
	 * Draw the text label on the cell state.
	 * 
	 * @param text the current text
	 * @param state the cell state
	 * @param html true, if the text may be HTML, false otherwise.
	 * @return the associated shape
	 * @see com.mxgraph.canvas.mxGraphics2DCanvas#drawLabel(java.lang.String, com.mxgraph.view.mxCellState, boolean)
	 */
	@Override
	public Object drawLabel(String text, mxCellState state, boolean html) {
		Map<String, Object> style = state.getStyle();
		mxITextShape shape = getTextShape(text, style, html);
		
		if (g != null && shape != null && drawLabels && text != null
				&& text.length() > 0)
		{
			mxRectangle translatedBounds = new mxRectangle(state.getLabelBounds());
			translatedBounds.setX(translatedBounds.getX() + translate.x);
			translatedBounds.setY(translatedBounds.getY() + translate.y);

			// Creates a temporary graphics instance for drawing this shape
			Graphics2D previousGraphics = g;
			g = createTemporaryGraphics(style, 100, null);

			// Draws the label background and border
			Color bg = mxUtils.getColor(style,
					mxConstants.STYLE_LABEL_BACKGROUNDCOLOR);
			Color border = mxUtils.getColor(style,
					mxConstants.STYLE_LABEL_BORDERCOLOR);
			paintRectangle(translatedBounds, bg, border);

			// Paints the label and restores the graphics object
			shape.paintShape(this, text, translatedBounds, style);
			g.dispose();
			g = previousGraphics;
		}
		
		return shape;
	}
	
	/**
	 * Paint the image.
	 * 
	 * This function handle all the awt supported {@link Image} plus the SVG
	 * format.
	 * 
	 * @param bounds
	 *            the current bounds
	 * @param style
	 *            the current style
	 * @see com.mxgraph.canvas.mxGraphics2DCanvas#paintImage(com.mxgraph.util.mxRectangle,
	 *      java.util.Map)
	 */
	@Override
	public void paintImage(mxRectangle bounds, Map<String, Object> style) {
		String image = getImageForStyle(style);
		
		if (image != null) {
			if (image.endsWith(".svg")) {
				// Remove the "Graphics2D from BufferedImage lacks BUFFERED_IMAGE hint"
				// message and tweak Batik rendering options to increase performance.
				g.setRenderingHint(RenderingHints.KEY_ANTIALIASING,
						RenderingHints.VALUE_ANTIALIAS_ON);
				g.setRenderingHint(RenderingHints.KEY_INTERPOLATION,
						RenderingHints.VALUE_INTERPOLATION_NEAREST_NEIGHBOR);
				g.setRenderingHint(RenderingHintsKeyExt.KEY_TRANSCODING,
						RenderingHintsKeyExt.VALUE_TRANSCODING_PRINTING);
				
				Rectangle rect = bounds.getRectangle();
				
				// Translate from (0,0) to icon base point.
				g.translate(rect.x, rect.y);
				
				// Paint the background image if applicable
				if (svgBackgroundImage != null) {
					paintSvgBackgroundImage(rect.width, rect.height);
				}

				paintSvgForegroundImage(rect.width, rect.height, image);

			} else {
				super.paintImage(bounds, style);
			}
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
				.getSVGComponent(svgBackgroundImage);

		if (background == null) {
			return;
		}

		// Scale to the bounds
		Dimension2D bounds = ScilabGraphUtils.getSVGDocumentSizes(svgBackgroundImage);

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
		/*
		 * Fetch SVG file representation
		 */
		URL url;
		try {
			url = new URL(image);
		} catch (MalformedURLException e) {
			LogFactory.getLog(ScilabCanvas.class).error(e);
			return;
		}
		GraphicsNode icon = ScilabGraphUtils.getSVGComponent(url);

		if (icon == null || icon.getBounds() == null) {
			return;
		}

		/*
		 * Perform calculations
		 */

		// Iso scale to the bounds - border size
		Dimension2D bounds = ScilabGraphUtils.getSVGDocumentSizes(url);

		// Calculating icon bordered bounds
		final double ih = bounds.getHeight();
		final double iw = bounds.getWidth(); 

		// Calculate per axis scaling factor
		final double shFactor = h / ih;
		final double swFactor = w / iw;

		// Calculate the default ratio (iso scaling)
		double ratio;
		if (shFactor > swFactor) {
			ratio = swFactor;
		} else {
			ratio = shFactor;
		}

		// Adding borders
		ratio *= BORDER_RATIO;

		// Calculate scaled height and width
		final double sh = ratio * ih;
		final double sw = ratio * iw;

		// Center the image on the block
		double tx = (w - sw) / 2;
		double ty = (h - sh) / 2;

		/*
		 * Everything has been calculated, render now.
		 */

		// Translate from base point to centered base point
		g.translate(tx, ty);

		// scale to the ratio
		g.scale(ratio, ratio);

		// Paint
		icon.paint(g);
	}
}


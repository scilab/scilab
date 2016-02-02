/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Clement DAVID
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

package org.scilab.modules.graph;

import java.awt.Color;
import java.awt.Graphics2D;
import java.awt.geom.Dimension2D;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.Map;
import java.util.logging.Logger;

import org.apache.batik.gvt.GraphicsNode;
import org.scilab.forge.jlatexmath.TeXFormula;
import org.scilab.modules.graph.shape.LatexTextShape;
import org.scilab.modules.graph.shape.MathMLTextShape;
import org.scilab.modules.graph.shape.SvgShape;
import org.scilab.modules.graph.utils.MathMLRenderUtils;
import org.scilab.modules.graph.utils.ScilabGraphConstants;
import org.scilab.modules.graph.utils.ScilabGraphUtils;
import org.scilab.modules.graph.view.SupportedLabelType;
import org.xml.sax.SAXException;

import com.mxgraph.canvas.mxGraphics2DCanvas;
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
    /** The max valid rotation value (always 360 degres) */
    public static final int MAX_ROTATION = 360;

    /** The border ratio between the background image and the icon image */
    private static final double BORDER_RATIO = 0.9;

    static {
        putShape(mxConstants.SHAPE_LABEL, new SvgShape());
        putTextShape(SupportedLabelType.Latex.name(), new LatexTextShape());
        putTextShape(SupportedLabelType.MathML.name(), new MathMLTextShape());
    }

    private URL urlBasePath;

    /** Default constructor */
    public ScilabCanvas() {
    }

    /**
     * Get the text shape associated with the text
     *
     * @param text
     *            the associated text
     * @param style
     *            the current style
     * @param html
     *            true, if the text is html formatted, false otherwise.
     * @return the associated text shape
     */
    public mxITextShape getTextShape(String text, Map<String, Object> style, boolean html) {
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
                    new TeXFormula(SupportedLabelType.Latex.escape(text));

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
     *
     * @param temporaryGraphics
     *            the current graphic surface
     * @param style
     *            Style contents
     * @param bounds
     *            the current bounds
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
     * @param style
     *            the current style
     * @param opacity
     *            the opacity
     * @param bounds
     *            the bounds
     * @return a graphic surface
     * @see com.mxgraph.canvas.mxGraphics2DCanvas#createTemporaryGraphics(java.util.Map,
     *      float, com.mxgraph.util.mxRectangle)
     */
    @Override
    public Graphics2D createTemporaryGraphics(Map<String, Object> style, float opacity, mxRectangle bounds) {
        Graphics2D temporaryGraphics = super.createTemporaryGraphics(style, opacity, bounds);

        applyFlipAndMirror(temporaryGraphics, style, bounds);

        return temporaryGraphics;
    }

    /**
     * Draw the text label on the cell state.
     *
     * This method is extracted from {@link mxGraphics2DCanvas} to add a text
     * argument to {@link #getTextShape(Map, boolean)}.
     *
     * @param text
     *            the current text
     * @param state
     *            the cell state
     * @param html
     *            true, if the text may be HTML, false otherwise.
     * @return the associated shape
     * @see com.mxgraph.canvas.mxGraphics2DCanvas#drawLabel(java.lang.String,
     *      com.mxgraph.view.mxCellState, boolean)
     */
    @Override
    public Object drawLabel(String text, mxCellState state, boolean html) {
        Map<String, Object> style = state.getStyle();
        mxITextShape shape = getTextShape(text, style, html);

        if (g != null && shape != null && drawLabels && text != null && text.length() > 0) {
            // Creates a temporary graphics instance for drawing this shape
            float opacity = mxUtils.getFloat(style, mxConstants.STYLE_TEXT_OPACITY, 100);
            Graphics2D previousGraphics = g;
            g = createTemporaryGraphics(style, opacity, null);

            // Draws the label background and border
            Color bg = mxUtils.getColor(style, mxConstants.STYLE_LABEL_BACKGROUNDCOLOR);
            Color border = mxUtils.getColor(style, mxConstants.STYLE_LABEL_BORDERCOLOR);
            paintRectangle(state.getLabelBounds().getRectangle(), bg, border);

            // Paints the label and restores the graphics object
            shape.paintShape(this, text, state, style);
            g.dispose();
            g = previousGraphics;
        }

        return shape;
    }

    /**
     * Paint the foreground image.
     *
     * This method paint an iso-scaled and centered image.
     *
     * @param w
     *            the width
     * @param h
     *            the height
     * @param image
     *            the current image
     */
    public void paintSvgForegroundImage(int w, int h, String image) {
        /*
         * Fetch SVG file representation
         */
        URL url;
        try {
            url = new URL(image);
        } catch (MalformedURLException e) {
            Logger.getLogger(ScilabCanvas.class.getName()).severe(e.toString());
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

    /**
     * Set the image path and store the path as a URL.
     *
     * @param imageBasePath
     *            the new path
     * @see com.mxgraph.canvas.mxBasicCanvas#setImageBasePath(java.lang.String)
     */
    @Override
    public void setImageBasePath(String imageBasePath) {
        super.setImageBasePath(imageBasePath);

        try {
            this.urlBasePath = new URL(imageBasePath);
        } catch (MalformedURLException e) {
            Logger.getLogger(ScilabCanvas.class.getName()).severe(e.toString());
        }
    }

    /**
     * Gets the image path from the given style. If the path is relative (does
     * not start with a slash) then it is appended to the imageBasePath.
     *
     * @param style
     *            the current style
     * @return the image path
     */
    @Override
    public String getImageForStyle(Map<String, Object> style) {
        String filename = mxUtils.getString(style, mxConstants.STYLE_IMAGE);

        if (filename == null) {
            return null;
        }

        try {
            return new URL(this.urlBasePath, filename).toExternalForm();
        } catch (MalformedURLException e) {
        }

        return null;
    }
}

/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010-2010 - DIGITEO - Clement DAVID <clement.david@scilab.org>
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

package org.scilab.modules.graph.shape;

import java.awt.Rectangle;
import java.awt.RenderingHints;
import java.awt.Shape;
import java.awt.geom.AffineTransform;

import org.apache.batik.ext.awt.RenderingHintsKeyExt;
import org.scilab.modules.graph.ScilabCanvas;

import com.mxgraph.canvas.mxGraphics2DCanvas;
import com.mxgraph.shape.mxLabelShape;
import com.mxgraph.util.mxConstants;
import com.mxgraph.util.mxUtils;
import com.mxgraph.view.mxCellState;

/**
 * Add the SVG capability to the standard Image shape
 */
public class SvgShape extends mxLabelShape {
    /**
     * Paint the shape
     *
     * @param canvas the current canvas
     * @param state the current state
     * @see com.mxgraph.shape.mxImageShape#paintShape(com.mxgraph.canvas.mxGraphics2DCanvas, com.mxgraph.view.mxCellState)
     */
    @Override
    public void paintShape(mxGraphics2DCanvas canvas, mxCellState state) {
        // paint previously set background
        super.paintShape(canvas, state);

        final String image = getImageForStyle(canvas, state);
        if (image != null && image.endsWith(".svg")) {
            // Remove the "Graphics2D from BufferedImage lacks BUFFERED_IMAGE hint"
            // message and tweak Batik rendering options to increase performance.
            canvas.getGraphics().setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
            canvas.getGraphics().setRenderingHint(RenderingHints.KEY_INTERPOLATION, RenderingHints.VALUE_INTERPOLATION_NEAREST_NEIGHBOR);
            canvas.getGraphics().setRenderingHint(RenderingHintsKeyExt.KEY_TRANSCODING, RenderingHintsKeyExt.VALUE_TRANSCODING_PRINTING);

            final Rectangle rect = getImageBounds(canvas, state);
            canvas.getGraphics().translate(rect.x, rect.y);

            ((ScilabCanvas) canvas).paintSvgForegroundImage(rect.width, rect.height, image);

            if (mxUtils.isTrue(state.getStyle(), mxConstants.STYLE_GLASS, false)) {
                drawGlassEffect(canvas, state);
            }
        } else {
            super.paintShape(canvas, state);
        }
    }

    /**
     * Get the image bounds for an SVG image
     *
     * @param canvas the current canvas
     * @param state the current state
     * @return the bounds of the image
     * @see com.mxgraph.shape.mxImageShape#getImageBounds(com.mxgraph.canvas.mxGraphics2DCanvas, com.mxgraph.view.mxCellState)
     */
    @Override
    public Rectangle getImageBounds(mxGraphics2DCanvas canvas, mxCellState state) {
        final int rotation = (int) mxUtils.getDouble(state.getStyle(),
                             mxConstants.STYLE_ROTATION, 0);
        final AffineTransform transform = new AffineTransform();
        transform.rotate(Math.toRadians(rotation), state.getCenterX(),
                         state.getCenterY());
        final Shape shape = transform.createTransformedShape(state
                            .getRectangle());
        return shape.getBounds();
    }
}

/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
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
import java.awt.Graphics2D;
import java.awt.Rectangle;
import java.awt.geom.AffineTransform;
import java.util.Map;

import javax.swing.CellRendererPane;
import javax.swing.Icon;

import org.scilab.modules.graph.utils.ScilabGraphUtils;

import com.mxgraph.canvas.mxGraphics2DCanvas;
import com.mxgraph.shape.mxITextShape;
import com.mxgraph.util.mxConstants;
import com.mxgraph.util.mxUtils;
import com.mxgraph.view.mxCellState;

/**
 * Implement a text shape that can draw LaTEX text.
 */
public class LatexTextShape implements mxITextShape {

    /**
     * Painter
     *
     * @param canvas
     *            the current canvas
     * @param text
     *            the text to render
     * @param state
     *            the current state
     * @param style
     *            the current style
     * @see com.mxgraph.shape.mxITextShape#paintShape(com.mxgraph.canvas.mxGraphics2DCanvas,
     *      java.lang.String, com.mxgraph.util.mxRectangle, java.util.Map)
     */
    @Override
    public void paintShape(mxGraphics2DCanvas canvas, String text, mxCellState state, Map<String, Object> style) {

        CellRendererPane rendererPane = canvas.getRendererPane();
        Rectangle rect = state.getLabelBounds().getRectangle();
        if (rect.isEmpty()) {
            rect.grow(1, 1);
        }
        Graphics2D g = canvas.getGraphics();

        if (rendererPane != null && (g.getClipBounds() == null || g.getClipBounds().intersects(rect))) {
            final double scale = canvas.getScale();
            final int x = rect.x;
            final int y = rect.y;
            final int w = rect.width;
            final int h = rect.height;

            if (g.hitClip(x, y, w, h)) {
                AffineTransform at = g.getTransform();

                /*
                 * do not scale x nor y, due to the g.scale() call.
                 */
                final double sx = x;
                final double sy = y;

                final double sw = w / scale;
                final double sh = h / scale;
                g.scale(scale, scale);

                // handle text color
                Color textColor = mxUtils.getColor(style, mxConstants.STYLE_FONTCOLOR, Color.BLACK);
                rendererPane.setForeground(textColor);

                // TODO: handle horizontal align
                final Object align = mxUtils.getString(style, mxConstants.STYLE_ALIGN, mxConstants.ALIGN_CENTER);
                final double horizAlignProportion;
                if (align.equals(mxConstants.ALIGN_LEFT)) {
                    horizAlignProportion = 0;
                } else if (align.equals(mxConstants.ALIGN_RIGHT)) {
                    horizAlignProportion = 1.0;
                } else {
                    horizAlignProportion = 0.5;
                }

                // TODO: handle vertical align
                final Object vertAlign = mxUtils.getString(style, mxConstants.STYLE_VERTICAL_ALIGN, mxConstants.ALIGN_MIDDLE);
                final double vertAlignProportion;
                if (vertAlign.equals(mxConstants.ALIGN_TOP)) {
                    vertAlignProportion = 0;
                } else if (vertAlign.equals(mxConstants.ALIGN_BOTTOM)) {
                    vertAlignProportion = 1.0;
                } else {
                    vertAlignProportion = 0.5;
                }

                // parse the text and cache it if valid. Will throw an exception
                // if the text is not valid but the text must have been already
                // checked on ScilabCanvas#getTextShape(...).
                final Icon icon = ScilabGraphUtils.getTexIcon(text);
                final int iw = icon.getIconWidth();
                final int ih = icon.getIconHeight();

                final double dx = (sw - iw) / 2;
                final double dy = (sh - ih) / 2;

                icon.paintIcon(rendererPane, g, (int) (sx + dx), (int) (sy + dy));

                // Restores the previous transformation
                g.setTransform(at);
            }
        }
    }

}

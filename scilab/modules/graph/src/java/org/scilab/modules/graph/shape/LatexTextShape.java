/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
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

import java.awt.Color;
import java.awt.Graphics2D;
import java.awt.geom.AffineTransform;
import java.util.Map;

import javax.swing.CellRendererPane;
import javax.swing.Icon;

import org.scilab.modules.graph.utils.ScilabGraphUtils;

import com.mxgraph.canvas.mxGraphics2DCanvas;
import com.mxgraph.shape.mxITextShape;
import com.mxgraph.util.mxConstants;
import com.mxgraph.util.mxRectangle;
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
        mxRectangle rect = state.getLabelBounds();
        Graphics2D g = canvas.getGraphics();

        if (rendererPane != null && (g.getClipBounds() == null || g.getClipBounds().intersects(rect.getRectangle()))) {
            final double scale = canvas.getScale();
            final double x = rect.getX();
            final double y = rect.getY();
            final double w = rect.getWidth();
            final double h = rect.getHeight();

            AffineTransform at = g.getTransform();

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
            float fontSize = (float) (mxUtils.getInt(style, mxConstants.STYLE_FONTSIZE, 16) * scale);
            final Icon icon = ScilabGraphUtils.getTexIcon(text, fontSize);
            // the icon is generated scaled so width and height are already scaled
            final double ih = icon.getIconHeight();
            final double iw = icon.getIconWidth();


            double dx = (iw - w) / 2;
            double dy = (ih - h) / 2;

            icon.paintIcon(rendererPane, g,
                           (int) (x + dx) + mxConstants.LABEL_INSET ,
                           (int) (y + dy) + mxConstants.LABEL_INSET);

            // Restores the previous transformation
            g.setTransform(at);
        }
    }

}

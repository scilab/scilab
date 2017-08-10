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
import java.awt.Component;
import java.awt.Graphics2D;
import java.awt.Rectangle;
import java.awt.geom.AffineTransform;
import java.util.Map;
import java.util.logging.Logger;

import javax.swing.CellRendererPane;

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
        Rectangle rect = state.getRectangle();
        Graphics2D g = canvas.getGraphics();

        if (rendererPane != null && (g.getClipBounds() == null || g.getClipBounds().intersects(rect))) {
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
                    rendererPane.paintComponent(g, comp, rendererPane, (int) (x / scale) + mxConstants.LABEL_INSET,
                                                (int) (y / scale) + mxConstants.LABEL_INSET, (int) (w / scale), (int) (h / scale), true);

                    // Restores the previous transformation
                    g.setTransform(at);
                } catch (SAXException e) {
                    Logger.getLogger(MathMLTextShape.class.getName()).severe(e.toString());
                }
            }
        }
    }

}

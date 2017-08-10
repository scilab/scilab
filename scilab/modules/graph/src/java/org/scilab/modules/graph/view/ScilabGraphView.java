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

package org.scilab.modules.graph.view;

import java.awt.Component;
import java.util.Map;

import javax.swing.Icon;

import org.scilab.modules.graph.utils.MathMLRenderUtils;
import org.scilab.modules.graph.utils.ScilabGraphUtils;
import org.scilab.modules.gui.messagebox.ScilabModalDialog;
import org.xml.sax.SAXException;

import com.mxgraph.model.mxCell;
import com.mxgraph.util.mxConstants;
import com.mxgraph.util.mxPoint;
import com.mxgraph.util.mxRectangle;
import com.mxgraph.util.mxUtils;
import com.mxgraph.view.mxCellState;
import com.mxgraph.view.mxGraph;
import com.mxgraph.view.mxGraphView;

/**
 * Implement specific method to render a graph
 */
public class ScilabGraphView extends mxGraphView {

    /**
     * Default constructor
     * @param graph the associated graph
     */
    public ScilabGraphView(mxGraph graph) {
        super(graph);
    }

    /**
     * Updates the label bounds in the given state.
     * @param state the cell visible state
     */
    @Override
    public void updateLabelBounds(mxCellState state) {
        Object cell = state.getCell();
        Map<String, Object> style = state.getStyle();
        String label = graph.getLabel(cell);

        SupportedLabelType type = SupportedLabelType.getFromHTML(label);
        mxRectangle labelBounds;
        double w;
        double h;

        switch (type) {
            case Latex:
                /*
                 * As we render text as an image, the label bounds are set to the
                 * scaled generated image values.
                 */
                try {
                    float fontSize = (float) (mxUtils.getInt(style, mxConstants.STYLE_FONTSIZE, 16) * scale);

                    final Icon icon = ScilabGraphUtils.getTexIcon(label, fontSize);
                    // the icon is generated scaled so width and height are already scaled
                    w = icon.getIconWidth();
                    h = icon.getIconHeight();

                    final mxPoint offset = state.getAbsoluteOffset();
                    double x = offset.getX();
                    double y = offset.getY();

                    final mxRectangle vertexBounds;
                    if (!graph.getModel().isEdge(cell)) {
                        vertexBounds = state;

                        x += vertexBounds.getX();
                        y += vertexBounds.getY();;

                        // the label is always centered
                        x -= (w - vertexBounds.getWidth()) / 2 ;
                        y -= (h - vertexBounds.getHeight()) / 2 ;
                    }

                    labelBounds = new mxRectangle(x, y, w, h);
                } catch (Exception e) {
                    // popup an error
                    // FIXME: use a ScilabGraphTab instead of null there
                    ScilabModalDialog.show(null, e.getLocalizedMessage());

                    // Set a non-rendering label on the model
                    // label will be printed (state contains the value)
                    mxCell c = (mxCell) cell;
                    c.setValue(((String) c.getValue()).substring(1));
                    labelBounds = getDefaultBounds(state, cell, style, label);
                }
                break;

            case MathML:
                /*
                 * As we render text as an image, the label bounds are set to the
                 * scaled generated image values.
                 */
                try {
                    Component comp = MathMLRenderUtils.getMathMLComponent(label);
                    w = comp.getWidth();
                    h = comp.getHeight();

                    final mxPoint offset = state.getOrigin();
                    final mxRectangle size = new mxRectangle();
                    size.setWidth(w);
                    size.setHeight(h);

                    labelBounds = mxUtils.getScaledLabelBounds(offset.getX(),
                                  offset.getY(), size, state.getWidth(),
                                  state.getHeight(), style, scale);
                } catch (SAXException e) {
                    // popup an error
                    // FIXME: use a ScilabGraphTab instead of null there
                    ScilabModalDialog.show(null, e.getLocalizedMessage());

                    // Set a non-rendering label on the model
                    // label will be printed (state contains the value)
                    mxCell c = (mxCell) cell;
                    c.setValue(((String) c.getValue()).substring(1));
                    labelBounds = getDefaultBounds(state, cell, style, label);
                }
                break;

            default:
                labelBounds = getDefaultBounds(state, cell, style, label);
                break;
        }

        state.setLabelBounds(labelBounds);
    }

    /**
     * Return the default bounds calculated as if the label were text.
     * @param state the cell state
     * @param cell the current cell
     * @param style the cell style
     * @param label the current text
     * @return the calculated bounds
     */
    private mxRectangle getDefaultBounds(mxCellState state, Object cell,
                                         Map<String, Object> style, String label) {
        mxRectangle labelBounds;
        mxRectangle vertexBounds;

        if (!graph.getModel().isEdge(cell)) {
            vertexBounds = state;
        } else {
            vertexBounds = null;
        }

        labelBounds = mxUtils.getLabelPaintBounds(label, style, graph
                      .isHtmlLabel(cell), state.getAbsoluteOffset(), vertexBounds,
                      scale);
        return labelBounds;
    }
}

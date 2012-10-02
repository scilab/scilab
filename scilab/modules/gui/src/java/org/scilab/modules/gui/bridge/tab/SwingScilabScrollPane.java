/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - 2008 Digiteo Jean-Baptiste Silvy
 * Copyright (C) 2012 - Scilab Enterprises - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.bridge.tab;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_AUTORESIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_AXES_SIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_BACKGROUND__;

import java.awt.Color;
import java.awt.Component;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.Point;
import java.awt.Rectangle;

import javax.swing.JScrollPane;
import javax.swing.ScrollPaneConstants;

import org.scilab.modules.graphic_objects.figure.Figure;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicView.GraphicView;
import org.scilab.modules.gui.bridge.canvas.SwingScilabCanvas;
import org.scilab.modules.renderer.JoGLView.util.ColorFactory;

/**
 * Scroll pane based on Swing technology
 * @author Jean-Baptiste silvy
 */
public class SwingScilabScrollPane extends JScrollPane implements ScilabScrollPane, GraphicView {

    /** Needed */
    private static final long serialVersionUID = -4262585651413643814L;

    private SwingScilabCanvas canvas;
    private Figure figure;
    private Component comp;

    /**
     * Create a new Scroll pane around an axes.
     * @param axes axes to scroll
     */
    public SwingScilabScrollPane(Component comp, SwingScilabCanvas canvas, Figure figure) {
        super(comp);
        this.comp = comp;
        this.canvas = canvas;
        this.figure = figure;
        // use the axes background as default one
        setRealBackground(canvas.getBackground());
        GraphicController.getController().register(this);

        if (figure.getAutoResize()) {
            // hide scroll bars
            setHorizontalScrollBarPolicy(ScrollPaneConstants.HORIZONTAL_SCROLLBAR_NEVER);
            setVerticalScrollBarPolicy(ScrollPaneConstants.VERTICAL_SCROLLBAR_NEVER);
        } else {
            // show scroll bars
            setHorizontalScrollBarPolicy(ScrollPaneConstants.HORIZONTAL_SCROLLBAR_AS_NEEDED);
            setVerticalScrollBarPolicy(ScrollPaneConstants.VERTICAL_SCROLLBAR_AS_NEEDED);
        }
    }

    /**
     * @return [x,y,w,h] Array of size 4 containing the position
     *         width and height of the viewPort
     */
    public int[] getViewingRegion() {
        Rectangle viewport = getViewport().getViewRect();
        int[] res = {(int) viewport.getX(),
                     (int) viewport.getY(),
                     (int) viewport.getWidth(),
                     (int) viewport.getHeight()
                    };
        return res;
    }

    /**
     * Set the background of the scrollPane
     * @param red red channel of the color
     * @param green green channel
     * @param blue blue channel
     */
    public void setBackground(double red, double green, double blue) {
        Color newColor = new Color((float) red, (float) green, (float) blue);
        setRealBackground(newColor);
    }

    /**
     * Set the displayed background of the scrollpane
     * @param background color to use as background
     */
    private void setRealBackground(Color background) {
        getViewport().setBackground(background);
    }

    /**
     * Move the viewport
     * @param xPos the x position to scroll to
     * @param yPos the y position to scroll to
     */
    public void setViewPosition(int xPos, int yPos) {
        getViewport().setViewPosition(new Point(xPos, yPos));
    }

    /**
     * @return Container representation of this object
     */
    public Container getAsContainer() {
        return this;
    }

    @Override
    public void updateObject(String id, int property) {
        // Watch figure.auto_resize = "on" | "off"
        if (figure.getIdentifier().equals(id)) {
            if (property == __GO_AUTORESIZE__) {
                Boolean autoResize = (Boolean) GraphicController.getController().getProperty(id, property);
                if (autoResize) {
                    // hide scroll bars
                    setHorizontalScrollBarPolicy(ScrollPaneConstants.HORIZONTAL_SCROLLBAR_NEVER);
                    setVerticalScrollBarPolicy(ScrollPaneConstants.VERTICAL_SCROLLBAR_NEVER);
                } else {
                    // restore them
                    setHorizontalScrollBarPolicy(ScrollPaneConstants.HORIZONTAL_SCROLLBAR_AS_NEEDED);
                    setVerticalScrollBarPolicy(ScrollPaneConstants.VERTICAL_SCROLLBAR_AS_NEEDED);
                }
            }

            if ( property == __GO_AXES_SIZE__) {
                Dimension d = new Dimension(figure.getAxesSize()[0], figure.getAxesSize()[1]);
                comp.setPreferredSize(d);
                comp.setSize(d);
                canvas.setBounds(0, 0, figure.getAxesSize()[0], figure.getAxesSize()[1]);
            }

            if (property == __GO_BACKGROUND__) {
                canvas.setBackground(ColorFactory.createColor(figure.getColorMap(), figure.getBackground()));
            }
        }

    }

    @Override
    public void createObject(String id) {
    }

    @Override
    public void deleteObject(String id) {
        if (figure.getIdentifier().equals(id)) {
            GraphicController.getController().unregister(this);
        }
    }

}

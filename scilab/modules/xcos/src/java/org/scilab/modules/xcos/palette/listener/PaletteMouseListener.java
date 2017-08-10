/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015 - Marcos CARDINOT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.palette.listener;

import java.awt.Component;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.geom.Rectangle2D;

import javax.swing.SwingUtilities;

import org.scilab.modules.xcos.palette.PaletteCtrl;
import org.scilab.modules.xcos.palette.view.PaletteBlockView;
import org.scilab.modules.xcos.palette.view.PaletteView;

/**
 * Implement the default mouse listener for the palette view
 * @author Marcos CARDINOT <mcardinot@gmail.com>
 */
public final class PaletteMouseListener extends MouseAdapter {

    private Point startPoint;

    /** Default constructor */
    public PaletteMouseListener() {
        super();
    }

    /**
     * Clear selections when palette view is clicked
     * @param e MouseEvent
     */
    @Override
    public void mouseClicked(MouseEvent e) {
        super.mouseClicked(e);
        PaletteCtrl.clearSelections();
    }

    /**
     * Invoked when the mouse is pressed
     * @param e MouseEvent
     */
    @Override
    public void mousePressed(MouseEvent e) {
        super.mousePressed(e);
        this.startPoint = e.getPoint();
    }

    /**
     * Select blocks by dragging the mouse pointer
     * @param e MouseEvent
     */
    @Override
    public void mouseDragged(MouseEvent e) {
        super.mouseDragged(e);
        if (!SwingUtilities.isLeftMouseButton(e)) {
            return;
        }

        Point endPoint = e.getPoint();
        double x = Math.min(startPoint.getX(), endPoint.getX());
        double y = Math.min(startPoint.getY(), endPoint.getY());
        double w = Math.abs(startPoint.getX() - endPoint.getX());
        double h = Math.abs(startPoint.getY() - endPoint.getY());

        Rectangle2D.Double rect = new Rectangle2D.Double(x, y, w, h);
        PaletteView view = (PaletteView) e.getSource();
        view.setSelectionRectangle(rect);

        Component[] blocks = view.getComponents();
        for (Component block : blocks) {
            Rectangle b = block.getBounds();
            boolean selected = rect.contains(b.getCenterX(), b.getCenterY());
            ((PaletteBlockView) block).getController().setSelected(selected);
        }

        // makes the scrollbar follow the selection
        view.scrollRectToVisible(new Rectangle(endPoint));
    }

    /**
     * @param e MouseEvent
     */
    @Override
    public void mouseReleased(MouseEvent e) {
        super.mouseReleased(e);
        PaletteView view = (PaletteView) e.getSource();
        view.setSelectionRectangle(null);
        Component[] blocks = view.getComponents();
        if (blocks.length > 0) {
            blocks[0].requestFocus();
        }
    }
}

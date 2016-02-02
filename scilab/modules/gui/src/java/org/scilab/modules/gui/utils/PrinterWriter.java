/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - DIGITEO - Allan CORNET
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

package org.scilab.modules.gui.utils;

import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Rectangle;
import java.awt.Shape;
import java.awt.print.PageFormat;
import java.awt.print.Printable;
import javax.swing.JEditorPane;
import javax.swing.text.View;

/**
 * Printer Writer (from a JEditorPane)
 * @author Allan CORNET - DIGITEO
 *
 */
public class PrinterWriter implements Printable {
    /**
     * boolean to allow control over whether pages too wide to fit on a page will be scaled.
     */
    private boolean scaleWidthToFit = true;

    /**
     * Used to keep track of when the page to print changes.
     */
    private int currentPage = -1;

    /**
     * Location of the current page end.
     */
    private double pageEndY;

    /**
     * Location of the current page start.
     */
    private double pageStartY;

    /**
     * Stores the JEditorPane that is being printed.
     */
    private JEditorPane pane;

    /**
     * Constructor
     * @param pane (JEditorPane)
     */
    public PrinterWriter(JEditorPane pane) {
        this.pane = pane;
    }

    /**
     * overload print method
     * @param graphics Graphics to print
     * @param pageFormat Page format
     * @param pageIndex page Index
     * @return PAGE_EXISTS or NO_SUCH_PAGE
     */
    public int print(Graphics graphics, PageFormat pageFormat, int pageIndex) {
        double scale = 1.0;
        Graphics2D graphics2D;
        View rootView;

        graphics2D = (Graphics2D) graphics;

        pane.setSize((int) pageFormat.getImageableWidth(),
                     Integer.MAX_VALUE);
        pane.validate();

        rootView = pane.getUI().getRootView(pane);

        if ((scaleWidthToFit)
                && (pane.getMinimumSize().getWidth() > pageFormat
                    .getImageableWidth())) {
            scale = pageFormat.getImageableWidth()
                    / pane.getMinimumSize().getWidth();
            graphics2D.scale(scale, scale);
        }

        graphics2D.setClip((int) (pageFormat.getImageableX() / scale),
                           (int) (pageFormat.getImageableY() / scale), (int) (pageFormat
                                   .getImageableWidth() / scale), (int) (pageFormat
                                           .getImageableHeight() / scale));

        if (pageIndex > currentPage) {
            currentPage = pageIndex;
            pageStartY += pageEndY;
            pageEndY = graphics2D.getClipBounds().getHeight();
        }

        graphics2D.translate(graphics2D.getClipBounds().getX(), graphics2D
                             .getClipBounds().getY());

        Rectangle allocation = new Rectangle(0, (int) - pageStartY,
                                             (int) (pane.getMinimumSize().getWidth()), (int) (pane
                                                     .getPreferredSize().getHeight()));

        if (printView(graphics2D, allocation, rootView)) {
            return Printable.PAGE_EXISTS;
        } else {
            pageStartY = 0;
            pageEndY = 0;
            currentPage = -1;
            return Printable.NO_SUCH_PAGE;
        }
    }

    /**
     * recursive method to print a graphics 2D object
     * @param graphics2D Graphics used
     * @param allocation Shape used to print
     * @param view view used
     * @return true or false
     */
    private boolean printView(Graphics2D graphics2D, Shape allocation, View view) {
        boolean pageExists = false;
        Rectangle clipRectangle = graphics2D.getClipBounds();
        Shape childAllocation;
        View childView;

        if (view.getViewCount() > 0
                && !view.getElement().getName().equalsIgnoreCase("td")) {
            for (int i = 0; i < view.getViewCount(); i++) {
                childAllocation = view.getChildAllocation(i, allocation);
                if (childAllocation != null) {
                    childView = view.getView(i);
                    if (printView(graphics2D, childAllocation, childView)) {
                        pageExists = true;
                    }
                }
            }
        } else {
            if (allocation.getBounds().getMaxY() >= clipRectangle.getY()) {
                pageExists = true;
                if ((allocation.getBounds().getHeight() > clipRectangle
                        .getHeight())
                        && (allocation.intersects(clipRectangle))) {
                    view.paint(graphics2D, allocation);
                } else {
                    if (allocation.getBounds().getY() >= clipRectangle
                            .getY()) {
                        if (allocation.getBounds().getMaxY() <= clipRectangle
                                .getMaxY()) {
                            view.paint(graphics2D, allocation);
                        } else {
                            if (allocation.getBounds().getY() < pageEndY) {
                                pageEndY = allocation.getBounds().getY();
                            }
                        }
                    }
                }
            }
        }
        return pageExists;
    }

}

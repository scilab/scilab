/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
 * Copyright (C) 2011-2017 - Scilab Enterprises - Clement DAVID
 * Copyright (C) 2015 - Marcos CARDINOT
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

package org.scilab.modules.xcos.palette.view;

import java.awt.AlphaComposite;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Rectangle;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;
import java.awt.geom.Rectangle2D;

import javax.swing.JPanel;
import javax.swing.Scrollable;
import javax.swing.SwingConstants;

import org.scilab.modules.xcos.utils.XcosConstants;
import org.scilab.modules.xcos.utils.XcosConstants.PaletteBlockSize;

/**
 * Implement a view of a block category.
 *
 * As the JScrollPane doesn't allow a to specify it's associated layout, we have
 * to use a mainArea to perform what we want there. Never use the
 * {@link javax.swing.JScrollPane#add(java.awt.Component)} on this class but use
 * {@link JPanel#add(java.awt.Component)} instead.
 */
@SuppressWarnings(value = { "serial" })
public class PaletteView extends JPanel implements Scrollable {

    private static Rectangle2D.Double selectionRect;
    private boolean isLoaded;

    /**
     * Default constructor
     */
    public PaletteView() {
        setName("PaletteView");
        initComponents();
    }

    /** Setup component */
    private void initComponents() {
        setBackground(Color.WHITE);

        setLayout(new ModifiedFlowLayout(FlowLayout.LEADING,
                                         XcosConstants.PALETTE_HMARGIN,
                                         XcosConstants.PALETTE_VMARGIN));

        // if this panel gains focus, try to select the first block!
        setFocusable(true);
        addFocusListener(new FocusListener() {
            @Override
            public void focusLost(FocusEvent e) {
            }
            @Override
            public void focusGained(FocusEvent e) {
                try {
                    if (getComponentCount() > 0) {
                        ((PaletteBlockView) getComponent(0)).getController().setSelected(true);
                    }
                } catch (ClassCastException err) {
                } catch (NullPointerException err) {
                }
            }
        });
    }

    /**
     * Sets the selection rectangle
     * @param rect Rectangle2D
     */
    public void setSelectionRectangle(Rectangle2D.Double rect) {
        selectionRect = rect;
        this.repaint();
    }

    /**
     * @return true if the view is already loaded
     */
    public boolean isLoaded() {
        return isLoaded;
    }

    /**
     * This class update the status of the view. If you want to save memory,
     * unload the view when hidden.
     *
     * @param b
     *            the load status
     */
    public void setLoaded(boolean b) {
        if (!b) {
            removeAll();
        }
        isLoaded = b;
    }

    /**
     * @return The prefered Scrollable dimension
     * @see javax.swing.Scrollable#getPreferredScrollableViewportSize()
     */
    @Override
    public Dimension getPreferredScrollableViewportSize() {
        return getPreferredSize();
    }

    /**
     * @param visibleRect
     *            The view area visible within the viewport
     * @param orientation
     *            Either SwingConstants.VERTICAL or SwingConstants.HORIZONTAL.
     * @param direction
     *            Less than zero to scroll up/left, greater than zero for
     *            down/right.
     * @return The "block" increment for scrolling in the specified direction.
     *         This value should always be positive.
     * @see javax.swing.Scrollable#getScrollableBlockIncrement(java.awt.Rectangle,
     *      int, int)
     */
    @Override
    public int getScrollableBlockIncrement(Rectangle visibleRect,
                                           int orientation, int direction) {
        PaletteBlockSize palBlockSize = PaletteManagerPanel.getCurrentSize();
        if (orientation == SwingConstants.VERTICAL) {
            return palBlockSize.getBlockDimension().height
                   + XcosConstants.PALETTE_VMARGIN;
        } else {
            return palBlockSize.getBlockDimension().width
                   + XcosConstants.PALETTE_HMARGIN;
        }
    }

    /**
     * @return always false
     * @see javax.swing.Scrollable#getScrollableTracksViewportHeight()
     */
    @Override
    public boolean getScrollableTracksViewportHeight() {
        return false;
    }

    /**
     * @return always true
     * @see javax.swing.Scrollable#getScrollableTracksViewportWidth()
     */
    @Override
    public boolean getScrollableTracksViewportWidth() {
        return true;
    }

    /**
     * @param visibleRect
     *            The view area visible within the viewport
     * @param orientation
     *            Either SwingConstants.VERTICAL or SwingConstants.HORIZONTAL.
     * @param direction
     *            Less than zero to scroll up/left, greater than zero for
     *            down/right.
     * @return PALETTE_BLOCK_HEIGHT or PALETTE_BLOCK_WIDTH depending on
     *         direction.
     * @see javax.swing.Scrollable#getScrollableUnitIncrement(java.awt.Rectangle,
     *      int, int)
     */
    @Override
    public int getScrollableUnitIncrement(Rectangle visibleRect,
                                          int orientation, int direction) {
        PaletteBlockSize palBlockSize = PaletteManagerPanel.getCurrentSize();
        if (orientation == SwingConstants.VERTICAL) {
            return palBlockSize.getBlockDimension().height;
        } else {
            return palBlockSize.getBlockDimension().width;
        }
    }

    /**
     * Paints the selection rectangle.
     * @param g Graphics
     */
    @Override
    protected void paintComponent(Graphics g) {
        super.paintComponent(g);
        if (selectionRect == null) {
            return;
        }

        Graphics2D g2d = (Graphics2D) g;
        final float alpha = 0.1f;
        AlphaComposite ta = AlphaComposite.getInstance(AlphaComposite.SRC_OVER, alpha);
        g2d.setComposite(ta);
        g2d.setColor(Color.BLUE);
        g2d.fill(selectionRect);
        g2d.setComposite(AlphaComposite.SrcOver);
        g2d.setColor(Color.BLACK);
        g2d.draw(selectionRect);
    }
}

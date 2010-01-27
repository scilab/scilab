/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Clément DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.palette.view;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.LayoutManager;
import java.awt.Rectangle;

import javax.swing.JPanel;
import javax.swing.Scrollable;

/**
 * Implement a view for the configurator.
 */
public class PaletteConfiguratorListView extends JPanel implements Scrollable {
	private static final LayoutManager LAYOUT =
		new ModifiedFlowLayout(FlowLayout.LEADING, 5, 5);
	private static final int MIN_DIMENSION = 125;
	private static final Dimension MINIMUM = new Dimension(MIN_DIMENSION, MIN_DIMENSION);
	
	/**
	 * Default constructor
	 */
	public PaletteConfiguratorListView() {
		setBackground(Color.WHITE);
		setLayout(LAYOUT);
		setMinimumSize(MINIMUM);
	}
	
	/**
	 * @return The prefered Scrollable dimension
	 * @see javax.swing.Scrollable#getPreferredScrollableViewportSize()
	 */
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
	public int getScrollableBlockIncrement(Rectangle visibleRect,
			int orientation, int direction) {
		return MIN_DIMENSION;
	}

	/**
	 * @return always false
	 * @see javax.swing.Scrollable#getScrollableTracksViewportHeight()
	 */
	public boolean getScrollableTracksViewportHeight() {
		return false;
	}

	/**
	 * @return always true
	 * @see javax.swing.Scrollable#getScrollableTracksViewportWidth()
	 */
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
	public int getScrollableUnitIncrement(Rectangle visibleRect,
			int orientation, int direction) {
		return MIN_DIMENSION;
	}
}

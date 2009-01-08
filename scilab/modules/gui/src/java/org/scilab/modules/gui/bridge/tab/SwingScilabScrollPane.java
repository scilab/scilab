/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - 2008 Digiteo Jean-Baptiste Silvy
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.bridge.tab;

import java.awt.Color;
import java.awt.Container;
import java.awt.Point;
import java.awt.Rectangle;

import javax.swing.JScrollPane;
import javax.swing.ScrollPaneConstants;

/**
 * Scroll pane based on Swing technology
 * @author Jean-Baptiste silvy
 */
public class SwingScilabScrollPane extends JScrollPane implements
		ScilabScrollPane {

	/** Needed */
	private static final long serialVersionUID = -4262585651413643814L;
	
	private SwingScilabAxes axes;

	/**
	 * Create a new Scroll pane around an axes.
	 * @param axes axes to scroll
	 */
	public SwingScilabScrollPane(SwingScilabAxes axes) {
		super(axes);
		this.axes = axes;
		// use the axes background as default one
		setRealBackground(axes.getBackground());
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
				     (int) viewport.getHeight()};
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
	 * Enable or disable auto resize mode
	 * @param autoResizeMode true if autoresize is on
	 */
	public void setAutoResizeMode(boolean autoResizeMode) {
		axes.setAutoResizeMode(autoResizeMode);
		if (autoResizeMode) {
			// hide scroll bars
			setHorizontalScrollBarPolicy(ScrollPaneConstants.HORIZONTAL_SCROLLBAR_NEVER);
			setVerticalScrollBarPolicy(ScrollPaneConstants.VERTICAL_SCROLLBAR_NEVER);
			// axes size may change
			axes.setSize(getSize());
		} else {
			// restore them
			setHorizontalScrollBarPolicy(ScrollPaneConstants.HORIZONTAL_SCROLLBAR_AS_NEEDED);
			setVerticalScrollBarPolicy(ScrollPaneConstants.VERTICAL_SCROLLBAR_AS_NEEDED);
		}
		
	}
	
	/**
	 * Enable or disable auto resize mode
	 * @return true if autoresize is on
	 */
	public boolean getAutoResizeMode() {
		return axes.getAutoResizeMode();
	}
	
	/**
	 * @return Container representation of this object
	 */
	public Container getAsContainer() {
		return this;
	}

}

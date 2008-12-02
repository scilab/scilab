/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 20072008 - INRIA - Vincent Couvert
 * Copyright (C) 2007 - INRIA - Bruno JOFRET
 * Copyright (C) 2007 - INRIA - Marouane BEN JELLOUL
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
import java.awt.Dimension;
import java.awt.Point;
import java.awt.ScrollPane;
import java.awt.event.ComponentEvent;
import java.awt.event.ComponentListener;

/**
 * Scroll pane based on AWT technology
 * @author Jean-Baptiste silvy
 */
public class AwtScilabScrollPane extends ScrollPane implements ScilabScrollPane, ComponentListener {

	/** needed */
	private static final long serialVersionUID = -3303313501821629057L;

	private SwingScilabAxes axes;
	
	/**
	 * Create a new Scroll pane around an axes.
	 * @param axes axes to scroll
	 */
	public AwtScilabScrollPane(SwingScilabAxes axes) {
		super(ScrollPane.SCROLLBARS_AS_NEEDED);
		add(axes);
		this.axes = axes;
		addComponentListener(this);
	}
	
	/**
	 * @return [x,y,w,h] Array of size 4 containing the position
	 *         width and height of the viewPort
	 */
	public int[] getViewingRegion() {
		int[] res = {getScrollPosition().x,
				 	 getScrollPosition().y,
				 	 getViewportSize().width,
				 	 getViewportSize().height};
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
		setBackground(newColor);
	}

	/**
	 * Move the viewport
	 * @param xPos the x position to scroll to
	 * @param yPos the y position to scroll to
	 */
	public void setViewPosition(int xPos, int yPos) {
		setScrollPosition(new Point(xPos, yPos));
	}
	
	/**
	 * Call when the scrollPane become hidden
	 * @param event generated event
	 */
	public void componentHidden(ComponentEvent event) { }


	/**
	 * Call when the scrollPane is moved
	 * @param event generated event
	 */
	public void componentMoved(ComponentEvent event) { }


	/**
	 * Call when the scrollPane is resized
	 * @param event generated event
	 */
	public void componentResized(ComponentEvent event) {
		if (axes.getAutoResizeMode()) {
			// in auto resize mode the viewport axes
			// fill the tab and then the view
			Dimension axesSize = this.getViewportSize();
			axes.setSize(axesSize);
		}
		
	}

	/**
	 * Call when the scrollPane appears on the screen
	 * @param event generated event
	 */
	public void componentShown(ComponentEvent event) { }

}

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
import java.awt.Graphics;
import java.awt.Point;
import java.awt.Rectangle;

import javax.swing.JScrollPane;

/**
 * Scroll pane based on Swing technology
 * @author Jean-Baptiste silvy
 */
public class SwingScilabScrollPane extends JScrollPane implements
		ScilabScrollPane {

	/** Needed */
	private static final long serialVersionUID = -4262585651413643814L;

	/**
	 * Create a new Scroll pane around an axes.
	 * @param axes axes to scroll
	 */
	public SwingScilabScrollPane(SwingScilabAxes axes) {
		super(axes);
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
		getViewport().setBackground(newColor);
	}

	/**
	 * Move the viewport
	 * @param xPos the x position to scroll to
	 * @param yPos the y position to scroll to
	 */
	public void setViewPosition(int xPos, int yPos) {
		getViewport().setViewPosition(new Point(xPos, yPos));
	}

}

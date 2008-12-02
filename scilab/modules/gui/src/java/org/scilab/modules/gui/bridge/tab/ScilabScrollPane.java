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

/**
 * Define a scrollPane that can be used wih Scilab
 * @author Jean-Baptiste Silvy
 */
public interface ScilabScrollPane {
	
	/**
	 * @return [x,y,w,h] Array of size 4 containing the position
	 *         width and height of the viewPort
	 */
	int[] getViewingRegion();
	
	/**
	 * Set the background of the scrollPane
	 * @param red red channel of the color
	 * @param green green channel
	 * @param blue blue channel
	 */
	void setBackground(double red, double green, double blue);
	
	/**
	 * Move the viewport
	 * @param xPos the x position to scroll to
	 * @param yPos the y position to scroll to
	 */
	void setViewPosition(int xPos, int yPos);
	
}

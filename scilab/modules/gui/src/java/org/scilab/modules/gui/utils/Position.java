/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent Couvert
 * Copyright (C) 2007 - INRIA - Bruno JOFRET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.utils;

/**
 * Class used to store position (X-coordinate and Y-coordinate) of objects in Scilab GUIs
 * @author Vincent COUVERT
 * @author Bruno JOFRET
 */
public class Position {

	private int x;
	private int y;

	/**
	 * Constructor
	 * @param x X-coordinate
	 * @param y Y-coordinate
	 */
	public Position(int x, int y) {
		this.x = x;
		this.y = y;
	}

	/**
	 * Gets the X-coordinate of objects in Scilab GUIs
	 * @return the X-coordinate of the object
	 */
	public int getX() {
		return x;
	}

	/**
	 * Sets the X-coordinate of objects in Scilab GUIs
	 * @param x the X-coordinate to set
	 */
	public void setX(int x) {
		this.x = x;
	}

	/**
	 * Gets the Y-coordinate of objects in Scilab GUIs
	 * @return the Y-coordinate of the object
	 */
	public int getY() {
		return y;
	}

	/**
	 * Sets the Y-coordinate of objects in Scilab GUIs
	 * @param y the Y-coordinate to set
	 */
	public void setY(int y) {
		this.y = y;
	}


}

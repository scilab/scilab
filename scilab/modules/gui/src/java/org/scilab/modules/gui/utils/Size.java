
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.utils;

/**
 * Class used to store size (width and height) of objects in Scilab GUIs
 * @author Vincent COUVERT
 * @author Bruno JOFRET
 */
public class Size {

	private int height;
	private int width;

	/**
	 * Constructor
	 * @param width the width of the object
	 * @param height the height of the object
	 */
	public Size(int width, int height) {
		this.height = height;
		this.width = width;
	}

	/**
	 * Gets the height of a Scilab GUI object
	 * @return the height
	 */
	public int getHeight() {
		return height;
	}
	/**
	 * Sets the height of a Scilab GUI object
	 * @param height the height to set
	 */
	public void setHeight(int height) {
		this.height = height;
	}
	/**
	 * Gets the width of a Scilab GUI object
	 * @return the width
	 */
	public int getWidth() {
		return width;
	}
	/**
	 * Sets the width of a Scilab GUI object
	 * @param width the width to set
	 */
	public void setWidth(int width) {
		this.width = width;
	}
}

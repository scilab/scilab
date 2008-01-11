
/* Copyright INRIA 2008 */

package org.scilab.modules.gui.utils;

import javax.swing.SwingConstants;

/**
 * Alignment property values for Scilab Uicontrols
 * @author Vincent COUVERT
 */
public final class ScilabAlignment {

	/**
	 * Left horizontal alignment
	 */
	public static final String LEFT = "left";
	
	/**
	 * Center horizontal alignment
	 */
	public static final String CENTER = "center";
	
	/**
	 * Right horizontal alignment
	 */
	public static final String RIGHT = "right";
	
	/**
	 * Top vertical alignment
	 */
	public static final String TOP = "top";

	/**
	 * Middle vertical alignment
	 */
	public static final String MIDDLE = "middle";

	/**
	 * Bottom vertical alignment
	 */
	public static final String BOTTOM = "bottom";
	
	/**
	 * Constructor
	 */
	private ScilabAlignment() {
		/* Should not be used */
		throw new UnsupportedOperationException();
	}
	
	/**
	 * Convert the Scilab string value for alignment to Swing value 
	 * @param alignment the Scilab value for alignment
	 * @return the Swing value
	 */
	public static int toSwingAlignment(String alignment) {
		int returnValue = SwingConstants.CENTER;
		
		if (alignment.equals(LEFT)) {
			returnValue = SwingConstants.LEFT;
		} else if (alignment.equals(RIGHT)) {
			returnValue = SwingConstants.RIGHT;
		} else if (alignment.equals(TOP)) {
			returnValue = SwingConstants.TOP;
		} else if (alignment.equals(BOTTOM)) {
			returnValue = SwingConstants.BOTTOM;
		}
		
		return returnValue;
	}
}

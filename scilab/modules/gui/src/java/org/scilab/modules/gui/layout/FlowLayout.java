
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.layout;

/**
 * Interface for FlowLayout associated to objects in Scilab GUIs
 * @author Marouane BEN JELLOUL
 */
public interface FlowLayout extends LayoutManager {
	//	 TODO : Add the methods that are usefull for a FlowLayout
	
	/**
	 * This value indicates that each row of components should be centered.
	 */
	int CENTER = java.awt.FlowLayout.CENTER;
	/**
	 * This value indicates that each row of components should be justified to the leading edge of the container's orientation,
	 *  for example, to the left in left-to-right orientations.
	 */
	int LEADING = java.awt.FlowLayout.LEADING;
	/**
	 * This value indicates that each row of components should be left-justified.
	 */
	int LEFT = java.awt.FlowLayout.LEFT;
	/**
	 * This value indicates that each row of components should be right-justified.
	 */
	int RIGHT = java.awt.FlowLayout.RIGHT;
	/**
	 * This value indicates that each row of components should be justified to the trailing edge of the container's orientation,
	 *  for example, to the right in left-to-right orientations.
	 */
	int TRAILING = java.awt.FlowLayout.TRAILING;
}

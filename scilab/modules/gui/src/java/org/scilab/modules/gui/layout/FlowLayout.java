/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Marouane BEN JELLOUL
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

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

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
 * Interface for BorderLayout associated to objects in Scilab GUIs
 * @author Marouane BEN JELLOUL
 */
public interface BorderLayout extends LayoutManager {
	//	 TODO : Add the methods that are usefull for a BorderLayout
	
	/**
	 * The center layout constraint (middle of container).
	 */
	String CENTER = java.awt.BorderLayout.CENTER;
	 
	/**
	 * The east layout constraint (right side of container).
	 */
	String EAST = java.awt.BorderLayout.EAST;
	
	/**
	 * The north layout constraint (top of container).
	 */
	String NORTH = java.awt.BorderLayout.NORTH;
	
	/**
	 * The south layout constraint (bottom of container).
	 */
	String SOUTH = java.awt.BorderLayout.SOUTH;
	
	/**
	 * The west layout constraint (left side of container).
	 */
	String WEST = java.awt.BorderLayout.WEST;
}

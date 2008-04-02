/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class containing several geometric algorithms 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


package org.scilab.modules.renderer.utils.geom3D;

/**
 * Class containing several geometric algorithms
 * @author Jean-Baptiste Silvy
 */
public final class GeomAlgos {

	/** Number of corners for a rectangle */
	public static final int RECTANGLE_NB_CORNERS = 4;
	
	/**
	 * Default constructor
	 */
	protected GeomAlgos() {
		
	}
	
	/**
	 * Find if the two rectangles given by their corners are concealing or not in 2D
	 * @param rectangle1 four corners of the first rectangle
	 * @param rectangle2 four corners of the second rectangle
	 * @return true if the rectangle are concealing, false otherwise
	 */
	public static boolean areRectangleConcealing(Vector3D[] rectangle1, Vector3D[] rectangle2) {
		// get min and max in 2D for each rectangle
		double xMin1 = Math.min(rectangle1[0].getX(), rectangle1[2].getX());
		double yMin1 = Math.min(rectangle1[0].getY(), rectangle1[2].getY());
		double xMax1 = Math.max(rectangle1[0].getX(), rectangle1[2].getX());
		double yMax1 = Math.max(rectangle1[0].getY(), rectangle1[2].getY());
		double xMin2 = Math.min(rectangle2[0].getX(), rectangle2[2].getX());
		double yMin2 = Math.min(rectangle2[0].getY(), rectangle2[2].getY());
		double xMax2 = Math.max(rectangle2[0].getX(), rectangle2[2].getX());
		double yMax2 = Math.max(rectangle2[0].getY(), rectangle2[2].getY());
		
		// rectangle are concealing if 2 bounds are within the others
		if (((xMin1 >= xMin2 && xMin1 <= xMax2) || (xMax1 >= xMin2 && xMax1 <= xMax2))
			&& ((yMin1 >= yMin2 && yMin1 <= yMax2)  || (yMax1 >= yMin2 && yMax1 <= yMax2))) {
			return true;
		}
					
		return false;
	}
	
}

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
	public static boolean areRectangleConcealing(double[] rectangle1, double[] rectangle2) {
		// get min and max in 2D for each rectangle
		double xMin1 = rectangle1[0];
		double yMin1 = rectangle1[2];
		double xMax1 = rectangle1[1];
		double yMax1 = rectangle1[RECTANGLE_NB_CORNERS - 1];
		double xMin2 = rectangle2[0];
		double yMin2 = rectangle2[2];
		double xMax2 = rectangle2[1];
		double yMax2 = rectangle2[RECTANGLE_NB_CORNERS - 1];
		
		return (((xMin1 >= xMin2 && xMin1 <= xMax2) || (xMax1 >= xMin2 && xMax1 <= xMax2))
				&& ((yMin1 >= yMin2 && yMin1 <= yMax2)  || (yMax1 >= yMin2 && yMax1 <= yMax2)));
					
	}
	
}

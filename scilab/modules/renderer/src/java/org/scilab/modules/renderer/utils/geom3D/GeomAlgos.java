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
	 * Find if a two point are on the same side of a line
	 * @param pointOnline1 first point to define the line
	 * @param pointOnline2 second point to define the line
	 * @param point1 first point to test 
	 * @param point2 second point to test
	 * @return true if the two vector are on the same side of the line
	 */
	private static boolean sameSide(Vector3D pointOnline1, Vector3D pointOnline2,
							 Vector3D point1, Vector3D point2) {
		Vector3D lineVector = pointOnline2.substract(pointOnline1);
		Vector3D side1 = lineVector.crossProduct(point1.substract(pointOnline1));
		Vector3D side2 = lineVector.crossProduct(point2.substract(pointOnline1));
		
		return side1.dotProduct(side2) >= 0.0;
	}
	
	/**
	 * Find a point is inside a rectangle in 2D
	 * @param rectangle four corners of the rectangle
	 * @param point point to chack
	 * @return true if the poinr is inside, false otherwise
	 */
	private static boolean isPointInsideRectangle(Vector3D[] rectangle, Vector3D point) {
		for (int i = 0; i < RECTANGLE_NB_CORNERS; i++) {
			if (!sameSide(rectangle[i % RECTANGLE_NB_CORNERS],
					      rectangle[(i + 1) % RECTANGLE_NB_CORNERS],
					      rectangle[(i + 2) % RECTANGLE_NB_CORNERS],
					      point)) {
				return false;
			}
		}
		return true;
	}
	
	/**
	 * Find if the two rectangles given by their corners are concealing or not in 2D
	 * @param rectangle1 four corners of the first rectangle
	 * @param rectangle2 four corners of the second rectangle
	 * @return true if the rectangle are concealing, false otherwise
	 */
	public static boolean areRectangleConcealing(Vector3D[] rectangle1, Vector3D[] rectangle2) {
		for (int i = 0; i < RECTANGLE_NB_CORNERS; i++) {
			// we need to cjeck twice since a rectangle might be smaller than the other
			// and fit inside
			if (isPointInsideRectangle(rectangle1, rectangle2[i])) {
				return true;
			}
			if (isPointInsideRectangle(rectangle2, rectangle1[i])) {
				return true;
			}
		}
		return false;
	}
	
}

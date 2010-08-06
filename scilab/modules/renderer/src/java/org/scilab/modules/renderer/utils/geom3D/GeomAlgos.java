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
	private static final int LAST_INDEX = 3;
	
	/** OpenGl maximum representable number (2^32) */
	private static final float BIGGEST = 4294967296f;
	
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
//		double xMin1 = rectangle1[0];
//		double yMin1 = rectangle1[2];
//		double xMax1 = rectangle1[1];
//		double yMax1 = rectangle1[RECTANGLE_NB_CORNERS - 1];
//		double xMin2 = rectangle2[0];
//		double yMin2 = rectangle2[2];
//		double xMax2 = rectangle2[1];
//		double yMax2 = rectangle2[RECTANGLE_NB_CORNERS - 1];
//		
//		return (((xMin1 >= xMin2 && xMin1 <= xMax2) || (xMax1 >= xMin2 && xMax1 <= xMax2))
//				&& ((yMin1 >= yMin2 && yMin1 <= yMax2)  || (yMax1 >= yMin2 && yMax1 <= yMax2)));
		
		return (((rectangle1[0] >= rectangle2[0] && rectangle1[0] <= rectangle2[1])
		      || (rectangle1[1] >= rectangle2[0] && rectangle1[1] <= rectangle2[1]))
		    &&  ((rectangle1[2] >= rectangle2[2] && rectangle1[2] <= rectangle2[RECTANGLE_NB_CORNERS - 1])
		      || (rectangle1[LAST_INDEX] >= rectangle2[2] && rectangle1[LAST_INDEX] <= rectangle2[LAST_INDEX])));
					
	}
	
	/**
	 * Aproximate a surface defined by 4 points with 2 triangles.
	 * This function is useful to draw such a surface. Using quads may lead to
	 * badly oriented facets is the 4 points are not coplanar.
	 * @param quad 4 vertices to approximate oredred clockwise or counter-clockwise
	 * @param firstTriangleIndices indices within the quad defining the first triangle
	 * @param secondTriangleIndices indices within the quad defining the first triangle
	 */
	private static void decomposeQuad(Vector3D[] quad, int[] firstTriangleIndices, int[] secondTriangleIndices) {
//		 The quad is defined with 4 oreinted Vertices with indiced 0, 1, 2 and 3
		// Their are only 2 choices for the triangles to preserve orientation:
		// 0, 1, 2 and 2, 3, 0 or 0, 1, 3 and 1, 2, 3
		
		// The selected configuration is the one which as the closest angle with Pi between the two triangles
		// For configuration one, the angle is approximated with (1, M, 3) where M is the middle of [0 2]
		Vector3D m = quad[0].add(quad[2]).scalarMult(1.0 / 2.0);
		// get cosine of the angle
		Vector3D m1 = quad[1].substract(m).getNormalized();
		Vector3D m3 = quad[LAST_INDEX].substract(m).getNormalized();
		double cosAngle1 = m1.dotProduct(m3);
		
		// For configuration two, the angle is approximated with (0, M', 2) where M' is the middle of [1 3]
		m = quad[1].add(quad[LAST_INDEX]).scalarMult(1.0 / 2.0);
		// get cosine of the angle
		Vector3D m0 = quad[0].substract(m).getNormalized();
		Vector3D m2 = quad[2].substract(m).getNormalized();
		double cosAngle2 = m0.dotProduct(m2);
		
		// angle close to Pi <=> cosAngle close to -1
		if ((cosAngle1 + 1) <= (cosAngle2 + 1)) {
			// first configuration
			// default configuration for Scilab 4
			firstTriangleIndices[0] = 0;
			firstTriangleIndices[1] = 1;
			firstTriangleIndices[2] = 2;
			
			secondTriangleIndices[0] = 0;
			secondTriangleIndices[1] = 2;
			secondTriangleIndices[2] = LAST_INDEX;
		} else {
			// second configuration
			firstTriangleIndices[0] = 0;
			firstTriangleIndices[1] = 1;
			firstTriangleIndices[2] = LAST_INDEX;
			
			secondTriangleIndices[0] = 1;
			secondTriangleIndices[1] = 2;
			secondTriangleIndices[2] = LAST_INDEX;
		}
	}
	
	/**
	 * Aproximate a surface defined by 4 points with 2 triangles.
	 * This function is useful to draw such a surface. Using quads may lead to
	 * badly oriented facets is the 4 points are not coplanar.
	 * Also compute colors indices corresponding to each triangle vertices
	 * @param quad 4 vertices to approximate oredred clockwise or counter-clockwise
	 * @param colorQuad color indices corresponding to each vertex of the quad
	 * @param triangle1 first triangle of the decomposition 
	 * @param colorTriangle1 color indices coresponding to each vertex of the first triangle
	 * @param triangle2 second triangle
	 * @param colorTriangle2 color indices coresponding to each vertex of the first triangle
	 */
	public static void decomposeQuad(Vector3D[] quad, int[] colorQuad,
									 Vector3D[] triangle1, int[] colorTriangle1,
									 Vector3D[] triangle2, int[] colorTriangle2) {
		int[] firstTriangleIndices = new int[LAST_INDEX];
		int[] secondTriangleIndices = new int[LAST_INDEX];
		
		decomposeQuad(quad, firstTriangleIndices, secondTriangleIndices);
		
		for (int i = 0; i < LAST_INDEX; i++) {
			triangle1[i] = quad[firstTriangleIndices[i]];
			colorTriangle1[i] = colorQuad[firstTriangleIndices[i]];
			triangle2[i] = quad[secondTriangleIndices[i]];
			colorTriangle2[i] = colorQuad[secondTriangleIndices[i]];
		}
	}
							
										
	
	/**
	 * Aproximate a surface defined by 4 points with 2 triangles.
	 * This function is useful to draw such a surface. Using quads may lead to
	 * badly oriented facets is the 4 points are not coplanar
	 * @param quad 4 vertices to approximate oredred clockwise or counter-clockwise
	 * @param triangle1 first triangle of the decomposition
	 * @param triangle2 second triangle
	 */
	public static void decomposeQuad(Vector3D[] quad, Vector3D[] triangle1, Vector3D[] triangle2) {
		int[] firstTriangleIndices = new int[LAST_INDEX];
		int[] secondTriangleIndices = new int[LAST_INDEX];
		
		decomposeQuad(quad, firstTriangleIndices, secondTriangleIndices);
		
		for (int i = 0; i < LAST_INDEX; i++) {
			triangle1[i] = quad[firstTriangleIndices[i]];
			triangle2[i] = quad[secondTriangleIndices[i]];
		}
	}
	
	/**
	 * Check if the 3 component of a 3D vector is finite.
	 * @param xCoord X coordinate of the vector
	 * @param yCoord Y coordinate of the vector
	 * @param zCoord Z coordinate of the vector
	 * @return true if the vector is finite, false otherwise
	 */
	public static boolean isVector3DFinite(double xCoord, double yCoord, double zCoord) {
		return    !(Double.isInfinite(xCoord) || Double.isNaN(xCoord)
				 || Double.isInfinite(yCoord) || Double.isNaN(yCoord)
				 || Double.isInfinite(zCoord) || Double.isNaN(zCoord));
	}
	
	/**
	 * Check if the 3 component of a 3D vector are not NaN.
	 * @param xCoord X coordinate of the vector
	 * @param yCoord Y coordinate of the vector
	 * @param zCoord Z coordinate of the vector
	 * @return true if the vector doesn't content any NaN value, false otherwise
	 */
	public static boolean isVector3DRepresentable(double xCoord, double yCoord, double zCoord) {
		return !(Double.isNaN(xCoord) || Double.isNaN(yCoord) || Double.isNaN(zCoord));
	}
	
	/**
	 * Check if the 3 component of a 3D vector are not NaN.
	 * @param v of the vector
	 * @return true if the vector doesn't content any NaN value, false otherwise
	 */
	public static boolean isVector3DRepresentable(Vector3D v) {
		return isVector3DRepresentable (v.getX(), v.getY(), v.getZ());
	}
	
	/**
	 * Transform a double value to a representable OpenGl value
	 * Because,
	 * The maximum representable magnitude of a floating-point number
	 * used to represent positional, normal, or texture coordinates must
	 * be at least 2^32 
	 * in :
	 * http://www.opengl.org/registry/doc/glspec31.20090528.pdf
	 * Chapter 2.1.1 Floating-Point Computation 
	 * @param value the value to represents  
	 * @return the representable OpenGl value
	 */
	public static float glRepresentable(double value) {
		float f = (float) value;
		if (Float.isInfinite(f)) {
			if (f > 0) {
				f = BIGGEST;
			} else {
				f = -BIGGEST;
			}
		}
		return f;
	}
}

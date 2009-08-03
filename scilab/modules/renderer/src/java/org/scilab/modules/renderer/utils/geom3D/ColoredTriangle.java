/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Sylvestre Koumar
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
 * This class decompose triangle in polygons then paint them
 *
 */
public class ColoredTriangle {
	
	/** Triangle side enumeration */	
	public enum Side {
		SIDE_BC, SIDE_AC, SIDE_AB;
	}
	
	/** Number of side of the polygon */
	private static final int SIDE_1 = 0;
	private static final int SIDE_2 = 1;
	private static final int SIDE_3 = 2;
	private static final int SIDE_4 = 3;
	private static final int SIDE_5 = 4;
	
	/** Numbers of sides of the polygon */
	private static final int POLY_SIDE3 = 3;
	private static final int POLY_SIDE4 = 4;
	private static final int POLY_SIDE5 = 5;
	
	/** gap for color */
	private static final double GAP = 0.5;
	
	/** Summit of triangle */
	private Vector3D a;
	private Vector3D b;
	private Vector3D c;
	
	/** Color of the summits of the triangle */
	private double colorA;
	private double colorB;
	private double colorC;
	
	/** Smallest & biggest color */
	private double min;
	
	/** previous and next polygon point */
	private Vector3D prevI1 = new Vector3D();
	private Vector3D prevI2 = new Vector3D();
	private Vector3D nextI1 = new Vector3D();
	private Vector3D nextI2 = new Vector3D();
	
	/** previous and next polygon side */
	private Side prevSideI1;
	private Side prevSideI2;
	private Side nextSideI1;
	private Side nextSideI2;
	
	
	private Vector3D orientation;
	
	
	/** 
	 * Constructor
	 * First we orderer coordinates and colors (smallest to the biggest)
	 * Color of coordinate B should nott be the same of the coordinate C
	 * @param v1 one of the summit
	 * @param v2 one of the summit
	 * @param v3 one of the summit
	 * @param colorV1 color of the summit
	 * @param colorV2 color of the summit
	 * @param colorV3 color of the summit
	 */
	public ColoredTriangle(Vector3D v1, Vector3D v2, Vector3D v3,
							double colorV1, double colorV2, double colorV3) {		
		
		/** Swap v1, v2 & v3 in the right order */				
		if (colorV1 <= colorV2 && colorV1 <= colorV3) {
			colorC = colorV1;
			c = new Vector3D(v1);		
			if (colorV2 <= colorV3) {
				colorB = colorV2;	
				colorA = colorV3;
				b = new Vector3D(v2);
				a = new Vector3D(v3);
				
			} else {
				colorB = colorV3;
				colorA = colorV2;
				b = new Vector3D(v3);
				a = new Vector3D(v2);
			}
		} else if (colorV2 <= colorV1 && colorV2 <= colorV3) {
			colorC = colorV2;
			c = new Vector3D(v2);		
			if (colorV1 <= colorV3) {
				colorB = colorV1;	
				colorA = colorV3;
				b = new Vector3D(v1);
				a = new Vector3D(v3);
			} else {
				colorB = colorV3;
				colorA = colorV1;
				b = new Vector3D(v3);
				a = new Vector3D(v1);
			}			
		} else if (colorV3 <= colorV1 && colorV3 <= colorV2) {
			colorC = colorV3;
			c = new Vector3D(v3);		
			if (colorV1 <= colorV2) {
				colorB = colorV1;	
				colorA = colorV2;
				b = new Vector3D(v1);
				a = new Vector3D(v2);
			} else {
				colorB = colorV2;
				colorA = colorV1;
				b = new Vector3D(v2);
				a = new Vector3D(v1);
			}			
		}		
		
		Vector3D CB = b.substract(c);
		Vector3D CA = a.substract(c);
		Vector3D V1V2 = v2.substract(v1);
		Vector3D V1V3 = v3.substract(v1);
		
		orientation = V1V2.crossProduct(V1V3);
		
		if ((CB.crossProduct(CA).dotProduct(V1V2.crossProduct(V1V3))) < 0) {
			double tempCol = colorB;
			colorB = colorA;
			colorA = tempCol;
			
			Vector3D tempVec = b;
			b = a;
			a = tempVec;
		}
	}
	
	/** 
	 * Decompose triangle
	 * @return res TriangleDecomposition
	 */
	public TriangleDecomposition decomposeTriangle() {		
		
		TriangleDecomposition res = new TriangleDecomposition();
		
		min = Math.min(Math.min(colorC, colorB), colorA);
		
		//if all colors are the same
		if (colorA == colorB && colorA == colorC) {
			int nbPolygons = 1;
			res.setNbPolygons(nbPolygons);	
			double curColor = colorC;		
			res.setPolygonColor(curColor, 0);
			Vector3D[] curPolygon = {c, b, a};
			res.setPolygon(curPolygon, 0);

		} else {
			
			int nbPolygons = getNbPolygons(colorA, colorB, colorC);
			double curColor = min;
			res.setNbPolygons(nbPolygons);
			
			//starting the polygon decomposition with the smallest color
			prevI1 = c;
			prevI2 = c;
			
			prevSideI1 = Side.SIDE_BC;
			prevSideI2 = Side.SIDE_AC;
			

			for (int i = 0; i < nbPolygons - 1; i++) {			
				// color of the next polygon
				

				res.setPolygonColor(curColor, i);			
				Vector3D[] polygonDecomposed = getPolygon(prevI1, prevI2, prevSideI1, prevSideI2, curColor);
				res.setPolygon(polygonDecomposed, i);
				
				//coordinates for the next polygon which will decomposed
				prevI1 = nextI1;
				prevI2 = nextI2;
				prevSideI1 = nextSideI1;
				prevSideI2 = nextSideI2;
				
				
				curColor += 1;
			}	

			//ending the polygon decomposition with the biggest color
			Vector3D[] lastPoly;
			Vector3D intersect = findIntersection(prevSideI1, prevSideI2);
			if (intersect.equals(c)) {
				lastPoly = new Vector3D[POLY_SIDE4];
				lastPoly[0] = prevI2;
				lastPoly[1] = prevI1;
				lastPoly[2] = b;
				lastPoly[3] = a;
			} else {
				lastPoly = new Vector3D[POLY_SIDE3];
				lastPoly[0] = prevI1;
				lastPoly[1] = prevI2;
				lastPoly[2] = intersect;
			}
			
			if (!checkTriangleOrientation(lastPoly[0], lastPoly[1], lastPoly[2], orientation)) {
				Vector3D tempOrientation = lastPoly[1];
				lastPoly[1] = lastPoly[2];
				lastPoly[2] = tempOrientation;				
			}
			
			res.setPolygonColor(curColor, nbPolygons - 1);
			res.setPolygon(lastPoly, nbPolygons - 1);
		}
		return res;
	}
	
	/** 
	 * Get the number of polygons which will be created 
	 * @param colorA int
	 * @param colorB int
	 * @param colorC int
	 * @return number of polygons
	 */
	public int getNbPolygons(double colorA, double colorB, double colorC) {
		// find lowest and highest colors
		double colMin = Math.min(Math.min(colorA, colorB), colorC);
		double colMax = Math.max(Math.max(colorA, colorB), colorC);
		
		// the number of polygon is the number of x,5 (x integer)
		// which is between colMin and colMax plus 1.
		// find x,5 which is just greater than colMin
		colMin = getSuperiorHalf(colMin);
		
		// find x,5 which is just lower than colMax
		colMax = getInferiorHalf(colMax);
		
		int res = (int) (colMax - colMin) + 2;
		
		return res;
	}
	
	/**  
	 * Get border of the current polygon
	 * @param vertexA Vector3D
	 * @param vertexB Vector3D
	 * @param vertexC Vector3D
	 * @param colorA int
	 * @param colorB int
	 * @param colorC int
	 * @param curColor int
	 * @return border of the current polygon
	 */
	public Vector3D[] getBorder(Vector3D vertexA, Vector3D vertexB, Vector3D vertexC, 
			int colorA, int colorB, int colorC, int curColor) {		

		Vector3D CB = vertexB.substract(vertexC);
		Vector3D vectorCB = CB.scalarMult((colorC - curColor) / (colorC - colorB));
		Vector3D p1 = vertexC.add(vectorCB);				

		Vector3D CA = vertexA.substract(vertexC);
		vectorCB = CB.scalarMult((colorA - curColor) / (colorC - colorB));
		Vector3D p2 = vertexC.add(vectorCB).add(CA);

		Vector3D[] curPolygon = {p1, p2};
		return curPolygon;
	}
	
	/** 
	 * Convert barycentric coordinates to 3D coordinates 
	 * @param x double
	 * @param y double
	 * @return barycentric coordinates converted into 3D coordinates 
	 */
	public Vector3D barycentricCoordTo3D(double x, double y) {
		
		Vector3D vectorCB = b.substract(c);
		Vector3D vectorCA = a.substract(c);
		
		Vector3D barycentrictTo3D = c.add(vectorCB.scalarMult(x).add(vectorCA.scalarMult(y)));		
		return barycentrictTo3D;
	}
	
	
	/**
	 * Get intersections which separates polygons in the triangle
	 * @param curColor current color
	 * @param intersections coordinates of intersections
	 * @param intersectionSides sides of intersections
	 */
	public void intersectionsManager(double curColor, Vector3D[] intersections, Side[] intersectionSides) {
		
		int nbIntersections = 0;		

		/* Test for (P1, P2) & (BC) */
		if (colorB != colorC && colorB != curColor) {
			double res1 = (colorC - curColor) / (colorC - colorB);
			if (res1 >= 0 && res1 <= 1) {
				Vector3D i1 = new Vector3D(res1, 0.0, 0.0);
				intersections[nbIntersections] = i1;
				intersectionSides[nbIntersections] = Side.SIDE_BC;
				nbIntersections++;				
			}
		}		
		
		/* Test for (P1, P2) & (AC) */
		if (colorA != colorC && colorA != curColor) {
			double res2 = (colorC - curColor) / (colorC - colorA);

			if (res2 >= 0 && res2 <= 1) {
				Vector3D i2 = new Vector3D(0.0, res2, 0.0);
				intersections[nbIntersections] = i2;
				intersectionSides[nbIntersections] = Side.SIDE_AC;
				nbIntersections++;
			}
		}
		
		/* Test for (P1, P2) & (AB) */
		// If colorA or colorB equals curColor, then the intersection
		// is on B or A. So the two previous tests have already been true.
		if (colorA != colorB) {
			double res3 = (colorA - curColor) / (colorA - colorB);
			double res4 = (colorB - curColor) / (colorB - colorA);

			if (res3 >= 0 && res4 >= 0) {
				Vector3D i3 = new Vector3D(res3, res4, 0.0);
				intersections[nbIntersections] = i3;
				intersectionSides[nbIntersections] = Side.SIDE_AB;
				nbIntersections++;
			}
		}
		
		
		
//		if (nbIntersections == 3) {//			
//			if (intersections[0].equals(intersections[1])) {
//				intersections[1] = intersections[2];
//				intersectionSides[1] = intersectionSides[2];
//			}			
//		}
		
		//Convert barycentric intersections coordinates to 3D coordinates 
		for (int i = 0; i < nbIntersections; i++) {
			intersections[i] = barycentricCoordTo3D(intersections[i].getX(), intersections[i].getY());
		}
	}	
	
	/**
	 * Get the polygon that we have decomposed
	 * @param prevI1 previous intersection1
	 * @param prevI2 previous intersection2
	 * @param prevSideI1 previous side1
	 * @param prevSideI2 previous side2
	 * @param curColor current color
	 * @return polygon decomposed
	 */
	public Vector3D[] getPolygon(Vector3D prevI1, Vector3D prevI2, Side prevSideI1, Side prevSideI2, double curColor) {
				
		Vector3D[] polygons = null;
		Vector3D p = new Vector3D();	
		
		Vector3D[] intersections = new Vector3D[3];
		Side[] intersectionsSides = new Side[3];		
		
		//finding nextI1 and nextI2	
		double color = getSuperiorHalf(curColor);
		intersectionsManager(color, intersections, intersectionsSides);		
		nextI1 = intersections[0];
		nextI2 = intersections[1];
		nextSideI1 = intersectionsSides[0];
		nextSideI2 = intersectionsSides[1];
		
		if (prevSideI1 == nextSideI2 || prevSideI2 == nextSideI1) {
			Vector3D temp = nextI1;
			nextI1 = nextI2;
			nextI2 = temp;
			Side tempSide = nextSideI1;
			nextSideI1 = nextSideI2;
			nextSideI2 = tempSide;
		}
		
		//if the decomposed polygon have 4 or 5 summits
		//coordinates are ordered in the reverse sense of the watch
		if (prevSideI1 == nextSideI1 && prevSideI2 == nextSideI2) {
			polygons = new Vector3D[POLY_SIDE4];			
			polygons[SIDE_1] = prevI1;
			polygons[SIDE_2] = prevI2;
			polygons[SIDE_3] = nextI2;
			polygons[SIDE_4] = nextI1;
			
		} else if (prevSideI1 != nextSideI1) {
			p = findIntersection(prevSideI1, nextSideI1);
			polygons = new Vector3D[POLY_SIDE5];
			polygons[SIDE_1] = prevI1;
			polygons[SIDE_2] = prevI2;			
			polygons[SIDE_3] = nextI2;
			polygons[SIDE_4] = nextI1;
			polygons[SIDE_5] = p;
			
		} else if (prevSideI2 != nextSideI2) {
			p = findIntersection(prevSideI2, nextSideI2);
			polygons = new Vector3D[POLY_SIDE5];
			polygons[SIDE_1] = prevI1;
			polygons[SIDE_2] = prevI2;	
			polygons[SIDE_3] = p;
			polygons[SIDE_4] = nextI2;
			polygons[SIDE_5] = nextI1;
		}
		
		//Test for the orientation of the triangle
		int polyLength = polygons.length;
		
		for (int i = 0; i < polyLength - 2; i++) {
			if (!checkTriangleOrientation(polygons[i], polygons[i + 1], polygons[i + 2], orientation)) {
				Vector3D tempOrientation = polygons[i + 1];
				polygons[i + 1] = polygons[i + 2];
				polygons[i + 2] = tempOrientation;				
			}
		}
		if (!checkTriangleOrientation(polygons[polyLength - 2], polygons[polyLength - 1], polygons[0], orientation)) {
			Vector3D tempOrientation = polygons[polyLength - 1];
			polygons[polyLength - 1] = polygons[0];
			polygons[0] = tempOrientation;				
		}
		
		if (!checkTriangleOrientation(polygons[polyLength - 1], polygons[0], polygons[1], orientation)) {
			Vector3D tempOrientation = polygons[0];
			polygons[0] = polygons[1];
			polygons[1] = tempOrientation;				
		}		
		
		for (int i = 1; i < polyLength - 1; i++) {
			if (!checkTriangleOrientation(polygons[0], polygons[i], polygons[i + 1], orientation)) {
				Vector3D tempOrientation = polygons[i];
				polygons[i] = polygons[i + 1];
				polygons[i + 1] = tempOrientation;				
			}
		}
		
		return polygons;
	}
	
	/**
	 * Find intersection of the new polygon 
	 * @param side1 of the triangle
	 * @param side2 of the triangle
	 * @return intersection vector3D
	 */
	public Vector3D findIntersection(Side side1, Side side2) {
		Vector3D p = null;
		
		//finding intersection among the sides
		if ((side1 == Side.SIDE_AC &&  side2 == Side.SIDE_AB)
		 || (side1 == Side.SIDE_AB &&  side2 == Side.SIDE_AC)) {
			p = new Vector3D(a);
			return p;
			
		} else if ((side1 == Side.SIDE_BC &&  side2 == Side.SIDE_AB)
				|| (side1 == Side.SIDE_AB &&  side2 == Side.SIDE_BC)) {
			p = new Vector3D(b);
			return p;			
			
		} else if ((side1 == Side.SIDE_AC &&  side2 == Side.SIDE_BC)
				|| (side1 == Side.SIDE_BC &&  side2 == Side.SIDE_AC)) {
			p = new Vector3D(c);
			return p;
			
		} else {
			return p;
		}	
		
	}	
	
	/**
	 * Check the triangle orientation (front or back)
	 * @param a Vector3D
	 * @param b Vector3D
	 * @param c Vector3D
	 * @param orientation Vector3Ds
	 * @return the orientation of the triangle
	 */
	public boolean checkTriangleOrientation(Vector3D a, Vector3D b, Vector3D c, Vector3D orientation) {
		Vector3D AB = b.substract(a);
		Vector3D AC = c.substract(a);		
		return (AB.crossProduct(AC)).dotProduct(orientation) >= 0;
	}
	
	/**
	 * Compute the lowest value under the way of x.5 where x is an integer which is greater
	 * than the input value.
	 * @param value input value
	 * @return result of the function.
	 */
	private double getSuperiorHalf(double value) {
		double res = Math.ceil(value - 0.5) + 0.5;
		
		// if value is under the way x.5, res will be equal to value
		if (res == value) {
			res += 1.0;
		}
		return res;
	}
	
	/**
	 * Compute the highest value under the way of x.5 where x is an integer which is lower
	 * than the input value.
	 * @param value input value
	 * @return result of the function.
	 */
	private double getInferiorHalf(double value) {
		double res = Math.floor(value + 0.5) - 0.5;
		
		// if value is under the way x.5, res will be equal to value
		if (res == value) {
			res -= 1.0;
		}
		return res;
	}
	
	

}

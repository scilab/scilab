package org.scilab.modules.renderer.utils.geom3D;

/**
 * This class decompose triangle in polygons then paint them
 * @author Sylvestre Koumar
 *
 */
public class ColoredTriangle {
	
	/** Triangle side enumeration */	
	public enum Side {
		SIDE_BC, SIDE_AC, SIDE_AB;
	}
	
	/** Summit of triangle */
	private Vector3D a;
	private Vector3D b;
	private Vector3D c;
	
	/** Color of the summits of the triangle */
	private int colorA;
	private int colorB;
	private int colorC;
	
	/** Smallest & biggest color */
	private int min;
	private int max;
	
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
							int colorV1, int colorV2, int colorV3) {		
		
		/** Swap v1, v2 & v3 in the right order */				
		if (colorV1 <= colorV2 && colorV1 <= colorV3) {
			colorC = colorV1;
			c = v1;		
			if (colorV2 <= colorV3) {
				colorB = colorV2;	
				colorA = colorV3;
				b = v2;
				a = v3;
				
			} else {
				colorB = colorV3;
				colorA = colorV2;
				b = v3;
				a = v2;
			}
		} else if (colorV2 <= colorV1 && colorV2 <= colorV3) {
			colorC = colorV2;
			c = v2;		
			if (colorV1 <= colorV3) {
				colorB = colorV1;	
				colorA = colorV3;
				b = v1;
				a = v3;
			} else {
				colorB = colorV3;
				colorA = colorV1;
				b = v3;
				a = v1;
			}			
		} else if (colorV3 <= colorV1 && colorV3 <= colorV2) {
			colorC = colorV3;
			c = v3;		
			if (colorV1 <= colorV2) {
				colorB = colorV1;	
				colorA = colorV2;
				b = v1;
				a = v2;
			} else {
				colorB = colorV2;
				colorA = colorV1;
				b = v2;
				a = v1;
			}			
		}	
		
		//ColorB & ColorC should not be the same
		int tempColor;
		Vector3D tempVector = new Vector3D();
		if(colorC == colorB) {
			tempColor = colorB;
			colorB = colorA;
			colorA = tempColor;
			
			tempVector = b;
			b = a;
			a = tempVector;
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
		if(colorA == colorB && colorA == colorC) {
			int nbPolygons = 1;
			res.setNbPolygons(nbPolygons);	
			int curColor = colorC;		
			res.setPolygonColor(curColor, 0);
			Vector3D[] curPolygon = {c, b, a};
			res.setPolygon(curPolygon, 0);

		} else {
			
			int nbPolygons = getNbPolygons(colorA, colorB, colorC);
			res.setNbPolygons(nbPolygons);
			
			//starting the polygon decomposition with the smallest color
			prevI1 = c;
			prevI2 = c;
			
			prevSideI1 = Side.SIDE_BC;
			prevSideI2 = Side.SIDE_AC;

			for(int i = 0 ; i < nbPolygons - 1 ; i++) {			
				// color of the next polygon
				int curColor = min + i;			

				res.setPolygonColor(curColor, i);			
				Vector3D[] polygonDecomposed = getPolygon(prevI1, prevI2, prevSideI1, prevSideI2, curColor);
				res.setPolygon(polygonDecomposed, i);
				
				//coordinates for the next polygon which will decomposed
				prevI1 = nextI1;
				prevI2 = nextI2;
				prevSideI1 = nextSideI1;
				prevSideI2 = nextSideI2;
			}	

			//ending the polygon decomposition with the biggest color
			Vector3D[] lastPoly = new Vector3D[3];
			lastPoly[0] = prevI1;
			lastPoly[1] = prevI2;
			lastPoly[2] = findIntersection(prevSideI1, prevSideI2);
			
			res.setPolygonColor(max, nbPolygons - 1);
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
	public int getNbPolygons(int colorA, int colorB, int colorC) {
		min = Math.min(Math.min(colorC, colorB), colorA);
		max = Math.max(Math.max(colorC, colorB), colorA);		
		return max - min + 1;
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
	public Vector3D[] getBorder(Vector3D vertexA, Vector3D vertexB,Vector3D vertexC, 
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
	 * @param vectorCB Vector3D
	 * @param vectorCA Vector3D
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
	 * @param curColor double
	 * @return intersections
	 */
	public void intersectionsManager(double curColor, Vector3D[] intersections, Side[] intersectionSides) {
		
		int nbIntersections = 0;		

		/** Test for (P1, P2) & (BC) */
		if (colorB != colorC) {
			double res1 = (colorC - curColor) / (colorC - colorB);
			if(res1 >= 0 && res1 <= 1) {
				Vector3D i1 = new Vector3D(res1, 0.0, 0.0);
				intersections[nbIntersections] = i1;
				intersectionSides[nbIntersections] = Side.SIDE_BC;
				nbIntersections++;				
			}
		}		
		
		/** Test for (P1, P2) & (AC) */
		if (colorA != colorC) {
			double res2 = (colorC - curColor) / (colorC - colorA);

			if(res2 >= 0 && res2 <= 1 && colorC != colorA) {
				Vector3D i2 = new Vector3D(0.0, res2, 0.0);
				intersections[nbIntersections] = i2;
				intersectionSides[nbIntersections] = Side.SIDE_AC;
				nbIntersections++;
			}
		}
		
		/** Test for (P1, P2) & (AB) */
		if (colorA != colorB) {
			double res3 = (colorA - curColor) / (colorA - colorB);
			double res4 = (colorB - curColor) / (colorB - colorA);

			if(res3 >= 0 && res4 >= 0 && colorA != colorB) {
				Vector3D i3 = new Vector3D(res3, res4, 0.0);
				intersections[nbIntersections] = i3;
				intersectionSides[nbIntersections] = Side.SIDE_AB;
				nbIntersections++;
			}
		}
		
		//compare i1 & i2
		if (intersections[0].getY() > intersections[1].getY()) {
			Vector3D tempVector = null;
			tempVector = intersections[0];
			intersections[0] = intersections[1];
			intersections[1] = tempVector;
			Side tmpSide = intersectionSides[0];
			intersectionSides[0] = intersectionSides[1];
			intersectionSides[1] = tmpSide;			
		} 	
		
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
	public Vector3D[] getPolygon(Vector3D prevI1, Vector3D prevI2, Side prevSideI1, Side prevSideI2, int curColor) {
				
		Vector3D[] polygons = null;
		Vector3D p = new Vector3D();	
		
		Vector3D[] intersections = new Vector3D[2];
		Side[] intersectionsSides = new Side[2];
		
		
		//finding nextI1 and nextI2	
		intersectionsManager(curColor + 0.5, intersections, intersectionsSides);		
		nextI1 = intersections[0];
		nextI2 = intersections[1];
		nextSideI1 = intersectionsSides[0];
		nextSideI2 = intersectionsSides[1];
		
		
		//if the decomposed polygon have 4 or 5 summits
		//coordinates are ordered in the reverse sense of the watch
		if (prevSideI1 == nextSideI1 && prevSideI2 == nextSideI2) {
			polygons = new Vector3D[4];
			
			polygons[0] = prevI1;
			polygons[1] = prevI2;
			polygons[2] = nextI2;
			polygons[3] = nextI1;
			
		} else if(prevSideI1 != nextSideI1) {
			p = findIntersection(prevSideI1, nextSideI1);
			polygons = new Vector3D[5];
			polygons[0] = prevI1;
			polygons[1] = prevI2;			
			polygons[2] = nextI2;
			polygons[3] = nextI1;
			polygons[4] = p;
			
		} else if(prevSideI2 != nextSideI2) {
			p = findIntersection(prevSideI2, nextSideI2);
			polygons = new Vector3D[5];
			polygons[0] = prevI1;
			polygons[1] = prevI2;	
			polygons[2] = p;
			polygons[3] = nextI2;
			polygons[4] = nextI1;
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
		Vector3D p = new Vector3D();
		
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

}

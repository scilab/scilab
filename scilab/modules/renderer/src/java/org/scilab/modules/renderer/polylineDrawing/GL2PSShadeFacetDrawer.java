/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Sylvestre Koumar
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.renderer.polylineDrawing;

import javax.media.opengl.GL;

import org.scilab.modules.renderer.utils.TexturedColorMap;
import org.scilab.modules.renderer.utils.geom3D.ColoredTriangle;
import org.scilab.modules.renderer.utils.geom3D.TriangleDecomposition;
import org.scilab.modules.renderer.utils.geom3D.Vector3D;
/**
 * Decompose triangle with GL2PS
 *
 */
public class GL2PSShadeFacetDrawer implements ShadeFacetDrawer {
	
	/** Number of side of the polygon */
	private static final int TRIANGLE = 3;
	private static final int SQUARE = 4;
	
	/** Coordinates of the triangle or the square */
	private Vector3D a;
	private Vector3D b;
	private Vector3D c;
	private Vector3D d;
	
	private GL gl;
	private TexturedColorMap colorMaps;
	
	/**
	 * Default constructor
	 */
	public GL2PSShadeFacetDrawer() {

	}

	/**
	 * Paint the polygon given with the table of color
	 * @param coords coordinates of triangle & square
	 * @param colors table of color
	 * @param gl GL 
	 * @param colorMap TexturedColorMap
	 */
	public void paintPolygon(Vector3D[] coords, int[] colors, GL gl,
							 TexturedColorMap colorMap) {	
		
		this.gl = gl;
		this.colorMaps = colorMap;
		
		if (coords.length == TRIANGLE) { 
			//Coordinates of the triangle
			a = new Vector3D(coords[0]);
			b = new Vector3D(coords[1]);
			c = new Vector3D(coords[2]);
			
			paintTriangle(a, b, c, colors[0], colors[1], colors[2]);
			
		} else if (coords.length >= SQUARE) {
			//Coordinates of the square
			//closing the square case
			a = new Vector3D(coords[0]);
			b = new Vector3D(coords[1]);
			c = new Vector3D(coords[2]);
			d = new Vector3D(coords[TRIANGLE]);
			
			//we separate the square on 2 triangle then we work on each triangle
			paintTriangle(a, b, c, colors[0], colors[1], colors[2]);
			paintTriangle(c, d, a, colors[2], colors[TRIANGLE], colors[0]);
			
		}
		
	}
	
	/**
	 * Decompose a triangle in polygons then paint them
	 * @param a coordinates of the polylines vertices
	 * @param b coordinates of the polylines vertices
	 * @param c coordinates of the polylines vertices
	 * @param color1 color of the polylines vertices
	 * @param color2 color of the polylines vertices
	 * @param color3 color of the polylines vertices
	 */
	public void paintTriangle(Vector3D a, Vector3D b, Vector3D c, double color1, double color2, double color3) {		
		
		//Calling this class will decompose triangle in colored polygons
		
		ColoredTriangle ct = new ColoredTriangle(a, b, c, color1, color2, color3);
		TriangleDecomposition td = ct.decomposeTriangle();	

		for (int i = 0; i < td.getNbPolygons(); i++) {
			int color = td.getPolygonColor(i);	

			double[] polyColor = colorMaps.getColor(color);
			gl.glColor3d(polyColor[0], polyColor[1], polyColor[2]);
			Vector3D[] polygon = td.getPolygon(i);			
			
			paintPolygon(gl, polygon);
		}
	}
	
	/**
	 * This function paint the first triangle and the following triangles for the decomposed polygon
	 * @param gl GL
	 * @param polygon decomposed polygon 
	 */
	public void paintPolygon(GL gl, Vector3D[] polygon) {
		
		// first triangle & following triangles
		for (int k = 1; k < polygon.length - 1; k++) {
			gl.glVertex3d(polygon[0].getX(), polygon[0].getY(), polygon[0].getZ());
			gl.glVertex3d(polygon[k].getX(), polygon[k].getY(), polygon[k].getZ());
			gl.glVertex3d(polygon[k + 1].getX(), polygon[k + 1].getY(), polygon[k + 1].getZ());
		}
		
	}

	/**
	 * Paint the polygon given with the table of color (not used here)
	 * @param triangleCoords coordinates of triangle & square
	 * @param triangleColors table of color
	 * @param gl GL 
	 * @param colorMap TexturedColorMap
	 */
	public void paintPolygon(Vector3D[] triangleCoords,
			double[] triangleColors, GL gl, TexturedColorMap colorMap) {
		// TODO Auto-generated method stub
		
	}

}

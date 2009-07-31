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
 * Class which decompose triangle
 */
public class TriangleDecomposition {
	
	private Vector3D[][] polygons;
	private int[] polygonColors;
	
	/**
	 * Default constructor
	 */
	public TriangleDecomposition() {
		polygons = null;
		polygonColors = null;
	}
	
	/**
	 * Get the number of polygons in the triangle
	 * @return the number of polygons in the triangle
	 */
	public int getNbPolygons() {
		return polygons.length;
	}
	
	/**
	 * Get the color of the polygon given in index
	 * @param index of the polygon
	 * @return the color of the polygon
	 */
	public int getPolygonColor(int index) {
		return polygonColors[index];
	}
	
	/**
	 * Get the polygon
	 * @param index of the polygon
	 * @return a polygon
	 */
	public Vector3D[] getPolygon(int index) {
		return polygons[index];
	}

	/**
	 * Set the number of polygons
	 * @param nbPolygons int
	 */
	protected void setNbPolygons(int nbPolygons) {
		polygons = new Vector3D[nbPolygons][];
		polygonColors = new int[nbPolygons];
		
	}
	
	/**
	 * Set polygon ordered in a Vector3D
	 * @param polygonVertices table of polygons
	 * @param polygonIndex index of the polygon
	 */
	protected void setPolygon(Vector3D[] polygonVertices, int polygonIndex) {
		int nbVertices = polygonVertices.length;
		this.polygons[polygonIndex] = new Vector3D[nbVertices];
		for (int i = 0; i < nbVertices; i++) {
			this.polygons[polygonIndex][i] = new Vector3D(polygonVertices[i]);
		}
	}

	/**
	 * Set color of the polygon
	 * @param color index of the color
	 * @param polygonIndex of the polygon
	 */
	protected void setPolygonColor(double color, int polygonIndex) {
		this.polygonColors[polygonIndex] = (int) Math.round(color);
	}
	
	


}

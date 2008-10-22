/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class drawing the head of segs arrows 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


package org.scilab.modules.renderer.segsDrawing;

import org.scilab.modules.renderer.drawers.ArrowHeadDrawerGL;
import org.scilab.modules.renderer.utils.geom3D.Vector3D;

/**
 * Class drawing the head of segs arrows
 * @author Jean-Baptiste Silvy
 */
public class SegsArrowDrawerGL extends ArrowHeadDrawerGL {
	
	/**
	 * Default constructor
	 */
	public SegsArrowDrawerGL() {
		super();
	}
	
	/**
	 * Draw a set of segment knowing their positions and colors
	 * @param startXCoords X coordinate of segments first point
	 * @param endXCoords X coordinate of segments end point
	 * @param startYCoords Y coordinate of segments first point
	 * @param endYCoords Y coordinate of segments end point
	 * @param startZCoords Z coordinate of segments first point
	 * @param endZCoords Z coordinate of segments end point
	 * @param colors array containing the color of each segment
	 */
	public void drawSegs(double[] startXCoords, double[] endXCoords, double[] startYCoords, double[] endYCoords,
						 double[] startZCoords, double[] endZCoords, int[] colors) {
		int nbSegs = startXCoords.length;
		
		// set the colors of each arrow head
		// convert colors to Scilab colors
		for (int i = 0; i < nbSegs; i++) {
			colors[i] = getColorMap().convertScilabToColorMapIndex(colors[i]);
		}
		setColors(colors);
		
		Vector3D[] startPoints = new Vector3D[nbSegs];
		Vector3D[] endPoints = new Vector3D[nbSegs];
		
		for (int i = 0; i < nbSegs; i++) {
			startPoints[i] = new Vector3D(startXCoords[i], startYCoords[i], startZCoords[i]);
			endPoints[i] = new Vector3D(endXCoords[i], endYCoords[i], endZCoords[i]);
		}
		drawArrowHeads(startPoints, endPoints);
		
	}

	/**
	 * Draw the segs using the precomputed values
	 */
	public void drawSegs() {
		redrawArrowHeads();
	}
	
}

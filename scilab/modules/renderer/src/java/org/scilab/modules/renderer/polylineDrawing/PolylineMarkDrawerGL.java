/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class containing the driver dependant routines to draw marks
 * on a polyline 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


package org.scilab.modules.renderer.polylineDrawing;


import org.scilab.modules.renderer.drawers.MarkDrawerGL;
import org.scilab.modules.renderer.utils.geom3D.Vector3D;

/**
 * Class containing the driver dependant routines to draw marks
 *        on a polyline
 * @author Jean-Baptiste Silvy
 */
public class PolylineMarkDrawerGL extends MarkDrawerGL implements PolylineDrawerStrategy {
	
	/**
	 * Default constructor
	 */
	public PolylineMarkDrawerGL() {
		super();
	}
	
	/**
	 * Display the object by displaying its display list
	 * @param parentFigureIndex index of the parent figure in which the object will be drawn
	 */
	@Override
	public void show(int parentFigureIndex) {
		showMarks();
	}

	/**
	 * Create the display list for the polyline
	 * @param xCoords X coordinates of the polylines vertices
	 * @param yCoords Y coordinates of the polylines vertices
	 * @param zCoords Z coordinates of the polylines vertices
	 */
	public void drawPolyline(double[] xCoords, double[] yCoords, double[] zCoords) {
		Vector3D[] verticesPositions = new Vector3D[xCoords.length];
		for (int i = 0; i < xCoords.length; i++) {
			verticesPositions[i] = new Vector3D(xCoords[i], yCoords[i], zCoords[i]);
		}
		
		drawMarks(verticesPositions);
		
	}
	
	/**
	 * Draw the mark on the polyline using the stored vertices.
	 */
	public void drawPolyline() {
		redrawMarks();
	}

}

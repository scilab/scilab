/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class containing the driver dependant routines to draw arrow 
 * heads on polylines 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


package org.scilab.modules.renderer.polylineDrawing;

import org.scilab.modules.renderer.drawers.ArrowHeadDrawerGL;
import org.scilab.modules.renderer.utils.geom3D.Vector3D;

/**
 * Class containing the driver dependant routines to draw arrow
 * heads on polylines.    
 * @author Jean-Baptiste Silvy
 */
public class PolylineArrowDrawerGL extends ArrowHeadDrawerGL implements PolylineDrawerStrategy {
	
	/**
	 * Default contructor
	 */
	public PolylineArrowDrawerGL() {
		super();
	}
	
	/**
	 * Create the display list for the polyline
	 * @param xCoords X coordinates of the polylines vertices
	 * @param yCoords Y coordinates of the polylines vertices
	 * @param zCoords Z coordinates of the polylines vertices
	 */
	public void drawPolyline(double[] xCoords, double[] yCoords, double[] zCoords) {
		
		Vector3D[] startPoints = new Vector3D[xCoords.length - 1];
		Vector3D[] endPoints = new Vector3D[xCoords.length - 1];
		
		// store the two endpoints of each arrow
		for (int i = 0; i < xCoords.length - 1; i++) {
			startPoints[i] = new Vector3D(xCoords[i], yCoords[i], zCoords[i]);
			endPoints[i] = new Vector3D(xCoords[i + 1], yCoords[i + 1], zCoords[i + 1]);
		}
		
		drawArrowHeads(startPoints, endPoints);
	}
	
	/**
	 * Draw the stored arrows coordinates.
	 */
	public void drawPolyline() {
		redrawArrowHeads();
	}
	
	

}

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
import org.scilab.modules.renderer.utils.geom3D.GeomAlgos;
import org.scilab.modules.renderer.utils.geom3D.Vector3D;
import java.util.LinkedList;

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
		
		LinkedList<Vector3D> startPoints = new LinkedList<Vector3D>();
		LinkedList<Vector3D> endPoints = new LinkedList<Vector3D>();
		
		// store the two endpoints of each arrow
		for (int i = 0; i < xCoords.length - 1; i++) {
			if (GeomAlgos.isVector3DRepresentable(xCoords[i], yCoords[i], zCoords[i])
					&& GeomAlgos.isVector3DRepresentable(xCoords[i + 1], yCoords[i + 1], zCoords[i + 1]) ) {
				startPoints.addLast(
					new Vector3D(
						GeomAlgos.glRepresentable(xCoords[i]),
						GeomAlgos.glRepresentable(yCoords[i]),
						GeomAlgos.glRepresentable(zCoords[i])
					)
				);
				endPoints.addLast(
					new Vector3D(
						GeomAlgos.glRepresentable(xCoords[i + 1]),
						GeomAlgos.glRepresentable(yCoords[i + 1]),
						GeomAlgos.glRepresentable(zCoords[i + 1])
					)
				);
			}
		}
		
		drawArrowHeads(startPoints.toArray(new Vector3D[0]), endPoints.toArray(new Vector3D[0]));
	}
	
	/**
	 * Draw the stored arrows coordinates.
	 */
	public void drawPolyline() {
		redrawArrowHeads();
	}
	
	

}

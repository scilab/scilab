/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class containing the driver dependant routines to fill the area
 * enclosed by the polyline with interpolated colors 
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
import org.scilab.modules.renderer.drawers.FillDrawerGL;
import org.scilab.modules.renderer.utils.TexturedColorMap;
import org.scilab.modules.renderer.utils.geom3D.Vector3D;


/**
 * Class containing the driver dependant routines to fill the area
 * enclosed by the polyline with interpolated colors.
 * Note that this class might be more generic by using a glu tesselator like
 * in fill drawing. 
 * @author Jean-Baptiste Silvy
 */
public class PolylineInterpColorDrawerGL extends FillDrawerGL {	
	
	/**
	 * Default constructor
	 */
	public PolylineInterpColorDrawerGL() {
		
	}
	
	/**
	 * Create the display list for the polyline
	 * @param xCoords X coordinates of the polylines vertices
	 * @param yCoords Y coordinates of the polylines vertices
	 * @param zCoords Z coordinates of the polylines vertices
	 * @param colors colors index for each vertex
	 */
	public void drawPolyline(double[] xCoords, double[] yCoords,
							 double[] zCoords, int[] colors) {
		
		GL gl = getGL();		
		TexturedColorMap colorMap = getColorMap();
		
		Vector3D[] vertex = new Vector3D[xCoords.length];
		for (int i = 0; i < xCoords.length; i++) {
			vertex[i] = new Vector3D(xCoords[i], yCoords[i], zCoords[i]);
		}
		
		ShadeFacetDrawer sfd = getParentFigureGL().getShadeFacetDrawer();		
		sfd.paintPolygon(vertex, colors, gl, colorMap);		
	}	

}

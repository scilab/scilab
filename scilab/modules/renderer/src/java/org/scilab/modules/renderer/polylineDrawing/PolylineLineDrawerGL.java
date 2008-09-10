/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class containing the driver dependant routines to draw the
 * main line of a polyline
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

import org.scilab.modules.renderer.drawers.LineDrawerGL;
import org.scilab.modules.renderer.utils.geom3D.GeomAlgos;
import org.scilab.modules.renderer.utils.glTools.GLTools;

/**
 * Class containing the driver dependant routines to draw the
 *        main line of a polyline 
 * @author Jean-Baptiste Silvy
 */
public class PolylineLineDrawerGL extends LineDrawerGL implements PolylineDrawerStrategy {

	/**
	 * Default constructor
	 */
	public PolylineLineDrawerGL() {
		super();
	}
	
	/**
	 * Create the display list for the polyline
	 * @param xCoords X coordinates of the polylines vertices
	 * @param yCoords Y coordinates of the polylines vertices
	 * @param zCoords Z coordinates of the polylines vertices
	 */
	public void drawPolyline(double[] xCoords, double[] yCoords, double[] zCoords) {
		
		GL gl = getGL();
		int nbLines = xCoords.length;
		
		// check if there is somthing to draw
		if (nbLines == 0) { return; }
		
		// set dash mode
		GLTools.beginDashMode(gl, getLineStyle(), getThickness());
		
		// set color
		double[] color = getLineColor();
		gl.glColor3d(color[0], color[1], color[2]);
		
		
		gl.glBegin(GL.GL_LINES);
		// the three arrays should have the same size
		
		int i = 0;
		// search for the first finite element
		while (!GeomAlgos.isVector3DFinite(xCoords[i], yCoords[i], zCoords[i])) {
			i++;
		}
		
		for ( ; i < nbLines - 1; i++) {
			if (GeomAlgos.isVector3DFinite(xCoords[i + 1], yCoords[i + 1], zCoords[i + 1])) {
				gl.glVertex3d(xCoords[i], yCoords[i], zCoords[i]);
				gl.glVertex3d(xCoords[i + 1], yCoords[i + 1], zCoords[i + 1]);
				
			} else {
				//	this line can't be displayed neither the next one
				i++;
			}
			
		}
		gl.glEnd();
		
		GLTools.endDashMode(gl);
		
	}
	
}

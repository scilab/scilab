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
		
		// check if there is something to draw
		// the three arrays should have the same size
		if (nbLines == 0) { return; }
		
		// set dash mode
		GLTools.beginDashMode(gl, getLineStyle(), getThickness());
		
		// set color
		double[] color = getLineColor();
		gl.glColor3d(color[0], color[1], color[2]);
		
		
		// The polyline might be cutted in several pieces if data contained some %nan values.
		// So several glBegin/glEnd pairs might be needed.
		// To get dashes along the whole polyline and not only triangles
		// We need to use GL_LINE_STRIP instead of GL_LINES
		boolean previousValueIsNan = true;
		for (int i = 0; i < nbLines; i++) {
			if (GeomAlgos.isVector3DRepresentable(xCoords[i], yCoords[i], zCoords[i])) {
				if (previousValueIsNan) {
					// new line sequence
					// check if there are at least two consecutive valid values.
					if (i < nbLines - 1 && GeomAlgos.isVector3DRepresentable(xCoords[i + 1], yCoords[i + 1], zCoords[i + 1])) {
						gl.glBegin(GL.GL_LINE_STRIP);
						previousValueIsNan = false;
					}
				}

				gl.glVertex3f(
						GeomAlgos.glRepresentable(xCoords[i]),
						GeomAlgos.glRepresentable(yCoords[i]),
						GeomAlgos.glRepresentable(zCoords[i]));
        
			} else if (!previousValueIsNan) {
				// stop recording
				// end the last polyline
				gl.glEnd();
				previousValueIsNan = true;
			}
			
		}
		if (!previousValueIsNan) {
			// end the last polyline
			gl.glEnd();
		}
		
		GLTools.endDashMode(gl);
		
	}
	
}

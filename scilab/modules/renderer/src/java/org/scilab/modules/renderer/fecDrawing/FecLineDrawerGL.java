/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * desc : Class drawing the mesh of fec object  
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


package org.scilab.modules.renderer.fecDrawing;

import javax.media.opengl.GL;

import org.scilab.modules.renderer.drawers.LineDrawerGL;
import org.scilab.modules.renderer.utils.glTools.GLTools;

/**
 * Class drawing the mesh of fec object
 * @author Jean-Baptiste Silvy
 */
public class FecLineDrawerGL extends LineDrawerGL {

	/**
	 * Default constructor
	 */
	public FecLineDrawerGL() {
		super();
	}

	
	/**
	 * Draw the triangles composing the fec object.
	 * @param xCoords X coordinates of nodes
	 * @param yCoords Y coordinates of nodes
	 * @param values value corresponding to each node
	 * @param firstPoints node indexes of triangles first point
	 * @param secondPoints node indexes of triangles second point
	 * @param thirdPoints node indexes of triangles third point
	 */
	public void drawFec(double[] xCoords, double[] yCoords, double[] values,
						int[] firstPoints, int[] secondPoints, int[] thirdPoints) {
		int nbTriangles = firstPoints.length;
		
		GL gl =  getGL();
		
		GLTools.beginDashMode(gl, getLineStyle(), getThickness());
		
		double[] color = getLineColor();
		gl.glColor3d(color[0], color[1], color[2]);
		
		gl.glBegin(GL.GL_LINES);
		for (int i = 0; i < nbTriangles; i++) {
			// draw the triangle defined by the current triangle
			gl.glVertex3d(xCoords[firstPoints[i]], yCoords[firstPoints[i]], 0.0);
			gl.glVertex3d(xCoords[secondPoints[i]], yCoords[secondPoints[i]], 0.0);
			
			gl.glVertex3d(xCoords[secondPoints[i]], yCoords[secondPoints[i]], 0.0);
			gl.glVertex3d(xCoords[thirdPoints[i]], yCoords[thirdPoints[i]], 0.0);
			
			gl.glVertex3d(xCoords[thirdPoints[i]], yCoords[thirdPoints[i]], 0.0);
			gl.glVertex3d(xCoords[firstPoints[i]], yCoords[firstPoints[i]], 0.0);
		}
		gl.glEnd();
		
		GLTools.endDashMode(gl);
		
	}
	
}

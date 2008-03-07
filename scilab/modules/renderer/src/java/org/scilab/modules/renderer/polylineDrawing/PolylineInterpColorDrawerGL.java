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

import com.sun.opengl.util.texture.Texture;

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
		
		Texture colormapTexture = getColorMap().getTexture();
		colormapTexture.enable();
		colormapTexture.bind();
		
		gl.glColor3d(0.0, 1.0, 0.0);
		gl.glBegin(GL.GL_TRIANGLE_FAN); // works with triangles and quads
		for (int i = 0; i < xCoords.length; i++) {
			getColorMap().applyTexCoord(gl, colors[i]);
			gl.glVertex3d(xCoords[i], yCoords[i], zCoords[i]);
		}
		gl.glEnd();
		colormapTexture.disable();
			
	}
	
	
}

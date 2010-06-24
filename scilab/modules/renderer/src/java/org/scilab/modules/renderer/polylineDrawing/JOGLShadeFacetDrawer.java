/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Sylvestre Koumar
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

import org.scilab.modules.renderer.utils.TexturedColorMap;
import org.scilab.modules.renderer.utils.geom3D.Vector3D;

/**
 * Decompose triangle with JOGL
 *
 */
public class JOGLShadeFacetDrawer implements ShadeFacetDrawer {
	
	/**
	 * Default constructor
	 */
	public JOGLShadeFacetDrawer() {

	}

	/**
	 * Paint the polygon given with the table of color
	 * @param coords coordinates of triangle & square
	 * @param colors table of color
	 * @param gl GL 
	 * @param colorMap TexturedColorMap
	 */
	public void paintPolygon(Vector3D[] coords, int[] colors, GL gl,
							 TexturedColorMap colorMap) {
		
		for (int i = 0; i < coords.length; i++) {
			colorMap.applyTexCoord(gl, colors[i]);
			gl.glVertex3d(coords[i].getX(), coords[i].getY(), coords[i].getZ());
		}
		
	}

	
	/**
	 * Paint the polygon given with the table of color
	 * @param triangleCoords coordinates of triangle & square
	 * @param triangleColors table of color
	 * @param gl GL 
	 * @param colorMap TexturedColorMap
	 */
	public void paintPolygon(Vector3D[] triangleCoords,
			double[] triangleColors, GL gl, TexturedColorMap colorMap) {
		
		for (int i = 0; i < triangleCoords.length; i++) {
			colorMap.applyTexCoord(gl, triangleColors[i]);
			gl.glVertex3d(triangleCoords[i].getX(), triangleCoords[i].getY(), triangleCoords[i].getZ());
		}
	}
}

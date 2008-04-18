/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * desc : Class used to draw uniformy colored facets
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


package org.scilab.modules.renderer.surfaceDrawing;

import javax.media.opengl.GL;

import org.scilab.modules.renderer.figureDrawing.DrawableFigureGL;
import org.scilab.modules.renderer.polylineDrawing.ShadeFacetDrawer;
import org.scilab.modules.renderer.utils.TexturedColorMap;
import org.scilab.modules.renderer.utils.geom3D.Vector3D;

/**
 * Class used to draw uniformy colored facets
 * @author Jean-Baptiste Silvy
 */
public class FlatShadedFacetDrawer extends FacetDrawerGL {
	

	
	/**
	 * Default constructor
	 * @param colorMap colormap to use.
	 */
	public FlatShadedFacetDrawer(TexturedColorMap colorMap) {
		super(colorMap);
	}
	
	/**
	 * Draw a facet
	 * @param vertices vertices of the facet (3 or 4).
	 * @param colors array containing the colors for the vertices.
	 * @param gl current OpenGL pipeline
	 */
	public void drawFacet(GL gl, Vector3D[] vertices, int[] colors) {
		
		
		gl.glBegin(GL.GL_POLYGON);
		
		// draw one facet with only one color
		double[] color = getColorMap().getColor(colors[0]);
		gl.glColor3d(color[0], color[1], color[2]);
		
		for (int i = 0; i < getNbVertices(); i++) {
			gl.glVertex3d(vertices[i].getX(), vertices[i].getY(), vertices[i].getZ());
		}
		
		gl.glEnd();
		
		
		if (getHiddenColor() != null) {
			// draw hidden color
			double[] hiddenColor = getHiddenColor();
			gl.glColor3d(hiddenColor[0], hiddenColor[1], hiddenColor[2]);
			
			gl.glBegin(GL.GL_POLYGON);
			
			// draw on reverse since back face culling is enable
			for (int i = getNbVertices() - 1; i >= 0; i--) {
				gl.glVertex3d(vertices[i].getX(), vertices[i].getY(), vertices[i].getZ());
			}
			gl.glEnd();
		}
		
		

	}

}

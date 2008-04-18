/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * desc : Class used to draw facet with different colors on each edge
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

import com.sun.opengl.util.texture.Texture;

/**
 * Class used to draw uniformy colored facets
 * @author Jean-Baptiste Silvy
 */
public class LinearShadedFacetDrawerGL extends FacetDrawerGL {

	private static final int TRIANGLE_NB_FACETS = 3;
	private static final int QUAD_NB_FACETS = 4;
	
	private Texture colorMapTexture;
	private ShadeFacetDrawer sfd;
	
	/**
	 * Default constructor
	 * @param colorMap colormap to use.
	 */
	public LinearShadedFacetDrawerGL(TexturedColorMap colorMap, ShadeFacetDrawer sfd) {
		super(colorMap);
		this.sfd = sfd;
	}
	
	/**
	 * To be called before any drawn action.
	 * @param gl current OpenGL pipeline
	 */
//	public void initializeDrawing(GL gl) {
//		
//		// bind texture before calling glBegin;
//		colorMapTexture = getColorMap().getTexture();
//		colorMapTexture.enable();
//		colorMapTexture.bind();
//		
//		super.initializeDrawing(gl);
//	}
	
	/**
	 * To be called when drawing ends.
	 * @param gl current OpenGL pipeline
	 */
//	public void endDrawing(GL gl) {
//		super.endDrawing(gl);
//		colorMapTexture.disable();
//		
//	}
	
	/**
	 * Draw a facet
	 * @param vertices vertices of the facet (3 or 4).
	 * @param colors array containing the colors channels for the vertices.
	 * @param gl current OpenGL pipeline
	 */
	public void drawFacet(GL gl, Vector3D[] vertices, int[] colors) {
		
		//Paint the polygon given with the table of color
		sfd.paintPolygon(vertices, colors, gl, getColorMap());		
	
		if (getHiddenColor() != null) {
			// draw hidden color
			gl.glColor3d(getHiddenColor()[0], getHiddenColor()[1], getHiddenColor()[2]);
			
			// draw on reverse since back face culling is enable
			gl.glBegin(GL.GL_POLYGON);
			for (int i = getNbVertices() - 1; i >= 0; i--) {
				gl.glVertex3d(vertices[i].getX(), vertices[i].getY(), vertices[i].getZ());
			}
			gl.glEnd();
		}

	}

}

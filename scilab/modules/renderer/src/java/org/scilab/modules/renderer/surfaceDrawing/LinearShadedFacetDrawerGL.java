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

import org.scilab.modules.renderer.polylineDrawing.ShadeFacetDrawer;
import org.scilab.modules.renderer.utils.TexturedColorMap;
import org.scilab.modules.renderer.utils.geom3D.GeomAlgos;
import org.scilab.modules.renderer.utils.geom3D.Vector3D;

import com.sun.opengl.util.texture.Texture;

/**
 * Class used to draw uniformy colored facets
 * @author Jean-Baptiste Silvy
 */
public class LinearShadedFacetDrawerGL extends FacetDrawerGL {

	private ShadeFacetDrawer sfd;
	
	/**
	 * Default constructor
	 * @param colorMap colormap to use.
	 * @param sfd ShadeFacetDrawer
	 */
	public LinearShadedFacetDrawerGL(TexturedColorMap colorMap, ShadeFacetDrawer sfd) {
		super(colorMap);
		this.sfd = sfd;
	}
	
	/**
	 * To be called before any drawn action.
	 * @param gl current OpenGL pipeline
	 */
	public void initializeDrawing(GL gl) {
		
		// bind texture before calling glBegin;
		Texture colorMapTexture = getColorMap().getTexture();
		colorMapTexture.enable();
		colorMapTexture.bind();
		
		super.initializeDrawing(gl);
	}
	
	/**
	 * To be called when drawing ends.
	 * @param gl current OpenGL pipeline
	 */
	public void endDrawing(GL gl) {
		super.endDrawing(gl);
		Texture colorMapTexture = getColorMap().getTexture();
		colorMapTexture.disable();
		
	}
	
	/**
	 * Draw a triangle with one color in reverse order compared to the one given by vertice
	 * @param gl current OpenGL pipeline
	 * @param vertices vertices of the triangle (size 3)
	 * @param colorIndex index of the color to apply
	 */
	protected void drawBackTriangle(GL gl, Vector3D[] vertices, int colorIndex) {
		// texture is enable here, it is not possible to use color directly
		// or the result will be hidden by the texture.
		// However, all the colors are in the texture, so use it!
		getColorMap().applyTexCoord(gl, colorIndex);
		
		for (int i = TRIANGLE_NB_EDGE - 1; i >= 0; i--) {
			gl.glVertex3d(vertices[i].getX(), vertices[i].getY(), vertices[i].getZ());
		}
	}
	
	/**
	 * Draw a facet
	 * @param vertices vertices of the facet (3 or 4).
	 * @param colors array containing the colors channels for the vertices.
	 * @param gl current OpenGL pipeline
	 */
	public void drawFacet(GL gl, Vector3D[] vertices, int[] colors) {
		
		//Paint the polygon given with the table of color
		
		// if all the colors within the facet are 0, the don't draw it
		boolean paintFacet = false;
		
		for (int i = 0; i < colors.length; i++) {
			if (colors[i] > 0) {
				paintFacet = true;
			}
			colors[i] = getColorMap().convertScilabToColorMapIndex(colors[i]);
		}
		
		if (!paintFacet || vertices.length < TRIANGLE_NB_EDGE) {
			// all the colors are 0 or this is not a facet
			return;
		}
	
		// for now draw only polygons with 3 or 4 edges
		if (vertices.length == TRIANGLE_NB_EDGE) {
			// only a triangle
			paintOneTriangle(vertices, colors, getColorMap(), gl);
			
		} else if (vertices.length == QUAD_NB_EDGE) {
			
			// its a quad, decompose it into 2 triangles
			Vector3D[] triangle1 =  new Vector3D[TRIANGLE_NB_EDGE];
			Vector3D[] triangle2 =  new Vector3D[TRIANGLE_NB_EDGE];
			int[] colorT1 = new int[TRIANGLE_NB_EDGE];
			int[] colorT2 = new int[TRIANGLE_NB_EDGE];
			GeomAlgos.decomposeQuad(vertices, colors, triangle1, colorT1, triangle2, colorT2);
			
			// draw the two decomposed triangles
			paintOneTriangle(triangle1, colorT1, getColorMap(), gl);
			paintOneTriangle(triangle2, colorT2, getColorMap(), gl);
			
		} else {
			// draw the facet using several triangles
			// use this method for now. It will work for flat facets
			// but is not optimal for non flat ones (in comparison with the decomposeQuad function)
			int nbTriangles = vertices.length - 2;
			for (int i = 1; i <= nbTriangles; i++) {
				Vector3D[] triangle = {vertices[0], vertices[i], vertices[i + 1]};
				int[] triangleColors = {colors[0], colors[i], colors[i + 1]};
				paintOneTriangle(triangle, triangleColors, getColorMap(), gl);
			}
		}
	}
		
	/**
	 * Paint both face of a triangle
	 * @param vertices 3 vertices of the triangle
	 * @param colors 3 colors indices, one for each vertex
	 * @param cmap colorMap
	 * @param gl current OpenGL pipeline
	 */
	private void paintOneTriangle(Vector3D[] vertices, int[] colors, TexturedColorMap cmap, GL gl) {
		// paint front face
		sfd.paintPolygon(vertices, colors, gl, cmap);
		
		// paint hidden face if needed
		if (isDisplayingHiddenSurface()) {
			drawBackTriangle(gl, vertices, getHiddenColorIndex());
		}
	}

	

}

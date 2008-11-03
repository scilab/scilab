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

import org.scilab.modules.renderer.utils.TexturedColorMap;
import org.scilab.modules.renderer.utils.geom3D.GeomAlgos;
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
	 * Draw a triangle with one color preserving the orientation given by vertice
	 * @param gl current OpenGL pipeline
	 * @param vertices vertices of the triangle (size 3)
	 * @param color color to apply
	 */
	private void drawFrontTriangle(GL gl, Vector3D[] vertices, double[] color) {
		gl.glColor3d(color[0], color[1], color[2]);
		
		for (int i = 0; i < TRIANGLE_NB_EDGE; i++) {
			gl.glVertex3d(vertices[i].getX(), vertices[i].getY(), vertices[i].getZ());
		}
	}
	
	/**
	 * Draw a triangle with one color in reverse order compared to the one given by vertice
	 * @param gl current OpenGL pipeline
	 * @param vertices vertices of the triangle (size 3)
	 * @param color color to apply
	 */
	protected void drawBackTriangle(GL gl, Vector3D[] vertices, double[] color) {
		gl.glColor3d(color[0], color[1], color[2]);
		
		for (int i = TRIANGLE_NB_EDGE - 1; i >= 0; i--) {
			gl.glVertex3d(vertices[i].getX(), vertices[i].getY(), vertices[i].getZ());
		}
	}
		
	/**
	 * Draw a facet
	 * @param vertices vertices of the facet (3 or 4).
	 * @param colors array containing the colors for the vertices.
	 * @param gl current OpenGL pipeline
	 */
	public void drawFacet(GL gl, Vector3D[] vertices, int[] colors) {
			
		// draw one facet with only one color
		
		// if the color is 0, it means that the facet should not be drawn
		// also check that it is at least a facet
		if (colors[0] <= 0 || vertices.length < TRIANGLE_NB_EDGE) {
			return;
		}
		
		double[] frontColor = getColorMap().getColor(getColorMap().convertScilabToColorMapIndex(colors[0]));
		
		// for now draw only polygons with 3 or 4 edges
		if (vertices.length == TRIANGLE_NB_EDGE) {
			// only a triangle
			paintOneTriangle(vertices, frontColor, gl);
			
		} else if (vertices.length == QUAD_NB_EDGE) {
			
			// its a quad, decompose it into 2 triangles
			Vector3D[] triangle1 =  new Vector3D[TRIANGLE_NB_EDGE];
			Vector3D[] triangle2 =  new Vector3D[TRIANGLE_NB_EDGE];
			GeomAlgos.decomposeQuad(vertices, triangle1, triangle2);
			
			// draw the two decomposed triangles
			paintOneTriangle(triangle1, frontColor, gl);
			paintOneTriangle(triangle2, frontColor, gl);

		} else if (vertices.length > QUAD_NB_EDGE) {
			// draw the facet using several triangles
			// use this method for now. It will work for flat facets
			// but is not optimal for non flat ones (in comparison with the decomposeQuad function)
			int nbTriangles = vertices.length - 2;
			for (int i = 1; i <= nbTriangles; i++) {
				Vector3D[] triangle = {vertices[0], vertices[i], vertices[i + 1]};

				paintOneTriangle(triangle, frontColor, gl);
			}
		}
		

	}
	
	/**
	 * Paint a single triangle
	 * @param vertices poisition of the triangle three vertices
	 * @param triangleColor array containing the 3 channels of the front color
	 * @param gl current OpenGL pipeline
	 */
	private void paintOneTriangle(Vector3D[] vertices, double[] triangleColor, GL gl) {
		// draw front triangle
		drawFrontTriangle(gl, vertices, triangleColor);
		
		// draw back triangle if needed
		if (isDisplayingHiddenSurface()) {
			drawBackTriangle(gl, vertices, getHiddenColor());
		}
	}

}

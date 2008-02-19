/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * desc : Class used to draw facets from their positions and colors  
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
import org.scilab.modules.renderer.utils.geom3D.Vector3D;

/**
 * Class used to draw facets from their positions and colors
 * @author Jean-Baptiste Silvy
 */
public abstract class FacetDrawerGL {

	private static final int TRIANGLE_NB_FACETS = 3;
	private static final int QUAD_NB_FACETS = 4;
	
	private int hiddenColor;
	private int nbVertices;
	
	private TexturedColorMap colorMap;
	
	/**
	 * Default constructor
	 * @param colorMap colorMap to use
	 */
	public FacetDrawerGL(TexturedColorMap colorMap) {
		hiddenColor = -1;
		this.colorMap = colorMap;
	}
	
	/**
	 * Create a new instance of facetDrawerGL
	 * @param colorFlag specify the kinf of facetdrawert to create
	 * @param colorMap colormap to use
	 * @return new instance of facetDrawerGL
	 */
	public static FacetDrawerGL create(int colorFlag, TexturedColorMap colorMap) {
		if (colorFlag == FacetColorComputer.INTERPOLATED_SHADING) {
			return new LinearShadedFacetDrawerGL(colorMap);
		} else {
			return new FlatShadedFacetDrawer(colorMap);
		}
	}

	/**
	 * @return color array containin gthe 3 channels of the color
	 */
	public double[] getHiddenColor() {
		if (hiddenColor >= 0 && hiddenColor < colorMap.getSize()) {
			return colorMap.getColor(hiddenColor);
		} else {
			return null;
		}
	}

	/**
	 * Set the hidden color
	 * @param hiddenColor color index
	 */
	public void setHiddenColor(int hiddenColor) {
		this.hiddenColor = hiddenColor;
	}
	
	/**
	 * To be called when drawing ends.
	 * @param gl current OpenGL pipeline
	 */
	public void endDrawing(GL gl) {
		gl.glEnd();
		gl.glDisable(GL.GL_CULL_FACE);
		gl.glDisable(GL.GL_POLYGON_OFFSET_FILL);
	}

	/**
	 * To be called before any drawn action.
	 * @param gl current OpenGL pipeline
	 */
	public void initializeDrawing(GL gl) {
		if (getHiddenColor() != null) {
			// we must draw one face with computed color
			// and one face with hidden color
			gl.glEnable(GL.GL_CULL_FACE);
			gl.glCullFace(GL.GL_BACK);
		}
		
		// enable polygon offset because we need to draw lines on polygons
		// and consequently they are drawn with the same Z.
		gl.glEnable(GL.GL_POLYGON_OFFSET_FILL);
		gl.glPolygonOffset(1.0f, 1.0f);
		
		if (getNbVertices() == TRIANGLE_NB_FACETS) {
			// triangle
			gl.glBegin(GL.GL_TRIANGLES);
		} else if (getNbVertices() == QUAD_NB_FACETS) {
			// quad
			gl.glBegin(GL.GL_QUADS);
		}
	}

	/**
	 * @return get the number of vertices per facet
	 */
	public int getNbVertices() {
		return nbVertices;
	}

	/**
	 * Set the nuymber of vertices per facet
	 * @param nbVertices number of vertices
	 */
	public void setNbVertices(int nbVertices) {
		this.nbVertices = nbVertices;
	}
	
	
	/**
	 * Draw a facet
	 * @param vertices vertices of the facet (3 or 4).
	 * @param colors array containing the colors for the vertices.
	 * @param gl current OpenGL pipeline
	 */
	public abstract void drawFacet(GL gl, Vector3D[] vertices, int[] colors);

	/**
	 * @return colormap used to draw the facets
	 */
	public TexturedColorMap getColorMap() {
		return colorMap;
	}
	
}

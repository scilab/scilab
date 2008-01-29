/*------------------------------------------------------------------------*/
/* file: FacetDrawerGL.java                                               */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class used to draw facets from their positions and colors       */
/*------------------------------------------------------------------------*/


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
		return colorMap.getColor(hiddenColor);
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

/*------------------------------------------------------------------------*/
/* file: FlatShededFacetDrawerGL.java                                     */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class used to draw uniformy colored facets                      */
/*------------------------------------------------------------------------*/


package org.scilab.modules.renderer.surfaceDrawing;

import javax.media.opengl.GL;

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
		
		
		// draw one facet with only one color
		double[] color = getColorMap().getColor(colors[0]);
		gl.glColor3d(color[0], color[1], color[2]);
		
		for (int i = getNbVertices() - 1; i >= 0; i--) {
			gl.glVertex3d(vertices[i].getX(), vertices[i].getY(), vertices[i].getZ());
		}
		
		if (getHiddenColor() != null) {
			// draw hidden color
			double[] hiddenColor = getHiddenColor();
			gl.glColor3d(hiddenColor[0], hiddenColor[1], hiddenColor[2]);
			
			// draw on reverse since back face culling is enable
			for (int i = 0; i < getNbVertices(); i++) {
				gl.glVertex3d(vertices[i].getX(), vertices[i].getY(), vertices[i].getZ());
			}
		}

	}

}

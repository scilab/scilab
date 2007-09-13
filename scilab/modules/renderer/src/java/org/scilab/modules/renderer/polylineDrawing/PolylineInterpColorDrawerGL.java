/*------------------------------------------------------------------------*/
/* file: PolylineInterpColorDrawerGL.java                                 */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependant routines to fill the area */
/*        enclosed by the polyline with interpolated colors               */
/*------------------------------------------------------------------------*/


package org.scilab.modules.renderer.polylineDrawing;

import javax.media.opengl.GL;

import org.scilab.modules.renderer.AutoDrawableObjectGL;

import com.sun.opengl.util.texture.Texture;

/**
 * Class containing the driver dependant routines to fill the area
 * enclosed by the polyline with interpolated colors.
 * Note that this class might be more generic by using a glu tesselator like
 * in fill drawing. 
 * @author Jean-Baptiste Silvy
 */
public class PolylineInterpColorDrawerGL extends AutoDrawableObjectGL {
	
	/** Move a little the indice in order to get the right color in the colormap */
	private static final double COLOR_OFFSET = 0.5;
	
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
		double size = getColorMap().getSize();
		
		Texture colormapTexture = getColorMap().getTexture();
		colormapTexture.enable();
		colormapTexture.bind();
		
		gl.glColor3d(1.0, 1.0, 1.0);
		gl.glBegin(GL.GL_TRIANGLE_FAN); // works with triangles and quads
		for (int i = 0; i < xCoords.length; i++) {
			gl.glTexCoord1d((colors[i] - COLOR_OFFSET) / size);
			gl.glVertex3d(xCoords[i], yCoords[i], zCoords[i]);
		}
		gl.glEnd();
		colormapTexture.disable();
			
	}
	
	
}

/*------------------------------------------------------------------------*/
/* file: ArcLineDrawerGL.java                                             */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependant routines to fill the area */
/*        enclosed by the polyline                                        */
/*------------------------------------------------------------------------*/

package org.scilab.modules.renderer.polylineDrawing;

import javax.media.opengl.GL;
import javax.media.opengl.glu.GLU;
import javax.media.opengl.glu.GLUtessellator;

import org.scilab.modules.renderer.drawers.FillDrawerGL;


/**
 * Class containing the driver dependant routines to fill the area
 *        enclosed by the polyline 
 * @author Jean-Baptiste Silvy
 */
public class PolylineFillDrawerGL extends FillDrawerGL implements PolylineDrawerStrategy {

	
	/**
	 * Default constructor.
	 */
	public PolylineFillDrawerGL() {
		super();
	}
	
	/**
	 * Create the display list for the polyline
	 * @param xCoords X coordinates of the polylines vertices
	 * @param yCoords Y coordinates of the polylines vertices
	 * @param zCoords Z coordinates of the polylines vertices
	 */
	public void drawPolyline(double[] xCoords, double[] yCoords, double[] zCoords) {
		
		// This function can draw arbitrary polygons, so we use Glu tesselation
		// However, it seems there is some bugs with the tesselation.
		GLU glu = getGlu();
		GL gl = getGL();
		
		// set color
		double[] color = getBackColor();
		gl.glColor3d(color[0], color[1], color[2]);
		
		PolylineTesselatorCallback callbacks = new PolylineTesselatorCallback();
		callbacks.setCurrentGL(gl, glu);
		
		GLUtessellator tess = glu.gluNewTess();
		
		// set the callback properties.
		glu.gluTessCallback(tess, GLU.GLU_TESS_VERTEX, callbacks);
		glu.gluTessCallback(tess, GLU.GLU_TESS_BEGIN, callbacks);
		glu.gluTessCallback(tess, GLU.GLU_TESS_END, callbacks);
		glu.gluTessCallback(tess, GLU.GLU_TESS_COMBINE, callbacks);
		glu.gluTessCallback(tess, GLU.GLU_TESS_ERROR, callbacks);
		
		// set the rendering prperties
		// specify that all the enclosed curve is displayed
		glu.gluTessProperty(tess, GLU.GLU_TESS_WINDING_RULE, GLU.GLU_TESS_WINDING_NONZERO);
		
		// draw the polygon
		glu.gluTessBeginPolygon(tess, null);
		glu.gluTessBeginContour(tess);
		for (int i = 0; i < xCoords.length; i++) {
			double[] coords = {xCoords[i], yCoords[i], zCoords[i]};
			glu.gluTessVertex(tess, coords, 0, coords);
		}
		glu.gluTessEndContour(tess);
		glu.gluEndPolygon(tess);
		
		glu.gluDeleteTess(tess);
		
	}

	
	
}

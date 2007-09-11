/*------------------------------------------------------------------------*/
/* file: PolylineLineDrawerGL.java                                        */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependant routines to draw the      */
/*        main line of a polyline                                         */
/*------------------------------------------------------------------------*/


package org.scilab.modules.renderer.polylineDrawing;

import javax.media.opengl.GL;

import org.scilab.modules.renderer.drawers.LineDrawerGL;
import org.scilab.modules.renderer.utils.glTools.GLTools;

/**
 * Class containing the driver dependant routines to draw the
 *        main line of a polyline 
 * @author Jean-Baptiste Silvy
 */
public class PolylineLineDrawerGL extends LineDrawerGL implements PolylineDrawerStrategy {

	/**
	 * Default constructor
	 */
	public PolylineLineDrawerGL() {
		super();
	}
	
	/**
	 * Create the display list for the polyline
	 * @param xCoords X coordinates of the polylines vertices
	 * @param yCoords Y coordinates of the polylines vertices
	 * @param zCoords Z coordinates of the polylines vertices
	 */
	public void drawPolyline(double[] xCoords, double[] yCoords, double[] zCoords) {
		
		GL gl = getGL();
		
		// set dash mode
		gl.glLineWidth(getThickness());
		GLTools.beginDashMode(gl, getLineStyle(), getThickness());
		
		// set color
		double[] color = getLineColor();
		gl.glColor3d(color[0], color[1], color[2]);
		
		
		gl.glBegin(GL.GL_LINE_STRIP);
		// the three arrays should have the same size
		for (int i = 0; i < xCoords.length; i++) {
			gl.glVertex3d(xCoords[i], yCoords[i], zCoords[i]);
		}
		gl.glEnd();
		
		GLTools.endDashMode(gl);
		
	}
	
}

/*------------------------------------------------------------------------*/
/* file: SegsLineDrawerGL.java                                            */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class drawing the line part of arrows                           */
/*------------------------------------------------------------------------*/


package org.scilab.modules.renderer.segsDrawing;

import javax.media.opengl.GL;

import org.scilab.modules.renderer.drawers.LineDrawerGL;
import org.scilab.modules.renderer.utils.glTools.GLTools;

/**
 * Class drawing the line part of arrows
 * @author Jean-Baptiste Silvy
 */
public class SegsLineDrawerGL extends LineDrawerGL {

	
	/**
	 * Default constructor
	 */
	public SegsLineDrawerGL() {
		super();
	}
	
	/**
	 * Set all line parameters at once, to avoid multiple Jni calls
	 * @param thickness thickness of the line in pixels
	 * @param lineStyle index of the line Style
	 */
	public void setLineParameters(float thickness, int lineStyle) {
		// colors is specified for each segment
		super.setLineParameters(0, thickness, lineStyle);
	}
	
	/**
	 * Draw a set of segment knowing their positions and colors
	 * @param startXCoords X coordinate of segments first point
	 * @param endXCoords X coordinate of segments end point
	 * @param startYCoords Y coordinate of segments first point
	 * @param endYCoords Y coordinate of segments end point
	 * @param startZCoords Z coordinate of segments first point
	 * @param endZCoords Z coordinate of segments end point
	 * @param colors array containing the color of each segment
	 */
	public void drawSegs(double[] startXCoords, double[] endXCoords, double[] startYCoords, double[] endYCoords,
						 double[] startZCoords, double[] endZCoords, int[] colors) {
		int nbSegs = startXCoords.length;
		GL gl = getGL();
		
		gl.glLineWidth(getThickness());
		GLTools.beginDashMode(gl, getLineStyle(), getThickness());
		
		gl.glBegin(GL.GL_LINES);
		
		for (int i = 0; i < nbSegs; i++) {
			int sciColorIndex = getColorMap().convertScilabToColorMapIndex(colors[i]);
			double[] curColor = getColorMap().getColor(sciColorIndex);
			gl.glColor3d(curColor[0], curColor[1], curColor[2]);
			
			gl.glVertex3d(startXCoords[i], startYCoords[i], startZCoords[i]);
			gl.glVertex3d(endXCoords[i], endYCoords[i], endZCoords[i]);
		}
		
		gl.glEnd();
		
		GLTools.endDashMode(gl);
		
	}
	
}

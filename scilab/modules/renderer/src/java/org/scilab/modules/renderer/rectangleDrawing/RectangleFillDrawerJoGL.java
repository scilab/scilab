/*------------------------------------------------------------------------*/
/* file: RectangleFillDrawerJoGL.java                                     */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependant routines to draw the      */
/*        inside of a rectangle object                                   */
/*------------------------------------------------------------------------*/


package org.scilab.modules.renderer.rectangleDrawing;

import org.scilab.modules.renderer.AutoDrawableObjectJoGL;
import javax.media.opengl.GL;

/**
 * Class containing functions called by RectangleFillDrawerJoGL.cpp
 * @author Jean-Baptiste Silvy
 */
public class RectangleFillDrawerJoGL extends AutoDrawableObjectJoGL {
	
	/** index of background color */
	private int backColor;
	
	/**
	 * Default Constructor
	 */
	public RectangleFillDrawerJoGL() {
		backColor = -1;
	}
	
	/**
	 * Set rectangle background color
	 * @param color index of background color
	 */
	public void setBackColor(int color) {
		backColor = color;
	}
	
	/**
	 * create the display list for the rectangle
	 * @param corner1X first corner X coordinate
	 * @param corner1Y first corner Y coordinate
	 * @param corner1Z first corner Z coordinate
	 * @param corner2X second corner X coordinate
	 * @param corner2Y second corner Y coordinate
	 * @param corner2Z second corner Z coordinate
	 * @param corner3X third corner X coordinate
	 * @param corner3Y third corner Y coordinate
	 * @param corner3Z third corner Z coordinate
	 * @param corner4X last corner X coordinate
	 * @param corner4Y last corner Y coordinate
	 * @param corner4Z last corner Z coordinate
	 */
	public void drawRectangle(double corner1X, double corner1Y, double corner1Z,
							  double corner2X, double corner2Y, double corner2Z,
							  double corner3X, double corner3Y, double corner3Z,
							  double corner4X, double corner4Y, double corner4Z) {
		GL gl = getGL();
		
		// set color
		double[] color = getColorMap().getColor(backColor);
		gl.glColor3d(color[0], color[1], color[2]);
		
		gl.glBegin(GL.GL_QUADS);
		gl.glVertex3d(corner1X, corner1Y, corner1Z);
		gl.glVertex3d(corner2X, corner2Y, corner2Z);
		gl.glVertex3d(corner3X, corner3Y, corner3Z);
		gl.glVertex3d(corner4X, corner4Y, corner4Z);
		gl.glEnd();
		
	}
}

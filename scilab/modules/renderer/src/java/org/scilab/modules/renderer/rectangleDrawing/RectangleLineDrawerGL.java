/*------------------------------------------------------------------------*/
/* file: RectangleLineDrawerJoGL.java                                     */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependant routines to draw the      */
/*        outline of a rectangle object                                   */
/*------------------------------------------------------------------------*/

package org.scilab.modules.renderer.rectangleDrawing;

import org.scilab.modules.renderer.AutoDrawableObjectGL;
import javax.media.opengl.GL;

import org.scilab.modules.renderer.utils.glTools.GLTools;

/**
 * Class containing functions called by RectangleLineDrawerJoGL.cpp
 * @author Jean-Baptiste Silvy
 */
public class RectangleLineDrawerGL extends AutoDrawableObjectGL {

	private int   lineColor;
	private float thickness;
	private int   lineStyle;
	
	/**
	 * Default constructor
	 */
	public RectangleLineDrawerGL() {
		super();
		lineColor = 1;
		thickness = 1.0f;
		lineStyle = 1;
	}
	
	/**
	 * Set line Color
	 * @param lineColor index of the line color in the colormap
	 */
	public void setLineColor(int lineColor) {
		this.lineColor = lineColor;
	}
	
	/**
	 * Set the thickness
	 * @param thickness thickness of the line in pixels
	 */
	public void setThickness(float thickness) {
		this.thickness = thickness;
	}
	
	/**
	 * Set the line style
	 * @param lineStyle index of the line Style
	 */
	public void setLineStyle(int lineStyle) {
		this.lineStyle = lineStyle;
	}
	
	/**
	 * Set all line parameters at once, to avoid multiple Jni calls
	 * @param lineColor index of the line color in the colormap
	 * @param thickness thickness of the line in pixels
	 * @param lineStyle index of the line Style
	 */
	public void setLineParameters(int lineColor, float thickness, int lineStyle) {
		setLineColor(lineColor);
		setThickness(thickness);
		setLineStyle(lineStyle);
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
		
		// set dash mode
		gl.glLineWidth(thickness);
		GLTools.beginDashMode(gl, lineStyle, thickness);
		
		// set color
		double[] color = getColorMap().getColor(lineColor);
		gl.glColor3d(color[0], color[1], color[2]);
		
		
		gl.glBegin(GL.GL_LINE_LOOP);
		gl.glVertex3d(corner1X, corner1Y, corner1Z);
		gl.glVertex3d(corner2X, corner2Y, corner2Z);
		gl.glVertex3d(corner3X, corner3Y, corner3Z);
		gl.glVertex3d(corner4X, corner4Y, corner4Z);
		gl.glEnd();
		
		GLTools.endDashMode(gl);
		
	}
	
}

/*------------------------------------------------------------------------*/
/* file: DrawableRectangleJoGL.java                                       */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependant routines to draw a        */
/*        subwin object with JoGL                                         */
/*------------------------------------------------------------------------*/

package org.scilab.modules.renderer.subwinDrawing;

import javax.media.opengl.GL;

import org.scilab.modules.renderer.AutoDrawableObjectGL;

/**
 * Class containing functions called by DrawableSubwinJoGL.cpp
 * @author Jean-Baptiste Silvy
 */
public class DrawableSubwinGL extends AutoDrawableObjectGL {

	/**
	 * Default Constructor
	 */
	public DrawableSubwinGL() {
		super();
	}
	
	/**
	 * Draw the bounding box of the subwin object.
	 * @param xMin lowest bound on X axis
	 * @param xMax highest bound on X axis
	 * @param yMin lowest bound on Y axis
	 * @param yMax highest bound on Y axis
	 * @param zMin lowest bound on Z axis
	 * @param zMax highest bound on Z axis
	 */
	public void drawBox(double xMin, double xMax, double yMin, double yMax, double zMin, double zMax) {
		GL gl = getGL();
		
		gl.glColor3d(0.0, 0.0, 0.0);
		gl.glBegin(GL.GL_LINE_STRIP);
		gl.glVertex3d(xMax, yMin, zMin);
		gl.glVertex3d(xMax, yMin, zMax);
		gl.glVertex3d(xMin, yMin, zMax);
		gl.glVertex3d(xMin, yMin, zMin);
		gl.glVertex3d(xMax, yMin, zMin);
		gl.glVertex3d(xMax, yMax, zMin);
		gl.glVertex3d(xMax, yMax, zMax);
		gl.glVertex3d(xMin, yMax, zMax);
		gl.glVertex3d(xMin, yMax, zMin);
		gl.glVertex3d(xMax, yMax, zMin);
		gl.glEnd();
		
		gl.glBegin(GL.GL_LINES);
		gl.glVertex3d(xMin, yMin, zMin);
		gl.glVertex3d(xMin, yMax, zMin);
		gl.glVertex3d(xMin, yMin, zMax);
		gl.glVertex3d(xMin, yMax, zMax);
		gl.glVertex3d(xMax, yMin, zMax);
		gl.glVertex3d(xMax, yMax, zMax);
		gl.glEnd();
	}

	
}

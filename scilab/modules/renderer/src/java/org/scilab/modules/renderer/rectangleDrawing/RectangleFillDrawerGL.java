/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class containing the driver dependant routines to draw the
 * inside of a rectangle object  
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


package org.scilab.modules.renderer.rectangleDrawing;

import org.scilab.modules.renderer.drawers.FillDrawerGL;
import org.scilab.modules.renderer.utils.CoordinateTransformation;

import javax.media.opengl.GL;

/**
 * Class containing functions called by RectangleFillDrawerJoGL.cpp
 * @author Jean-Baptiste Silvy
 */
public class RectangleFillDrawerGL extends FillDrawerGL implements RectangleDrawerStrategy {
	
	private boolean isZconstant;
	
	/**
	 * Default Constructor
	 */
	public RectangleFillDrawerGL() {
		super();
		isZconstant = false;
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
		
		isZconstant =    corner1Z == corner2Z
		              && corner1Z == corner3Z
		              && corner1Z == corner4Z;
		
		// set color
		double[] color = getBackColor();
		gl.glColor3d(color[0], color[1], color[2]);
		
		// push polygons back if needed
		CoordinateTransformation transform = getParentFigureGL().getCoordinateTransformation();
		transform.pushPolygonsBack(gl, this);
		
		gl.glBegin(GL.GL_QUADS);
		gl.glVertex3d(corner1X, corner1Y, corner1Z);
		gl.glVertex3d(corner2X, corner2Y, corner2Z);
		gl.glVertex3d(corner3X, corner3Y, corner3Z);
		gl.glVertex3d(corner4X, corner4Y, corner4Z);
		gl.glEnd();
		
		transform.endPushPolygonsBack(gl, this);
		
	}
	
	/**
	 * This method is used to know if polygon offset is needed in 2d mode.
	 * If this function returns true, then the polygon offset is not needed in 2d mode.
	 * If it returns false, polygon offset is always needed.
	 * @return true if the object is flat along Z coordinate
	 */
	public boolean isZConstant() {
		return isZconstant;
	}
}

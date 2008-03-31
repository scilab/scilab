/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class containing the driver dependant routines to draw the
 * outline of a rectangle object  
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.renderer.rectangleDrawing;

import java.util.Vector;

import javax.media.opengl.GL;

import org.scilab.modules.renderer.drawers.LineDrawerGL;
import org.scilab.modules.renderer.utils.geom3D.Vector3D;
import org.scilab.modules.renderer.utils.glTools.GLTools;

/**
 * Class containing functions called by RectangleLineDrawerJoGL.cpp
 * @author Jean-Baptiste Silvy
 */
public class RectangleLineDrawerGL extends LineDrawerGL implements RectangleDrawerStrategy {
	
	/**
	 * Default constructor
	 */
	public RectangleLineDrawerGL() {
		super();
	}
	
	
	/**
	 * Draw the rectangle
	 * @param gl current GL pipeline
	 * @param corner1 first corner coordinates
	 * @param corner2 second corner coordinates
	 * @param corner3 third corner coordinates
	 * @param corner4 fourth corner coordinates
	 */
	public void drawRectangle(GL gl, Vector3D corner1, Vector3D corner2, Vector3D corner3, Vector3D corner4) {
		// set dash mode
		GLTools.beginDashMode(gl, getLineStyle(), getThickness());
		
		// set color
		double[] color = getLineColor();
		gl.glColor3d(color[0], color[1], color[2]);
		
		
		gl.glBegin(GL.GL_LINE_LOOP);
		gl.glVertex3d(corner1.getX(), corner1.getY(), corner1.getZ());
		gl.glVertex3d(corner2.getX(), corner2.getY(), corner2.getZ());
		gl.glVertex3d(corner3.getX(), corner3.getY(), corner3.getZ());
		gl.glVertex3d(corner4.getX(), corner4.getY(), corner4.getZ());
		gl.glEnd();
		
		GLTools.endDashMode(gl);
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
		drawRectangle(getGL(),
				      new Vector3D(corner1X, corner1Y, corner1Z),
				      new Vector3D(corner2X, corner2Y, corner2Z),
				      new Vector3D(corner3X, corner3Y, corner3Z),
				      new Vector3D(corner4X, corner4Y, corner4Z));
		
	}
	
}

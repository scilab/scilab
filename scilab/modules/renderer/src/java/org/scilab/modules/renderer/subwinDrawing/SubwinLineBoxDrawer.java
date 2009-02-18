/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * desc : Parent class for drawing different kind of axes box 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.renderer.subwinDrawing;

import javax.media.opengl.GL;

import org.scilab.modules.renderer.utils.geom3D.Vector3D;
import org.scilab.modules.renderer.utils.glTools.GLTools;


/**
 * Box drawer dedicated at drawing box lines
 * @author Jean-Baptiste Silvy
 */
public abstract class SubwinLineBoxDrawer extends SubwinBoxDrawer {

	private int lineColor;
	private int lineStyle;
	private float lineThickness;
	private int hiddenAxisColor;
	
	/**
	 * Default constructor
	 */
	public SubwinLineBoxDrawer() {
		super();
		hiddenAxisColor = 0;
		lineColor = 0;
		lineStyle = 0;
		lineThickness = 0.0f;
	}
	
	/**
	 * Set color of the subwin box lines, excluding the back trihedron.
	 * @param colorIndex colormap index of the color.
	 */
	public void setLineColor(int colorIndex) {
		this.lineColor = colorIndex;
	}
	
	/**
	 * @return Array of size 3 containing the 3 channels of the line color.
	 */
	public double[] getLineColor() {
		return getColorMap().getColor(lineColor);
	}
	
	/**
	 * Set the kind of line (pattern) to use for drawing the box.
	 * @param lineStyle index of the line style
	 */
	public void setLineStyle(int lineStyle) {
		this.lineStyle = lineStyle;
	}
	
	/**
	 * @return index of line style
	 */
	public int getLineStyle() {
		return lineStyle;
	}
	
	/**
	 * Set the tickness of the lines composing the box.
	 * @param thickness thickness in pixels
	 */
	public void setThickness(float thickness) {
		this.lineThickness = thickness;
	}
	
	/**
	 * @return thickness of box lines
	 */
	public float getThickness() {
		return lineThickness;
	}
	
	/**
	 * Set color of the back trihedron lines
	 * @param colorIndex colormap index of the color.
	 */
	public void setHiddenAxisColor(int colorIndex) {
		this.hiddenAxisColor = colorIndex;
	}
	
	/**
	 * @return Array of size 3 containing the 3 channels of the hidden axis color.
	 */
	public double[] getHiddenAxisColor() {
		return getColorMap().getColor(hiddenAxisColor);
	}
	
	/**
	 * Set all parameters with a single function
	 * @param hiddenAxisColor color of back trihedron lines
	 * @param lineColor color of the other lines of the box
	 * @param lineStyle pattern to use for lines
	 * @param thickness thickness of the lines
	 */
	public void setBoxParameters(int hiddenAxisColor, int lineColor,
								 int lineStyle, float thickness) {
		setHiddenAxisColor(hiddenAxisColor);
		setLineColor(lineColor);
		setLineStyle(lineStyle);
		setThickness(thickness);
	}
	
	/**
	 * Draw the lines between the back and front coordinates
	 * @param concealedCornerIndex index of the concealed corner
	 */
	protected void drawHalfBox(int concealedCornerIndex) {
		GL gl = getGL();
		
		
		GLTools.beginDashMode(gl, getLineStyle(), getThickness());
		
		double[] color = getLineColor();
		gl.glColor3d(color[0], color[1], color[2]);
		
		gl.glBegin(GL.GL_LINE_LOOP);
		for (int i = 0; i < HALF_BOX_LINES[concealedCornerIndex].length; i++) {
			Vector3D curVertex = getBoxCorner(HALF_BOX_LINES[concealedCornerIndex][i]);
			gl.glVertex3d(curVertex.getX(), curVertex.getY(), curVertex.getZ());
		}
		gl.glEnd();
		GLTools.endDashMode(gl);
	}
	
	
	
	/**
	 * Draw the front thrihedron of the box.
	 * @param concealedCornerIndex index of the concealed corner
	 */
	protected void drawFrontTrihedron(int concealedCornerIndex) {
		GL gl = getGL();
		
		// set dash mode
		GLTools.beginDashMode(gl, getLineStyle(), getThickness());
		
		double[] color = getLineColor();
		gl.glColor3d(color[0], color[1], color[2]);
		
		gl.glBegin(GL.GL_LINES);
		for (int i = 0; i < FRONT_TRIHEDRON[concealedCornerIndex].length; i++) {
			for (int j = 0; j < FRONT_TRIHEDRON[concealedCornerIndex][i].length; j++) {
				Vector3D curVertex = getBoxCorner(FRONT_TRIHEDRON[concealedCornerIndex][i][j]);
				gl.glVertex3d(curVertex.getX(), curVertex.getY(), curVertex.getZ());
			}
		}
		gl.glEnd();
		GLTools.endDashMode(gl);
	}
	
	/**
	 * Draw the back triedre of the subwin box
	 * @param concealedCornerIndex index of the concealed corner
	 */
	protected void drawTrihedron(int concealedCornerIndex) {
		
		if (getCoordinateTransformation().is2dMode()) {
			// don't display back trihedron in 2d mode, it may give some bad results
			return;
		}
		
		GL gl = getGL();
		
		// the concealed line is draw wiht dashes
		GLTools.beginDashMode(gl, 2, getThickness());
		
		double[] hiddenColor = getHiddenAxisColor();
		gl.glColor3d(hiddenColor[0], hiddenColor[1], hiddenColor[2]);
		
		gl.glBegin(GL.GL_LINES);
		for (int i = 0; i < BACK_TRIHEDRON[concealedCornerIndex].length; i++) {
			for (int j = 0; j < BACK_TRIHEDRON[concealedCornerIndex][i].length; j++) {
				Vector3D curVertex = getBoxCorner(BACK_TRIHEDRON[concealedCornerIndex][i][j]);
				gl.glVertex3d(curVertex.getX(), curVertex.getY(), curVertex.getZ());
			}
		}
		gl.glEnd();
		
		GLTools.endDashMode(gl);
		
		
	}

}

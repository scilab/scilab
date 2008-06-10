/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
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

import org.scilab.modules.renderer.DrawableObjectGL;
import org.scilab.modules.renderer.utils.CoordinateTransformation;
import org.scilab.modules.renderer.utils.geom3D.Vector3D;
import org.scilab.modules.renderer.utils.glTools.GLTools;

/**
 * Parent class for drawing different kind of axes box
 * @author Jean-Baptiste Silvy
 */
public abstract class SubwinBoxDrawer extends DrawableObjectGL {

	/** Number of corners of the box */
	protected static final int BOX_NB_CORNERS = 8;
	
	/** For each corners, lines to draw for the back trihedron */
	private static final int[][][] BACK_TRIHEDRON = {{{0, 1}, {0, 4}, {0, 2}},
													 {{1, 0}, {1, 3}, {1, 5}},
													 {{2, 0}, {2, 3}, {2, 6}},
													 {{3, 1}, {3, 2}, {3, 7}},
													 {{4, 0}, {4, 5}, {4, 6}},
												   	 {{5, 1}, {5, 4}, {5, 7}},
												   	 {{6, 4}, {6, 2}, {6, 7}},
												   	 {{7, 3}, {7, 5}, {7, 6}}};
	
	/** For each corners, corners of the facet to draw as back facet. */
	private static final int[][][] BACK_FACETS = {{{0, 2, 3, 1}, {0, 4, 5, 1}, {0, 4, 6, 2}}, 
												  {{1, 0, 4, 5}, {1, 3, 2, 0}, {1, 5, 7, 3}},
												  {{2, 3, 1, 0}, {2, 6, 7, 3}, {2, 0, 4, 6}},
												  {{3, 7, 6, 2}, {3, 1, 0, 2}, {3, 7, 5, 1}},
												  {{4, 5, 1, 0}, {4, 5, 7, 6}, {4, 6, 2, 0}},
												  {{5, 1, 0, 4}, {5, 7, 6, 4}, {5, 7, 3, 1}},
												  {{6, 7, 5, 4}, {6, 2, 3, 7}, {6, 2, 0, 4}},
												  {{7, 5, 4, 6}, {7, 3, 2, 6}, {7, 3, 1, 5}}};
	
	/** For each corners, lines to draw between back trihedron and front one */
	private static final int[][] HALF_BOX_LINES = {{1, 3, 2, 6, 4, 5},
								     			   {0, 2, 3, 7, 5, 4},
												   {3, 1, 0, 4, 6, 7},
												   {2, 0, 1, 5, 7, 6},
												   {5, 1, 0, 2, 6, 7},
												   {4, 0, 1, 3, 7, 6},
												   {7, 3, 2, 0, 4, 5},
												   {6, 2, 3, 1, 5, 4}};
	
	/** For each corners, lines to draw for the front trihedron */
	private static final int[][][] FRONT_TRIHEDRON = {{{7, 3}, {7, 5}, {7, 6}},
		                                              {{6, 4}, {6, 2}, {6, 7}},
		                                              {{5, 1}, {5, 4}, {5, 7}},
		                                              {{4, 0}, {4, 5}, {4, 6}},
													  {{3, 1}, {3, 2}, {3, 7}},
													  {{2, 0}, {2, 3}, {2, 6}},
												      {{1, 0}, {1, 3}, {1, 5}},
												   	  {{0, 1}, {0, 4}, {0, 2}}};
	
	private int hiddenAxisColor;
	private int backgroundColor;
	private int lineColor;
	private int lineStyle;
	private float lineThickness;
	
	private Vector3D[] boxCorners;
	
	/**
	 * Default constructor
	 */
	public SubwinBoxDrawer() {
		super();
		hiddenAxisColor = 0;
		backgroundColor = 0;
		lineColor = 0;
		lineStyle = 0;
		lineThickness = 0.0f;
		boxCorners = null;
	}
	
	/**
	 * Display the object using already given parameters.
	 * Display lists are not usable since hidden axis might change at every redraw.
	 * @param parentFigureIndex index of the parent figure in which the object will be drawn
	 */
	public void show(int parentFigureIndex) {
		initializeDrawing(parentFigureIndex);
		drawBox();
		endDrawing();
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
	 * Set the background color of the subwindow (facets color of the back trihedron).
	 * @param colorIndex colormap index of the color.
	 */
	public void setBackgroundColor(int colorIndex) {
		this.backgroundColor = colorIndex;
	}
	
	/**
	 * @return Array of size 3 containing the 3 channels of the background color.
	 */
	public double[] getBackgroundColor() {
		return getColorMap().getColor(backgroundColor);
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
	 * Set all parameters with a single function
	 * @param hiddenAxisColor color of back trihedron lines
	 * @param backgroundColor color of back trihedron facets
	 * @param lineColor color of the other lines of the box
	 * @param lineStyle pattern to use for lines
	 * @param thickness thickness of the lines
	 */
	public void setBoxParameters(int hiddenAxisColor, int backgroundColor, int lineColor,
								 int lineStyle, float thickness) {
		setHiddenAxisColor(hiddenAxisColor);
		setBackgroundColor(backgroundColor);
		setLineColor(lineColor);
		setLineStyle(lineStyle);
		setThickness(thickness);
	}
	
	/**
	 * Draw the box specified by its bounds.
	 * @param xMin minimun bounds on X axis.
	 * @param xMax maximum bounds on X axis.
	 * @param yMin minimun bounds on Y axis.
	 * @param yMax maximum bounds on Y axis.
	 * @param zMin minimun bounds on Z axis.
	 * @param zMax maximum bounds on Z axis.
	 */
	public void drawBox(double xMin, double xMax, double yMin, double yMax, double zMin, double zMax) {
		Vector3D[] newBoxCorners = {new Vector3D(xMin, yMin, zMin),
					   				new Vector3D(xMin, yMin, zMax),
					   				new Vector3D(xMin, yMax, zMin),
									new Vector3D(xMin, yMax, zMax),
									new Vector3D(xMax, yMin, zMin),
									new Vector3D(xMax, yMin, zMax),
									new Vector3D(xMax, yMax, zMin),
									new Vector3D(xMax, yMax, zMax)};
		boxCorners = newBoxCorners;
		
		drawBox();
	}
	
	/**
	 * @return index of the coner which is concealed
	 */
	protected int findConcealedCorner() {
		GL gl = getGL();
		CoordinateTransformation transform = CoordinateTransformation.getTransformation(gl);
		
		// get box corners in canvas coordinates
		Vector3D[] canvasCorners = transform.getCanvasCoordinates(gl, boxCorners);
		
		
		
		// find the farthest one
		double eyeDistance = canvasCorners[0].getZ();
		int farthestCornerIndex = 0;
		for (int i = 1; i < BOX_NB_CORNERS; i++) {
			if (canvasCorners[i].getZ() > eyeDistance) {
				eyeDistance = canvasCorners[i].getZ();
				farthestCornerIndex = i;
			}
		}
		return farthestCornerIndex;
		
	}
	/**
	 * Draw the back triedre of the subwin box
	 * @param concealedCornerIndex index of the concealed corner
	 */
	protected void drawTrihedron(int concealedCornerIndex) {
		
		GL gl = getGL();
		
		// draw background
		double[] backColor = getBackgroundColor();
		gl.glColor3d(backColor[0], backColor[1], backColor[2]);
		
		// push back polygons from the box lines
		// just draw them in the back otherwise it will cut objects
		// which are drawn outside axes box
		// depth test disable so no need to push polygons back
		gl.glDisable(GL.GL_DEPTH_TEST);
		
		gl.glBegin(GL.GL_QUADS);
		for (int i = 0; i < BACK_FACETS[concealedCornerIndex].length; i++) {
			for (int j = 0; j < BACK_FACETS[concealedCornerIndex][i].length; j++) {
				Vector3D curVertex = boxCorners[BACK_FACETS[concealedCornerIndex][i][j]];
				gl.glVertex3d(curVertex.getX(), curVertex.getY(), curVertex.getZ());
			}
		}
		gl.glEnd();
		gl.glEnable(GL.GL_DEPTH_TEST);
		
		// the concealed line is draw wiht dashes
		GLTools.beginDashMode(gl, 2, getThickness());
		
		double[] hiddenColor = getHiddenAxisColor();
		gl.glColor3d(hiddenColor[0], hiddenColor[1], hiddenColor[2]);
		
		gl.glBegin(GL.GL_LINES);
		for (int i = 0; i < BACK_TRIHEDRON[concealedCornerIndex].length; i++) {
			for (int j = 0; j < BACK_TRIHEDRON[concealedCornerIndex][i].length; j++) {
				Vector3D curVertex = boxCorners[BACK_TRIHEDRON[concealedCornerIndex][i][j]];
				gl.glVertex3d(curVertex.getX(), curVertex.getY(), curVertex.getZ());
			}
		}
		gl.glEnd();
		
		GLTools.endDashMode(gl);
		
		
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
			Vector3D curVertex = boxCorners[HALF_BOX_LINES[concealedCornerIndex][i]];
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
				Vector3D curVertex = boxCorners[FRONT_TRIHEDRON[concealedCornerIndex][i][j]];
				gl.glVertex3d(curVertex.getX(), curVertex.getY(), curVertex.getZ());
			}
		}
		gl.glEnd();
		GLTools.endDashMode(gl);
	}
	
	
	/**
	 * Draw the box specified by its bounds.
	 */
	protected abstract void drawBox();
	
}

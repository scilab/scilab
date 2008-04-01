/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Parent class for drawing grid 
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
 * Parent class for drawing grid
 * @author Jean-Baptiste Silvy
 */
public abstract class GridDrawerGL extends BoxTrimmingObjectGL {

	private int gridColor;
	private float gridThickness;
	
	private double[] gridPositions;
	
	/**
	 * Default constructor
	 */
	public GridDrawerGL() {
		super();
	}
	
	/**
	 * Should not be called
	 * @param parentFigureIndex index of parent figure
	 */
	public void show(int parentFigureIndex) {
		// should not be called
	}
	
	/**
	 * Specify a new color for displaying the grid
	 * @param gridColor color index in the color map
	 */
	public void setGridColor(int gridColor) {
		this.gridColor = getColorMap().convertScilabToColorMapIndex(gridColor);
	}
	
	/**
	 * @return array of size 3 containing the 3 channels of the color
	 */
	public double[] getGridColor() {
		return getColorMap().getColor(gridColor);
	}
	
	/**
	 * Specify a new thickness for the grid
	 * @param gridThickness thickness in pixels
	 */
	public void setGridThickness(float gridThickness) {
		this.gridThickness = gridThickness;
	}
	
	/**
	 * @return thickness of the grid in pixels
	 */
	public float getGridThickness() {
		return gridThickness;
	}
	
	/**
	 * @param index index of the grid line
	 * @return Position along axis for the index grid line
	 */
	protected double getGridPosition(int index) {
		return gridPositions[index];
	}
	
	/**
	 * @return all positions of the grid
	 */
	protected double[] getGridPositions() {
		return gridPositions;
	}
	
	/**
	 * @return number of positions fro the grid
	 */
	protected int getGridNbPositions() {
		return gridPositions.length;
	}
	
	/**
	 * Set all the constant parameters of the grid
	 * @param gridColor color index of the color
	 * @param gridThickness thickness in pixels
	 */
	public void setGridParameters(int gridColor, float gridThickness) {
		setGridColor(gridColor);
		setGridThickness(gridThickness);
	}
	
	/**
	 * Draw the grid
	 * @param gridPositions starting points of grid along the concerned axis
	 */
	public void drawGrid(double[] gridPositions) {
		this.gridPositions = gridPositions;
		drawGrid();
	}
	
	/**
	 * Draw the grif from computed positions.
	 * Each grid line is actually made of two consecutive lines, each one on one facet.
	 * @param startingPoints positions of the first points of lines
	 * @param middlePoints postions of the points belonging to both lines
	 * @param endPoints positions of the last point of the second line
	 */
	public void drawGrid(Vector3D[] startingPoints, Vector3D[] middlePoints, Vector3D[] endPoints) {
		GL gl = getGL();
		
		GLTools.beginDashMode(gl, 2, getGridThickness());
		
		double[] color = getGridColor();
		gl.glColor3d(color[0], color[1], color[2]);
		
		gl.glBegin(GL.GL_LINES);
		for (int i = 0; i < startingPoints.length; i++) {
			
			// check if point can be drawn
			if (startingPoints[i] == null || middlePoints[i] == null || endPoints[i] == null) {
				continue;
			}
			
			// first part
			gl.glVertex3d(startingPoints[i].getX(), startingPoints[i].getY(), startingPoints[i].getZ());
			gl.glVertex3d(middlePoints[i].getX(), middlePoints[i].getY(), middlePoints[i].getZ());
			
			// second part
			gl.glVertex3d(middlePoints[i].getX(), middlePoints[i].getY(), middlePoints[i].getZ());
			gl.glVertex3d(endPoints[i].getX(), endPoints[i].getY(), endPoints[i].getZ());
			
		}
		gl.glEnd();
		
		GLTools.endDashMode(gl);
		
		
	}
	
	
	
	/**
	 * Draw the grid with already set positions
	 */
	public abstract void drawGrid();

}

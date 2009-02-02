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

import org.scilab.modules.renderer.DrawableObjectGL;
import org.scilab.modules.renderer.utils.CoordinateTransformation;
import org.scilab.modules.renderer.utils.geom3D.Vector3D;
import org.scilab.modules.renderer.utils.glTools.GLTools;

/**
 * Parent class for drawing grid
 * @author Jean-Baptiste Silvy
 */
public class GridDrawerGL extends DrawableObjectGL {

	private int gridColor;
	private float gridThickness;
	private boolean drawFront;
	
	private double[] gridPositions;
	
	private Vector3D[] startingPoints;
	private Vector3D[] middlePoints;
	private Vector3D[] endPoints;
	
	/**
	 * Default constructor
	 */
	public GridDrawerGL() {
		super();
	}
	
	/**
	 * @param parentFigureIndex index of parent figure
	 */
	public void show(int parentFigureIndex) {
		showGrid();
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
	 * Specify whether the grid should be drawn above or behind other objects. 
	 * @param drawFront if true draw above, if false draw behind
	 */
	public void setDrawFront(boolean drawFront) {
		this.drawFront = drawFront;
	}
	
	/**
	 * @return true if the grid is drawn above other objects, false otherwise
	 */
	public boolean isDrawingFront() {
		return drawFront;
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
	 * @param drawFront if true draw the grid above other objects
	 *                  if false draw behind
	 */
	public void setGridParameters(int gridColor, float gridThickness, boolean drawFront) {
		setGridColor(gridColor);
		setGridThickness(gridThickness);
		setDrawFront(drawFront);
	}
	
	/**
	 * Draw the grid
	 * @param gridPositions starting points of grid along the concerned axis
	 */
	public void drawGrid(double[] gridPositions) {
		this.gridPositions = gridPositions;
		//drawGrid();
	}
	
	/**
	 * Draw the grif from computed positions.
	 * Each grid line is actually made of two consecutive lines, each one on one facet.
	 * @param startingPoints positions of the first points of lines
	 * @param middlePoints postions of the points belonging to both lines
	 * @param endPoints positions of the last point of the second line
	 */
	public void drawGrid(Vector3D[] startingPoints, Vector3D[] middlePoints, Vector3D[] endPoints) {
		
		
		this.startingPoints = startingPoints;
		this.middlePoints = middlePoints;
		this.endPoints = endPoints;
		
		showGrid();
		
		
	}
	
	/**
	 * Draw the grid
	 * @param firstAxisStart position of an end of the first axis the grid is drawn on
	 * @param firstAxisEnd position of an end of the first axis the grid is drawn on
	 * @param secondAxisStart position of an end of the second axis the grid is drawn on
	 * @param secondAxisEnd position of an end of the first axis the grid is drawn on
	 * @param thirdAxisStart position of an end of the first axis the grid is drawn on
	 * @param thirdAxisEnd position of an end of the first axis the grid is drawn on
	 * @param relativeTicksPositions relative position of grid, between the axis bounds
	 */
	public void drawGrid(double[] firstAxisStart, double[] firstAxisEnd,
						 double[] secondAxisStart, double[] secondAxisEnd,
						 double[] thirdAxisStart, double[] thirdAxisEnd,
						 double[] relativeTicksPositions) {
		
		// reallocate grid points
		int nbTicks = relativeTicksPositions.length;
		startingPoints = new Vector3D[nbTicks];
		middlePoints = new Vector3D[nbTicks];
		endPoints = new Vector3D[nbTicks];
		
		// comite axis directions
		Vector3D firstAxisDir = new Vector3D(firstAxisEnd[0] - firstAxisStart[0],
										     firstAxisEnd[1] - firstAxisStart[1],
										     firstAxisEnd[2] - firstAxisStart[2]);
		
		Vector3D secondAxisDir = new Vector3D(secondAxisEnd[0] - secondAxisStart[0],
											  secondAxisEnd[1] - secondAxisStart[1],
											  secondAxisEnd[2] - secondAxisStart[2]);
		
		Vector3D thirdAxisDir = new Vector3D(thirdAxisEnd[0] - thirdAxisStart[0],
											 thirdAxisEnd[1] - thirdAxisStart[1],
											 thirdAxisEnd[2] - thirdAxisStart[2]);
		
		Vector3D firstAxisStartV = new Vector3D(firstAxisStart);
		Vector3D secondAxisStartV = new Vector3D(secondAxisStart);
		Vector3D thirdAxisStartV = new Vector3D(thirdAxisStart);
		
		for (int i = 0; i < nbTicks; i++) {
			if (relativeTicksPositions[i] >= 0.0 && relativeTicksPositions[i] <= 1.0) {
				startingPoints[i] = firstAxisStartV.add(firstAxisDir.scalarMult(relativeTicksPositions[i]));
				middlePoints[i] = secondAxisStartV.add(secondAxisDir.scalarMult(relativeTicksPositions[i]));
				endPoints[i] = thirdAxisStartV.add(thirdAxisDir.scalarMult(relativeTicksPositions[i]));
			} else {
				startingPoints[i] = null;
				middlePoints[i] = null;
				endPoints[i] = null;
			}
		}
		
		// draw the grid using precomputed data
		showGrid();
		
	}
	
	/**
	 * Draw the grid using precomputed data
	 */
	public void showGrid() {
		GL gl = getGL();
		
		// use dash to draw grid
		GLTools.beginDashMode(gl, GLTools.GRID_DASH_INDEX, getGridThickness());
		
		double[] color = getGridColor();
		gl.glColor3d(color[0], color[1], color[2]);
		
		// draw grid above or behind
		CoordinateTransformation transform = getParentFigureGL().getCoordinateTransformation();
		if (isDrawingFront()) {
			transform.drawFront(gl);
		} else {
			transform.drawBack(gl);
		}
		
		//gl.glDisable(GL.GL_DEPTH_TEST);
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
		// disable front or back drawing
		//gl.glEnable(GL.GL_DEPTH_TEST);
		transform.drawMiddle(gl);
		
		GLTools.endDashMode(gl);
	}

}

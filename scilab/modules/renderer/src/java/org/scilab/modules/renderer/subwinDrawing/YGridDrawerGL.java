/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class for drawing grid on Y axis
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


package org.scilab.modules.renderer.subwinDrawing;


import org.scilab.modules.renderer.utils.geom3D.Vector3D;

/**
 * Class for drawing grid on X axis
 * @author Jean-Baptiste Silvy
 */
public class YGridDrawerGL extends GridDrawerGL {

	/**
	 * Default constructor
	 */
	public YGridDrawerGL() {
		super();
	}
	
	/**
	 * Get the starting points for grid drawing
	 * @param xCoordinate X coordinate of the Y axis
	 * @param zCoordinate Z coordinate of the Y axis
	 * @return array containing positions of grid
	 */
	private Vector3D[] getGridStartingPoints(double xCoordinate, double zCoordinate) {
		int nbGridLines = getGridNbPositions();
		Vector3D[] res = new Vector3D[nbGridLines];
		
		for (int i = 0; i < nbGridLines; i++) {
			double yCoordinate = getGridPosition(i);
			if (yCoordinate < getYmin() || yCoordinate > getYmax()) {
				res[i] = null;
			} else {
				res[i] = new Vector3D(xCoordinate, yCoordinate, zCoordinate);
			}
		}
		
		return res;
		
	}
	
	/**
	 * Get the middle points for grid drawing, there are the points belinging to both
	 * grid lines segment
	 * @param xCoordinate X coordinate of the Y axis
	 * @param zCoordinate Z coordinate of the Y axis
	 * @return array containing the positions
	 */
	private Vector3D[] getGridMiddlePoints(double xCoordinate, double zCoordinate) {
		// middle points are on the opposite of Y axis
		double xCoordinateMiddle = findOtherXBound(xCoordinate);
		
		return getGridStartingPoints(xCoordinateMiddle, zCoordinate);
	}
	
	/**
	 * Get the last point to display on the grid line.
	 * It is oppeosed on the Z axis with middle points
	 * @param xCoordinate X coordinate of the Y axis
	 * @param zCoordinate Z coordinate of the Y axis
	 * @return array containing the positions
	 */
	private Vector3D[] getGridEndPoints(double xCoordinate, double zCoordinate) {
		// middle points are on the opposite of Y axis
		double zCoordinateEnd = findOtherZBound(zCoordinate);
		
		return getGridMiddlePoints(xCoordinate, zCoordinateEnd);
	}
	
	/**
	 * Draw the grid with already set positions
	 */
	public void drawGrid() {
		double zCoordinate = findLowerZCoordinate();
		double xCoordinate = findFrontXCoordinate(zCoordinate);
		
		Vector3D[] startingPoints = getGridStartingPoints(xCoordinate, zCoordinate);
		Vector3D[] middlePoints = getGridMiddlePoints(xCoordinate, zCoordinate);
		Vector3D[] endPoints = getGridEndPoints(xCoordinate, zCoordinate);
		
		drawGrid(startingPoints, middlePoints, endPoints);
		
	}

}

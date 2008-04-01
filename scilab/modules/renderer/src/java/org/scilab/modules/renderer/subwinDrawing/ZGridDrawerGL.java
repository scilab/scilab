/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class for drawing grid on Z axis  
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
 * Class for drawing grid on Z axis
 * @author Jean-Baptiste Silvy
 */
public class ZGridDrawerGL extends GridDrawerGL {

	/**
	 * Default constructor
	 */
	public ZGridDrawerGL() {
		super();
	}
	
	/**
	 * Get the starting points for grid drawing
	 * @param xCoordinate X coordinate of the Z axis
	 * @param yCoordinate Y coordinate of the Z axis
	 * @return array containing positions of grid
	 */
	private Vector3D[] getGridStartingPoints(double xCoordinate, double yCoordinate) {
		int nbGridLines = getGridNbPositions();
		Vector3D[] res = new Vector3D[nbGridLines];
		
		for (int i = 0; i < nbGridLines; i++) {
			double zCoordinate = getGridPosition(i);
			if (zCoordinate < getZmin() || zCoordinate > getZmax()) {
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
	 * @param xCoordinate X coordinate of the Z axis
	 * @param yCoordinate Y coordinate of the Z axis
	 * @return array containing the positions
	 */
	private Vector3D[] getGridMiddlePoints(double xCoordinate, double yCoordinate) {
		// middle points are on the opposite of  X or Y axis
		double zCoordFront = findLowerZCoordinate();
		
		if (isSharingEndWithXaxis(zCoordFront, xCoordinate)) {
			// inverse y
			return getGridStartingPoints(xCoordinate, findOtherYBound(yCoordinate));
		} else {
			return getGridStartingPoints(findOtherXBound(xCoordinate), yCoordinate);
		}
	}
	
	/**
	 * Get the last point to display on the grid line.
	 * It is on the opposed edge of the box
	 * @param xCoordinate X coordinate of the Z axis
	 * @param yCoordinate Y coordinate of the Z axis
	 * @return array containing the positions
	 */
	private Vector3D[] getGridEndPoints(double xCoordinate, double yCoordinate) {
		
		return getGridStartingPoints(findOtherXBound(xCoordinate), findOtherYBound(yCoordinate));
	}
	
	/**
	 * Draw the grid with already set positions
	 */
	public void drawGrid() {

		double xCoordinate = findLeftMostXCoordinate();
		double yCoordinate = findLeftMostYCoordinate();
		
		Vector3D[] startingPoints = getGridStartingPoints(xCoordinate, yCoordinate);
		Vector3D[] middlePoints = getGridMiddlePoints(xCoordinate, yCoordinate);
		Vector3D[] endPoints = getGridEndPoints(xCoordinate, yCoordinate);
		
		drawGrid(startingPoints, middlePoints, endPoints);
		
	}

}

/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * desc : Class drawing axis aligned with X axis   
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


package org.scilab.modules.renderer.axesDrawing;

import org.scilab.modules.renderer.utils.geom3D.Vector3D;


/**
 * Class drawing axis alinged with X axis
 * @author Jean-Baptiste Silvy
 */
public class XAxesDrawerGL extends AxesTicksDrawerGL {

	private double yCoordinate;
	private double zCoordinate;
	
	/**
	 * Defulat constructor
	 */
	public XAxesDrawerGL() {
		super();
		yCoordinate = 0.0;
		zCoordinate = 0.0;
	}
	
	/**
	 * Set the y and z coordinates of the axis
	 * @param yCoordinate y Coordinate of the axis
	 * @param zCoordinate z Coordinate of the axis
	 */
	public void setConstantCoordinates(double yCoordinate, double zCoordinate) {
		this.yCoordinate = yCoordinate;
		this.zCoordinate = zCoordinate;
	}
	
	/**
	 * Compute ticks positions from an array of X coordinates
	 * @param xCoordinates X coordinates of ticks
	 * @param yCoordinate Y coordinate common for all ticks
	 * @param zCoordinate Z coordinate common for all ticks
	 * @return array of vector with only displayable ticks
	 */
	protected Vector3D[] findTicksPositions(double[] xCoordinates, double yCoordinate, double zCoordinate) {
		int nbTicks = xCoordinates.length;
		Vector3D[] res = new Vector3D[nbTicks];
		for (int i = 0; i < nbTicks; i++) {
			// only x coordinate is changing
			res[i] = new Vector3D(xCoordinates[i], yCoordinate, zCoordinate);
			res[i] = getTransform().getCanvasCoordinates(getGL(), res[i]);
		}
		return res;
	}
	
	/**
	 * @return an array containing positions of ticks along the axis
	 */
	protected Vector3D[] findTicksPositions() {
		return findTicksPositions(getTicksPositions(), yCoordinate, zCoordinate);
	}
	
	/**
	 * @return an array containing positions of subticks along the axis
	 */
	protected Vector3D[] findSubticksPositions() {
		return findTicksPositions(getSubTicksPositions(), yCoordinate, zCoordinate);
	}

	/**
	 * @return position of the beginning of the axis
	 */
	protected Vector3D getAxisSegmentStart() {
		return getTransform().getCanvasCoordinates(getGL(), new Vector3D(getTicksPositions()[0], yCoordinate, zCoordinate));
	}
	
	/**
	 * @return position of the end of the axis
	 */
	protected Vector3D getAxisSegmentEnd() {
		double[] xCoordinates = getTicksPositions();
		return getTransform().getCanvasCoordinates(getGL(),
												   new Vector3D(xCoordinates[xCoordinates.length - 1],
												   yCoordinate, zCoordinate));
	}
	
	
	
}

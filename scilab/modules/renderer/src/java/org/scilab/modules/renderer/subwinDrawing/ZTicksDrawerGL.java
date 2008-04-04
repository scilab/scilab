/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class drawing ticks for the Z axis       
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

/**
 * Class drawing ticks for the Z axis
 * @author Jean-Baptiste Silvy
 */
public class ZTicksDrawerGL extends TicksDrawerGL {

	/**
	 * Default constructor
	 */
	public ZTicksDrawerGL() {
		super();
	}
	
	/**
	 * @param xCoordinate X coordinate of the Z axis
	 * @param yCoordinate Y coordinate of the Z axis
	 * @return one of the axis segment edge
	 */
	protected Vector3D getAxisSegmentStart(double xCoordinate, double yCoordinate) {
		return new Vector3D(xCoordinate, yCoordinate, getZmin());
	}
	
	/**
	 * @param xCoordinate X coordinate of the Z axis
	 * @param yCoordinate Y coordinate of the Z axis
	 * @return the other axis segment edge
	 */
	protected Vector3D getAxisSegmentEnd(double xCoordinate, double yCoordinate) {
		return new Vector3D(xCoordinate, yCoordinate, getZmax());
	}
	
	/**
	 * @param xCoordinate X coordinate of the Z axis
	 * @param yCoordinate Y coordinate of the Z axis
	 * @return direction in which to draw the ticks
	 */
	public Vector3D findTicksDirection(double xCoordinate, double yCoordinate) {
		Vector3D res = findZTicksDirection(xCoordinate, yCoordinate);
		
		return setTicksDirectionLength(res);
		
	}
	
	/**
	 * Compute ticks positions from an array of Y coordinates
	 * @param xCoordinate X coordinates common for all ticks
	 * @param yCoordinate Y coordinates common for all ticks
	 * @param zCoordinates Z coordinates of ticks
	 * @return array of vector with only displayable ticks
	 */
	private Vector3D[] getTicksPosition(double xCoordinate, double yCoordinate, double[] zCoordinates) {
		int nbTicks = zCoordinates.length;
		Vector3D[] res = new Vector3D[nbTicks];
		
		GL gl = getGL();
		
		for (int i = 0; i < nbTicks; i++) {
			double zCoordinate = zCoordinates[i];
			// remove ticks wich are out of bounds
			if (yCoordinate <= getYmax() && yCoordinate >= getYmin()) {
				res[i] = new Vector3D(xCoordinate, yCoordinate, zCoordinate);
				res[i] = getTransform().getCanvasCoordinates(gl, res[i]);
			} else {
				res[i] = null;
			}
		}

		return res;
	}
	
	/**
	 * Get the base of each ticks segment.
	 * @param xCoordinate X coordinate of the Z axis
	 * @param yCoordinate Y coordinate of the Z axis
	 * @return array containing the base of each ticks
	 */
	protected Vector3D[] getTicksPositions(double xCoordinate, double yCoordinate) {
		return getTicksPosition(xCoordinate, yCoordinate, getTicksPositions());
	}
	
	/**
	 * Get the base of each ticks segment.
	 * @param xCoordinate X coordinate of the Z axis
	 * @param yCoordinate Y coordinate of the Z axis
	 * @return array containing the base of each subticks
	 */
	protected Vector3D[] getSubTicksPositions(double xCoordinate, double yCoordinate) {
		return getTicksPosition(xCoordinate, yCoordinate, getSubTicksPositions());
	}

	/**
	 * @return maximum distance from ticks to the axis.Draw ticks from the recorded data.
	 */
	public double drawTicks() {
		double yCoordinate = findLeftMostYCoordinate();
		double xCoordinate = findLeftMostXCoordinate();
		
		Vector3D[] ticksPosition = getTicksPositions(xCoordinate, yCoordinate);
		Vector3D[] subticksPosition = getSubTicksPositions(xCoordinate, yCoordinate);
		Vector3D ticksDirection = findTicksDirection(xCoordinate, yCoordinate);
		
		GL gl = getGL();
		Vector3D axisStartPix = getTransform().getCanvasCoordinates(gl, getAxisSegmentStart(xCoordinate, yCoordinate));
		Vector3D axisStartEnd = getTransform().getCanvasCoordinates(gl, getAxisSegmentEnd(xCoordinate, yCoordinate));
		
		return drawTicks(ticksPosition, subticksPosition, ticksDirection,
				         axisStartPix, axisStartEnd);
		
	}

}

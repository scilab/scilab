/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class drawing ticks for the Y axis
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
 * Class drawing ticks for the Y axis
 * @author Jean-Baptiste Silvy
 */
public abstract class YTicksDrawerGL extends TicksDrawerGL {

	/**
	 * Default constructor
	 */
	public YTicksDrawerGL() {
		super();
	}
	
	
	/**
	 * Compute the X coordinate of the Y axis segment
	 * Select the one wich is on the back of the axes box
	 * @param zCoordinate Z coordinate of the Y axis segment already computed by findZCoordinate
	 * @return X coordinate of the segment to draw
	 */
	protected double findBackXCoordinate(double zCoordinate) {
		if (findFrontXCoordinate(zCoordinate) == getXmin()) {
			return getXmax();
		} else {
			return getXmin();
		}
	}
	
	/**
	 * Compute the X coordinate of the Y axis segment
	 * @param zCoordinate Z coordinate of the Y axis segment already computed by findZCoordinate
	 * @return X coordinate of the segment to draw
	 */
	protected double findMiddleXCoordinate(double zCoordinate) {
		// find if 0 is in the range of X coordinates
		// if so then one of xMax or xMax is <= 0 and the other is not
		if (getXmin() * getXmax() <= 0.0) {
			return 0.0;
		} else if (getXmax() < 0.0) {
			// both bounds are negative
			return getXmax();
		} else {
			// both bounds are positive
			return getXmin();
		}
	}
	
	/**
	 * @param xCoordinate X coordinate of the Y axis
	 * @param zCoordinate Z coordinate of the Y axis
	 * @return one of the axis segment edge
	 */
	protected Vector3D getAxisSegmentStart(double xCoordinate, double zCoordinate) {
		return new Vector3D(xCoordinate, getYmin(), zCoordinate);
	}
	
	/**
	 * @param xCoordinate X coordinate of the Y axis
	 * @param zCoordinate Z coordinate of the Y axis
	 * @return the other axis segment edge
	 */
	protected Vector3D getAxisSegmentEnd(double xCoordinate, double zCoordinate) {
		return new Vector3D(xCoordinate, getYmax(), zCoordinate);
	}
	
	/**
	 * @param xCoordinate X coordinate of the Y axis
	 * @param zCoordinate Z coordinate of the Y axis
	 * @return direction in which to draw the ticks
	 */
	public Vector3D findTicksDirection(double xCoordinate, double zCoordinate) {
	
		Vector3D res = findYTicksDirection(xCoordinate, zCoordinate);
		
		// if angle is too small then use Z direction
		if (!checkTicksDirection(res, getAxisSegmentStart(xCoordinate, zCoordinate), getAxisSegmentEnd(xCoordinate, zCoordinate))) {
			if (Math.abs(zCoordinate - getZmin()) < Math.abs(zCoordinate - getZmax())) {
				// yCoordinate is closer to Ymin
				res = new Vector3D(0.0, 0.0, getZmin() - getZmax());
			} else {
				res = new Vector3D(0.0, 0.0, getZmax() - getZmin());
			}
		}
		
		return setTicksDirectionLength(res);
		
	}
	
	/**
	 * Compute ticks positions from an array of Y coordinates
	 * @param xCoordinate X coordinates common for all ticks
	 * @param yCoordinates Y coordinates of ticks
	 * @param zCoordinate Z coordinate common for all ticks
	 * @return array of vector with only displayable ticks
	 */
	private Vector3D[] getTicksPosition(double xCoordinate, double[] yCoordinates, double zCoordinate) {
		int nbTicks = yCoordinates.length;
		Vector3D[] res = new Vector3D[nbTicks];
		GL gl = getGL();
		
		for (int i = 0; i < nbTicks; i++) {
			double yCoordinate = yCoordinates[i];
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
	 * @param xCoordinate X coordinate of the Y axis
	 * @param zCoordinate Z coordinate of the Y axis
	 * @return array containing the base of each ticks
	 */
	protected Vector3D[] getTicksPositions(double xCoordinate, double zCoordinate) {
		return getTicksPosition(xCoordinate, getTicksPositions(), zCoordinate);
	}
	
	/**
	 * Get the base of each ticks segment.
	 * @param xCoordinate X coordinate of the Y axis
	 * @param zCoordinate Z coordinate of the Y axis
	 * @return array containing the base of each subticks
	 */
	protected Vector3D[] getSubTicksPositions(double xCoordinate, double zCoordinate) {
		return getTicksPosition(xCoordinate, getSubTicksPositions(), zCoordinate);
	}
	

	/**
	 * Draw ticks from the recorded data.
	 * @return maximum distance from ticks to the axis.
	 */
	public double drawTicks() {
		
		double zCoordinate = findZCoordinate();
		double xCoordinate = findXCoordinate(zCoordinate);
		
		Vector3D[] ticksPosition = getTicksPositions(xCoordinate, zCoordinate);
		Vector3D[] subticksPosition = getSubTicksPositions(xCoordinate, zCoordinate);
		Vector3D ticksDirection = findTicksDirection(xCoordinate, zCoordinate);
		
		GL gl = getGL();
		Vector3D axisStartPix = getTransform().getCanvasCoordinates(gl, getAxisSegmentStart(xCoordinate, zCoordinate));
		Vector3D axisStartEnd = getTransform().getCanvasCoordinates(gl, getAxisSegmentEnd(xCoordinate, zCoordinate));
		
		return drawTicks(ticksPosition, subticksPosition, ticksDirection,
				         axisStartPix, axisStartEnd);
	}

	/**
	 * Find the Z coordinate of the Y axis segment.
	 * Y axis segment is always displayed on the bottom of the box.
	 * @return Z coordinate of the segment to draw
	 */
	public abstract double findZCoordinate();
	
	/**
	 * Compute the X coordinate of the X axis segment
	 * @param zCoordinate Z coordinate of the Y axis segment already computed by findZCoordinate
	 * @return Y coordinate of the segment to draw
	 */
	public abstract double findXCoordinate(double zCoordinate);
	

}

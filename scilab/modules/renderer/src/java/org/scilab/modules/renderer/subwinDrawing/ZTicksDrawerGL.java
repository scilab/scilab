/*------------------------------------------------------------------------*/
/* file: ZTicksDrawerGL.java                                              */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class drawing ticks for the Z axis                              */
/*------------------------------------------------------------------------*/


package org.scilab.modules.renderer.subwinDrawing;

import javax.media.opengl.GL;

import org.scilab.modules.renderer.utils.CoordinateTransformation;
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
		GL gl = getGL();
		Vector3D res;
		
		// we must find wether Z axis ticks are directed by X axis or Y axis
		// Actually Z axis is direted by the axis with which it shares on edge
		// when X and Y axis are in defautl mode, ie bottom and left

		
		// get the three coordinates of the edge whuch is common for X and Y axis in default mode
		double zCoordFront = findLowerZCoordinate();
		double xCoordFront = YTicksDrawerGL.findFrontXCoordinate(gl, zCoordFront, getXmin(), getXmax(), getYmin());
		double yCoordFront = XTicksDrawerGL.findFrontYCoordinate(gl, zCoordFront, getXmin(), getYmin(), getYmax());
		
		// find if Z axis has a common edge with X axis or Z axis.
		if (xCoordinate == xCoordFront) {
			res = new Vector3D(0.0, yCoordinate - yCoordFront, 0.0);
		} else {
			res = new Vector3D(xCoordinate - xCoordFront, 0.0, 0.0);
		}
		
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
		
		for (int i = 0; i < nbTicks; i++) {
			double zCoordinate = zCoordinates[i];
			// remove ticks wich are out of bounds
			if (yCoordinate <= getYmax() && yCoordinate >= getYmin()) {
				res[i] = new Vector3D(xCoordinate, yCoordinate, zCoordinate);
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
	 * Check if labels can be displayed has if.
	 * @return true if ticks can be displayed or false if we need to reduc number of ticks.
	 */
	public boolean checkTicks() {
		double yCoordinate = findYCoordinate();
		double xCoordinate = findXCoordinate();
		
		Vector3D[] ticksPosition = getTicksPositions(xCoordinate, yCoordinate);
		Vector3D ticksDirection = findTicksDirection(xCoordinate, yCoordinate);
		
		return checkLabels(ticksPosition, ticksDirection);
	}

	/**
	 * Draw ticks from the recorded data.
	 */
	public void drawTicks() {
		double yCoordinate = findYCoordinate();
		double xCoordinate = findXCoordinate();
		
		Vector3D[] ticksPosition = getTicksPositions(xCoordinate, yCoordinate);
		Vector3D[] subticksPosition = getSubTicksPositions(xCoordinate, yCoordinate);
		Vector3D ticksDirection = findTicksDirection(xCoordinate, yCoordinate);
		drawTicksLines(ticksPosition, subticksPosition, ticksDirection,
					   getAxisSegmentStart(xCoordinate, yCoordinate),
					   getAxisSegmentEnd(xCoordinate, yCoordinate));
		
		drawLabels(ticksPosition, ticksDirection);

	}
	
	/**
	 * Find the X coordinate of the Z axis segment.
	 * Z axis segment is always displayed on the left of the box.
	 * @return X coordinate of the segment to draw
	 */
	public double findXCoordinate() {
		GL gl = getGL();
		
		// same processus as for Z coordinate
		Vector3D pointXmin = new Vector3D(getXmin(), getYmin(), getZmin());
		Vector3D pointXmax = new Vector3D(getXmax(), getYmin(), getZmin());
		
		// find the one which is upper in term of pixels
		CoordinateTransformation transform = CoordinateTransformation.getTransformation(gl);
		
		pointXmin = transform.getCanvasCoordinates(gl, pointXmin);
		pointXmax = transform.getCanvasCoordinates(gl, pointXmax);
		
		// get the one which is most on the right
		if (pointXmax.getX() > pointXmin.getX()) {
			return getXmin();
		} else {
			return getXmax();
		}
	}
	
	/**
	 * Compute the Y coordinate of the Z axis segment
	 * @return Y coordinate of the segment to draw
	 */
	public double findYCoordinate() {
		GL gl = getGL();
		
		// same processus as for Z coordinate
		Vector3D pointYmin = new Vector3D(getXmin(), getYmin(), getZmin());
		Vector3D pointYmax = new Vector3D(getXmin(), getYmax(), getZmin());
		
		// find the one which is upper in term of pixels
		CoordinateTransformation transform = CoordinateTransformation.getTransformation(gl);
		
		pointYmin = transform.getCanvasCoordinates(gl, pointYmin);
		pointYmax = transform.getCanvasCoordinates(gl, pointYmax);
		
		// get the one which is most on the left
		if (pointYmax.getX() > pointYmin.getX()) {
			return getYmin();
		} else {
			return getYmax();
		}
	}

}

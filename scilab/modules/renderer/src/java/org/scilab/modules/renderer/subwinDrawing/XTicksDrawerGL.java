/*------------------------------------------------------------------------*/
/* file: XTicksDrawerGL.java                                              */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class drawing ticks for the X axis                              */
/*------------------------------------------------------------------------*/


package org.scilab.modules.renderer.subwinDrawing;

import javax.media.opengl.GL;

import org.scilab.modules.renderer.utils.CoordinateTransformation;
import org.scilab.modules.renderer.utils.geom3D.Vector3D;


/**
 * Class drawing ticks for the X axis
 * @author Jean-Baptiste Silvy
 */
public class XTicksDrawerGL extends TicksDrawerGL {

	private static final double MAX_COS = 0.99;
	
	/**
	 * Default consturctor.
	 */
	public XTicksDrawerGL() {
		super();
	}
	
	
	/**
	 * Find the Z coordinate of the X axis segment.
	 * X axis segment is always displayed on the bottom of tyhe box.
	 * @return Z coordinate of the segment to draw
	 */
	public double findZCoordinate() {
		GL gl = getGL();
		
		// get two point with different Z.
		Vector3D pointZmin = new Vector3D(getXmin(), getYmin(), getZmin());
		Vector3D pointZmax = new Vector3D(getXmin(), getYmin(), getZmax());
		
		// find the one which is upper in term of pixels
		CoordinateTransformation transform = CoordinateTransformation.getTransformation(gl);
		
		pointZmin = transform.getCanvasCoordinates(gl, pointZmin);
		pointZmax = transform.getCanvasCoordinates(gl, pointZmax);
		
		if (pointZmax.getY() > pointZmin.getY()) {
			// standard case Zmax plane is higher than Zmin plane
			return getZmin();
		} else {
			return getZmax();
		}
		
	}
	
	/**
	 * Compute the Y coordinate of the X axis segment
	 * @param zCoordinate Z coordinate of the X axis segment alredy computed by findZCoordinate
	 * @return Y coordinate of the segment to draw
	 */
	public double findYCoordinate(double zCoordinate) {
		GL gl = getGL();
		
		// same processus as for Z coordinate
		Vector3D pointYmin = new Vector3D(getXmin(), getYmin(), zCoordinate);
		Vector3D pointYmax = new Vector3D(getXmin(), getYmax(), zCoordinate);
		
		// find the one which is upper in term of pixels
		CoordinateTransformation transform = CoordinateTransformation.getTransformation(gl);
		
		pointYmin = transform.getCanvasCoordinates(gl, pointYmin);
		pointYmax = transform.getCanvasCoordinates(gl, pointYmax);
		
		// get the lowest one
		if (pointYmax.getY() > pointYmin.getY()) {
			return getYmin();
		} else {
			return getYmax();
		}
		
	}
	
	/**
	 * @param yCoordinate Y coordinate of the X axis
	 * @param zCoordinate Z coordinate of the X axis
	 * @return direction in which to draw the ticks
	 */
	public Vector3D findTicksDirection(double yCoordinate, double zCoordinate) {
		GL gl = getGL();
		Vector3D res;
		if (Math.abs(yCoordinate - getYmin()) < Math.abs(yCoordinate - getYmax())) {
			// yCoordinate is closer to Ymin
			res = new Vector3D(0.0, getYmin() - getYmax(), 0.0);
		} else {
			res = new Vector3D(0.0, getYmax() - getYmin(), 0.0);
		}
		
		Vector3D origin = new Vector3D(0.0, 0.0, 0.0);
		
		CoordinateTransformation transform = CoordinateTransformation.getTransformation(gl);
		
		// to get real pixel values we should use project instead of getCanvasCoordinates
		// however gatCanvasCoordinate is viewport independant.
		origin = transform.getCanvasCoordinates(gl, origin);
		Vector3D pixRes = transform.getCanvasCoordinates(gl, res);
		
		// if ticks are too close from beeing horizonal, then put them vertical
		Vector3D ticksDir = pixRes.substract(origin);
		ticksDir.normalize();
		if (Math.abs(ticksDir.dotProduct(new Vector3D(1.0, 0.0, 0.0))) > MAX_COS) {
			if (Math.abs(zCoordinate - getZmin()) < Math.abs(zCoordinate - getZmax())) {
				// yCoordinate is closer to Ymin
				res = new Vector3D(0.0, 0.0, getZmin() - getZmax());
			} else {
				res = new Vector3D(0.0, 0.0, getZmax() - getZmin());
			}
		}
		pixRes = transform.getCanvasCoordinates(gl, res);
		// get length in pixels
		double pixelLength = pixRes.substract(origin).getNorm();
		
		// apply number of pixels 
		res.scalarMultSelf(TICKS_PIXEL_LENGTH / pixelLength);
		
		return res;
		
	}
	
	/**
	 * Compute ticks positions from an array of x coordinates
	 * @param xCoordinates X coordinates of ticks
	 * @param yCoordinate Y coordinate common for all ticks
	 * @param zCoordinate Z coordinate common for all ticks
	 * @return array of vector with only displayable ticks
	 */
	private Vector3D[] getTicksPosition(double[] xCoordinates, double yCoordinate, double zCoordinate) {
		int nbTicks = xCoordinates.length;
		Vector3D[] res = new Vector3D[nbTicks];
		
		for (int i = 0; i < nbTicks; i++) {
			double xCoordinate = xCoordinates[i];
			// remove ticks wich are out of bounds
			if (xCoordinate <= getXmax() && xCoordinate >= getXmin()) {
				res[i] = new Vector3D(xCoordinate, yCoordinate, zCoordinate);
			} else {
				res[i] = null;
			}
		}

		return res;
	}
	
	/**
	 * Get the base of each ticks segment.
	 * @param yCoordinate Y coordinate of the X axis
	 * @param zCoordinate Z coordinate of the X axis
	 * @return array containing the base of each ticks
	 */
	protected Vector3D[] getTicksPositions(double yCoordinate, double zCoordinate) {
		return getTicksPosition(getTicksPositions(), yCoordinate, zCoordinate);
	}
	
	/**
	 * Get the base of each ticks segment.
	 * @param yCoordinate Y coordinate of the X axis
	 * @param zCoordinate Z coordinate of the X axis
	 * @return array containing the base of each subticks
	 */
	protected Vector3D[] getSubTicksPositions(double yCoordinate, double zCoordinate) {
		return getTicksPosition(getSubTicksPositions(), yCoordinate, zCoordinate);
	}
	

	/**
	 * Draw ticks from the recorded data.
	 */
	public void drawTicks() {
		
		double zCoordinate = findZCoordinate();
		double yCoordinate = findYCoordinate(zCoordinate);
		
		// draw axis ticks and lines
		Vector3D axisSegmentStart = new Vector3D(getXmin(), yCoordinate, zCoordinate);
		Vector3D axisSegmentEnd = new Vector3D(getXmax(), yCoordinate, zCoordinate);
		
		Vector3D[] ticksPosition = getTicksPositions(yCoordinate, zCoordinate);
		Vector3D[] subticksPosition = getSubTicksPositions(yCoordinate, zCoordinate);
		Vector3D ticksDirection = findTicksDirection(yCoordinate, zCoordinate);
		drawTicksLines(ticksPosition, subticksPosition, ticksDirection, axisSegmentStart, axisSegmentEnd);
		
		drawLabels(ticksPosition, ticksDirection);
		
	}
	
	/**
	 * Check if labels can be displayed has if.
	 * @return true if ticks can be displayed or false if we need to reduc number of ticks.
	 */
	public boolean checkTicks() {
		double zCoordinate = findZCoordinate();
		double yCoordinate = findYCoordinate(zCoordinate);
		
		Vector3D[] ticksPosition = getTicksPositions(yCoordinate, zCoordinate);
		Vector3D ticksDirection = findTicksDirection(yCoordinate, zCoordinate);
		
		return checkLabels(ticksPosition, ticksDirection);
	}

}

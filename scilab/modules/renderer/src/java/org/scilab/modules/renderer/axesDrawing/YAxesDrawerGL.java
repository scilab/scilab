/*------------------------------------------------------------------------*/
/* file: YAxesDrawerGL.java                                               */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class drawing axis aligned with Y axis                          */
/*------------------------------------------------------------------------*/


package org.scilab.modules.renderer.axesDrawing;

import org.scilab.modules.renderer.utils.geom3D.Vector3D;


/**
 * Class drawing axis aligned with Y axis
 * @author Jean-Baptiste Silvy
 */
public class YAxesDrawerGL extends AxesTicksDrawerGL {

	private double xCoordinate;
	private double zCoordinate;
	
	/**
	 * Default constructor
	 */
	public YAxesDrawerGL() {
		super();
		xCoordinate = 0.0;
		zCoordinate = 0.0;
	}
	
	/**
	 * Set the x and z coordinates of the axis
	 * @param xCoordinate x Coordinate of the axis
	 * @param zCoordinate z Coordinate of the axis
	 */
	public void setConstantCoordinates(double xCoordinate, double zCoordinate) {
		this.xCoordinate = xCoordinate;
		this.zCoordinate = zCoordinate;
	}
	
	/**
	 * Compute ticks positions from an array of X coordinates
	 * @param xCoordinates X coordinate common for all ticks
	 * @param yCoordinates Y coordinates of ticks
	 * @param zCoordinate Z coordinate common for all ticks
	 * @return array of vector with only displayable ticks
	 */
	protected Vector3D[] findTicksPositions(double xCoordinates, double[] yCoordinates, double zCoordinate) {
		int nbTicks = yCoordinates.length;
		Vector3D[] res = new Vector3D[nbTicks];
		for (int i = 0; i < nbTicks; i++) {
			// only x coordinate is changing
			res[i] = new Vector3D(xCoordinate, yCoordinates[i], zCoordinate);
		}
		return res;
	}
	
	/**
	 * @return an array containing positions of ticks along the axis
	 */
	protected Vector3D[] findTicksPositions() {
		return findTicksPositions(xCoordinate, getTicksPositions(), zCoordinate);
	}
	
	/**
	 * @return an array containing positions of subticks along the axis
	 */
	protected Vector3D[] findSubticksPositions() {
		return findTicksPositions(xCoordinate, getSubTicksPositions(), zCoordinate);
	}

	/**
	 * @return position of the beginning of the axis
	 */
	protected Vector3D getAxisSegmentStart() {
		return new Vector3D(xCoordinate, getTicksPositions()[0], zCoordinate);
	}
	
	/**
	 * @return position of the end of the axis
	 */
	protected Vector3D getAxisSegmentEnd() {
		double[] yCoordinates = getTicksPositions();
		return new Vector3D(xCoordinate,
							yCoordinates[yCoordinates.length - 1],
							zCoordinate);
	}
	
	
	
}

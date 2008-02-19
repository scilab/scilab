/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Object drawing decoaration around axes box 
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

/**
 * Object drawing decoaration around axes box
 * @author Jean-Baptiste Silvy
 */
public abstract class BoxTrimmingObjectGL extends DrawableObjectGL {

	private double xMin;
	private double xMax;
	private double yMin;
	private double yMax;
	private double zMin;
	private double zMax;

	/**
	 * Default constructor
	 */
	public BoxTrimmingObjectGL() {
		super();
		xMin = 0.0;
		xMax = 0.0;
		yMin = 0.0;
		yMax = 0.0;
		zMin = 0.0;
		zMax = 0.0;
	}
	
	/**
	 * Should not be called
	 * @param parentFigureIndex index of parent figure
	 */
	public void show(int parentFigureIndex) {
		// should not be called
	}
	
	/**
	 * Set axes box to know were to draw items.
	 * @param xMin minimun bounds on X axis.
	 * @param xMax maximum bounds on X axis.
	 * @param yMin minimun bounds on Y axis.
	 * @param yMax maximum bounds on Y axis.
	 * @param zMin minimun bounds on Z axis.
	 * @param zMax maximum bounds on Z axis.
	 */
	public void setAxesBounds(double xMin, double xMax, double yMin, double yMax, double zMin, double zMax) {
		this.xMin = xMin;
		this.xMax = xMax;
		this.yMin = yMin;
		this.yMax = yMax;
		this.zMin = zMin;
		this.zMax = zMax;
	}
	
	/**
	 * @return minimium bound on X axis
	 */
	protected double getXmin() {
		return xMin;
	}
	
	/**
	 * @return maximum bound on X axis
	 */
	protected double getXmax() {
		return xMax;
	}
	
	/**
	 * @return minimium bound on Y axis
	 */
	protected double getYmin() {
		return yMin;
	}
	
	/**
	 * @return maximum bound on Y axis
	 */
	protected double getYmax() {
		return yMax;
	}
	
	/**
	 * @return minimium bound on Z axis
	 */
	protected double getZmin() {
		return zMin;
	}
	
	/**
	 * @return maximum bound on Z axis
	 */
	protected double getZmax() {
		return zMax;
	}
	
	/**
	 * Knowing on of the X bounds (xMin or xMax), find the other
	 * @param xBound xMin or xMax
	 * @return the other bound
	 */
	protected double findOtherXBound(double xBound) {
		if (Math.abs(xBound - xMin) < Math.abs(xBound - xMax)) {
			// closer to xMin
			return xMax;
		} else {
			return xMin;
		}
	}
	
	/**
	 * Knowing on of the y bounds (yMin or yMax), find the other
	 * @param yBound yMin or yMax
	 * @return the other bound
	 */
	protected double findOtherYBound(double yBound) {
		if (Math.abs(yBound - yMin) < Math.abs(yBound - yMax)) {
			// closer to yMin
			return yMax;
		} else {
			return yMin;
		}
	}
	
	/**
	 * Knowing on of the Z bounds (zMin or zMax), find the other
	 * @param zBound zMin or zMax
	 * @return the other bound
	 */
	protected double findOtherZBound(double zBound) {
		if (Math.abs(zBound - zMin) < Math.abs(zBound - zMax)) {
			// closer to zMin
			return zMax;
		} else {
			return zMin;
		}
	}
	
	/**
	 * Compute the X coordinate of the Y axis segment
	 * Select the one wich is in front of the camera
	 * @param zCoordinate zCoordinate Z coordinate of the Y axis segment
	 * @return X coordinate of the segment to draw
	 */
	protected double findFrontXCoordinate(double zCoordinate) {
		
		GL gl = getGL();
		
		// same processus as for Z coordinate
		Vector3D pointXmin = new Vector3D(getXmin(), getYmin(), zCoordinate);
		Vector3D pointXmax = new Vector3D(getXmax(), getYmin(), zCoordinate);
		
		// find the one which is upper in term of pixels
		CoordinateTransformation transform = CoordinateTransformation.getTransformation(gl);
		
		pointXmin = transform.getCanvasCoordinates(gl, pointXmin);
		pointXmax = transform.getCanvasCoordinates(gl, pointXmax);
		
		// get the lowest one
		if (pointXmax.getY() > pointXmin.getY()) {
			return getXmin();
		} else {
			return getXmax();
		}
	}
	
	/**
	 * Find the height of the bottom facet
	 * @return Z coordinate of the segment to draw
	 */
	protected double findLowerZCoordinate() {
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
	 * Select the one wich is in front of the camera
	 * @param zCoordinate Z coordinate of the X axis segment already computed by findZCoordinate
	 * @return Y coordinate of the segment to draw
	 */
	protected double findFrontYCoordinate(double zCoordinate) {
		GL gl = getGL();
		
//		 same processus as for Z coordinate
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
	 * Compute the Y coordinate of the Z axis segment
	 * @return Y coordinate of the segment to draw
	 */
	public double findLeftMostYCoordinate() {
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
	
	/**
	 * Find the X coordinate of the Z axis segment.
	 * Z axis segment is always displayed on the left of the box.
	 * @return X coordinate of the segment to draw
	 */
	public double findLeftMostXCoordinate() {
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
	 * To know if the Z axis is sharing an edge with X bottom axis
	 * @param zCoordFront Z coordinate of the front point
	 * @param xCoordinate X coordinate of the Z axis
	 * @return true if X axis and Z axis has a common edge
	 */
	protected boolean isSharingEndWithXaxis(double zCoordFront, double xCoordinate) {
		//	 get the three coordinates of the edge whuch is common for X and Y axis in default mode
		double xCoordFront = findFrontXCoordinate(zCoordFront);
		double xCoordBack  = findOtherXBound(xCoordFront);
		
		// equivalent to return xCoordinate == xCoordBack
		return (Math.abs(xCoordinate - xCoordBack) < Math.abs(xCoordinate - xCoordFront));
	}
	
	/**
	 * Compute the default direction of ticks on X axis, parallel to Y axis
	 * @param yCoordinate Y coordinate of the X axis
	 * @param zCoordinate Z coordinate of the X axis
	 * @return direction of the ticks normalized
	 */
	protected Vector3D findXTicksDirection(double yCoordinate, double zCoordinate) {
		Vector3D res;
		if (Math.abs(yCoordinate - getYmin()) <= Math.abs(yCoordinate - getYmax())) {
			// yCoordinate is closer to Ymin
			res = new Vector3D(0.0, getYmin() - getYmax(), 0.0);
		} else {
			res = new Vector3D(0.0, getYmax() - getYmin(), 0.0);
		}
		return res.getNormalized();
	}
	
	/**
	 * Compute the default direction of ticks on Y axis, parallel to X axis
	 * @param xCoordinate X coordinate of the Y axis
	 * @param zCoordinate Z coordinate of the Y axis
	 * @return direction of the ticks normalized
	 */
	protected Vector3D findYTicksDirection(double xCoordinate, double zCoordinate) {
		Vector3D res;
		if (Math.abs(xCoordinate - getXmin()) <= Math.abs(xCoordinate - getXmax())) {
			// yCoordinate is closer to Ymin
			res = new Vector3D(getXmin() - getXmax(), 0.0, 0.0);
		} else {
			res = new Vector3D(getXmax() - getXmin(), 0.0, 0.0);
		}
		return res.getNormalized();
	}

	/**
	 * Compute the default direction of ticks on Z axis.
	 * @param xCoordinate X coordinate of the Z axis
	 * @param yCoordinate Y coordinate of the Z axis
	 * @return direction of the ticks normalized
	 */
	protected Vector3D findZTicksDirection(double xCoordinate, double yCoordinate) {
		Vector3D res;
		// we must find wether Z axis ticks are directed by X axis or Y axis
		// Actually Z axis is directed by the axis with which it shares on edge
		// when X and Y axis are in defautl mode, ie bottom and left

		
		// get the three coordinates of the edge which is common for X and Y axis in default mode
		double zCoordFront = findLowerZCoordinate();
		
		if (isSharingEndWithXaxis(zCoordFront, xCoordinate)) {
			res = new Vector3D(xCoordinate - findFrontXCoordinate(zCoordFront), 0.0, 0.0);
		} else {
			res = new Vector3D(0.0, yCoordinate - findFrontYCoordinate(zCoordFront), 0.0);
		}
		return res.getNormalized();
	}
	
}

/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Represent a 3D vector 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


package org.scilab.modules.renderer.utils.geom3D;


/**
 * Class which represent a 3D vector
 * @author Jean-Baptiste Silvy
 */
public class Vector3D {

	public static final Vector3D ORIGIN = new Vector3D(0.0, 0.0, 0.0);
	public static final Vector3D X_AXIS = new Vector3D(1.0, 0.0, 0.0);
	public static final Vector3D Y_AXIS = new Vector3D(0.0, 1.0, 0.0);
	public static final Vector3D Z_AXIS = new Vector3D(0.0, 0.0, 1.0);
	
	/** Number of dimensions in a Vector */
	public static final int DIMENSION = 3;
	
	private double xCoord;
	private double yCoord;
	private double zCoord;
	
	/**
	 * Default constructor
	 */
	public Vector3D() {
		xCoord = 0.0;
		yCoord = 0.0;
		zCoord = 0.0;
	}
	
	/**
	 * Copy constructor
	 * @param copy the vector which is copied
	 */
	public Vector3D(Vector3D copy) {
		setValues(copy);
	}
	
	/**
	 * Constructor from an array
	 * @param values array with 3 values
	 */
	public Vector3D(double[] values) {
		setValues(values);
	}
	
	/**
	 * Constructor from the 3 coordinates
	 * @param xCoord X coordinate
	 * @param yCoord Y coordinate
	 * @param zCoord Z coordinate
	 */
	public Vector3D(double xCoord, double yCoord, double zCoord) {
		setValues(xCoord, yCoord, zCoord);
	}
	
	/**
	 * @return Vector X coordinates
	 */
	public double getX() {
		return xCoord;
	}
	
	/**
	 * @return Vector Y coordinates
	 */
	public double getY() {
		return yCoord;
	}
	
	/**
	 * @return Vector Z coordinates
	 */
	public double getZ() {
		return zCoord;
	}
	
	/**
	 * @return Vector X coordinates as float
	 */
	public float getXf() {
		return (float) xCoord;
	}
	
	/**
	 * @return Vector Y coordinates as float
	 */
	public float getYf() {
		return (float) yCoord;
	}
	
	/**
	 * @return Vector Z coordinates as float
	 */
	public float getZf() {
		return (float) zCoord;
	}
	
	/**
	 * Specify a new X coordinate
	 * @param xCoord new coordinate
	 */
	public void setX(double xCoord) {
		this.xCoord = xCoord;
	}
	
	/**
	 * Specify a new Y coordinate
	 * @param yCoord new coordinate
	 */
	public void setY(double yCoord) {
		this.yCoord = yCoord;
	}
	
	/**
	 * Specify a new Z coordinate
	 * @param zCoord new coordinate
	 */
	public void setZ(double zCoord) {
		this.zCoord = zCoord;
	}
	
	
	/**
	 * Set new coordinates for the vector
	 * @param x new X coordinate
	 * @param y new Y coordinate
	 * @param z new Z coordinate
	 */
	public void setValues(double x, double y, double z) {
		xCoord = x;
		yCoord = y;
		zCoord = z;
	}
	
	/**
	 * Set coordinates of the vector
	 * @param values array of size 3 containing the coordinates
	 */
	public void setValues(double[] values) {
		xCoord = values[0];
		yCoord = values[1];
		zCoord = values[2];
	}
	
	/**
	 * Copy a vector to this one
	 * @param copy vector to copy
	 */
	public void setValues(Vector3D copy) {
		xCoord = copy.xCoord;
		yCoord = copy.yCoord;
		zCoord = copy.zCoord;
	}
	
	/**
	 * @return array of the 3 coodinates
	 */
	public double[] getValues() {
		double[] res = {xCoord, yCoord, zCoord};
		return res;
	}
	
	/**
	 * Return the dot product of this vector with an other
	 * @param v2 other vector
	 * @return dot prodeuct of the two vectors
	 */
	public double dotProduct(Vector3D v2) {
		return xCoord * v2.xCoord + yCoord * v2.yCoord + zCoord * v2.zCoord;
	}
	
	/**
	 * Compute the cross product of this vector with an other
	 * @param v2 other vector
	 * @return new vector, result of the cross product
	 */
	public Vector3D crossProduct(Vector3D v2) {
		return new Vector3D(yCoord * v2.zCoord - zCoord * v2.yCoord,
							zCoord * v2.xCoord - xCoord * v2.zCoord,
							xCoord * v2.yCoord - yCoord * v2.xCoord);
	}
	
	/**
	 * @return norm of the vector
	 */
	public double getNorm() {
		return Math.sqrt(getSquareNorm());
	}
	
	/**
	 * @return square of the vector norm
	 */
	public double getSquareNorm() {
		return this.dotProduct(this);
	}
	
	/**
	 * Normalize the vector
	 */
	public void normalize() {
		double norm = getNorm();
		xCoord /= norm;
		yCoord /= norm;
		zCoord /= norm;
	}
	
	/**
	 * Return a normalized vector of this
	 * @return new normalized vector
	 */
	public Vector3D getNormalized() {
		Vector3D res = new Vector3D(this);
		res.normalize();
		return res;
	}
	
	/**
	 * Create the relut of mutiplying the vactor by a scalar
	 * @param scalar scalar to use
	 * @return new vector result of mutiplying this by a scalar
	 */
	public Vector3D scalarMult(double scalar) {
		return new Vector3D(scalar * xCoord, scalar * yCoord, scalar * zCoord);
	}
	
	/**
	 * Apply a scale to the vector.
	 * @param scaleX scale along X axis.
	 * @param scaleY scale along Y axis.
	 * @param scaleZ scale along Z axis.
	 * @return new vector with scaled applied.
	 */
	public Vector3D scale(double scaleX, double scaleY, double scaleZ) {
		return new Vector3D(scaleX * xCoord, scaleY * yCoord, scaleZ * zCoord);
	}
	
	/**
	 * Multiply this vector by a scalar.
	 * @param scalar scalar to use
	 */
	public void scalarMultSelf(double scalar) {
		xCoord = scalar * xCoord;
		yCoord = scalar * yCoord;
		zCoord = scalar * zCoord;
	}
	
	/**
	 * Compute the sum of the vector and an other
	 * @param v2 other vector
	 * @return new vector which is the sum of the two above
	 */
	public Vector3D add(Vector3D v2) {
		return new Vector3D(xCoord + v2.xCoord, yCoord + v2.yCoord, zCoord + v2.zCoord);
	}
	
	/**
	 * Compute the difference of the vector and an other
	 * @param v2 other vector
	 * @return new vector, this - v2
	 */
	public Vector3D substract(Vector3D v2) {
		return new Vector3D(xCoord - v2.xCoord, yCoord - v2.yCoord, zCoord - v2.zCoord);
	}
	
	/**
	 * @return a vector which is the opposite of this
	 */
	public Vector3D getOpposite() {
		return new Vector3D(-xCoord, -yCoord, -zCoord);
	}
	
	/**
	 * Print the vector
	 * @return text of the vector
	 */
	@Override
	public String toString() {
		final String comma = ", ";
		return "[" + xCoord + comma + yCoord + comma + zCoord + "]";
	}
	
	/**
	 * @return an other vector copy of this one.
	 */
	public Vector3D getCopy() {
		return new Vector3D(xCoord, yCoord, zCoord);
	}
	
	/**
	 * Compute the rotation of this vector (as a point) around an axis.
	 * @param axisCenter point on the axis
	 * @param axisDir direction of the axis
	 * @param angle rotation angle in radian
	 * @return new vector, result of the rotation (as a point)
	 */
	public Vector3D rotate(Vector3D axisCenter, Vector3D axisDir, double angle) {
		axisDir.normalize();
		
		// Olinde Rodrigues formula
		// V = cosa.U + (1-cosa)(U.N).N + sina.N^U
		// where V is the result, U the intial point and N the axes
		// which is supposed to be centered on the origin.
		// For us we need to translate to the axes center A.
		// in our case: P' = A + cosa.AP + (1-cosa).(AP.N).N + sina.N.AP
		// where P is the point to rotate and P' the resulting point
		Vector3D ap = this.substract(axisCenter);
		Vector3D firstPart = ap.scalarMult(Math.cos(angle));
		Vector3D secondPart = axisDir.scalarMult(ap.dotProduct(axisDir));
		secondPart.scalarMultSelf(1.0 - Math.cos(angle)); // we got the second part of the formula
		Vector3D thirdPart = axisDir.crossProduct(ap).scalarMult(Math.sin(angle));
		
		// sum all
		return axisCenter.add(firstPart.add(secondPart.add(thirdPart)));
		
	}
	
	/**
	 * @return true is all the coordinates of the vector are finites
	 */
	public boolean isFinite() {
		return GeomAlgos.isVector3DFinite(xCoord, yCoord, zCoord);
	}
	
	/**
	 * Test if this vector equals another
	 * use with caution since it deals with double
	 * @param v2 other vector to test
	 * @return true if the each coordinates of the two vector equals
	 */
	public boolean equals(Vector3D v2) {
		return   (xCoord == v2.xCoord)
		      && (yCoord == v2.yCoord)
		      && (zCoord == v2.zCoord);
	}
	
	/**
	 * We must provide this
	 * @return super hashcode
	 */
	public int hashCode() {
		return (int) xCoord + (int) yCoord + (int) zCoord;
	}
	
	/**
	 * @return greatest coordinate of the vector
	 */
	public double getMaxCoordinate() {
		return Math.max(xCoord, Math.max(yCoord, zCoord));
	}
	
	/**
	 * @return lowest coordinate of the vector
	 */
	public double getMinCoordinate() {
		return Math.min(xCoord, Math.min(yCoord, zCoord));
	}
	
	
}

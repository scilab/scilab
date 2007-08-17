/*------------------------------------------------------------------------*/
/* file: Matrix4D.java                                                    */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Represent a 4x4 matrix (homogenous coordinates)                 */
/*------------------------------------------------------------------------*/


package org.scilab.modules.renderer.utils.geom3D;

/**
 * Represent a 4x4 matrix (homogenous coordinates) 
 * @author Jean-Baptiste Silvy
 * @copyright INRIA 2007
 */
public class Matrix4D {

	private static final int MATRIX_SIZE = 4;
	private static final int MATRIX_NB_ELEMENTS = MATRIX_SIZE * MATRIX_SIZE;
	
	private double[][] values = new double[MATRIX_SIZE][MATRIX_SIZE];
	
	/**
	 * Default contructor. Set matrix to identity.
	 */
	public Matrix4D() {
		setToIdentity();
	}
	
	/**
	 * Set the matrix to identity.
	 */
	public void setToIdentity() {
		for (int i = 0; i < MATRIX_SIZE; i++) {
			for (int j = 0; j < MATRIX_SIZE; j++) {
				values[i][j] = 0.0;
			}
			values[i][i] = 1.0;
		}
	}
	
	/**
	 * Set values of a rotation matrix from the current orthogonal basis (X,Y,Z) and a new one.
	 * The three vector must be othogonal.
	 * @param newX first vector of the new basis.
	 * @param newY second vector of the new basis.
	 * @param newZ thirs vector of the new basis.
	 */
	public void setFromRotatedBasis(Vector3D newX, Vector3D newY, Vector3D newZ) {

		// Create an orthonormal and normalized basis.
		double[] xValues = newX.getNormalized().getValues();
		double[] yValues = newY.getNormalized().getValues();
		double[] zValues = newZ.getNormalized().getValues();
		
		setToIdentity();
		
		for (int i = 0; i < MATRIX_SIZE - 1; i++) {
			values[i][0] = xValues[i];
			values[i][1] = yValues[i];
			values[i][2] = zValues[i];
		}
	}
	
	/**
	 * Get the Matrix as an array so it can be used by OpenGL
	 * @return an array of size 16, stored column wise.
	 */
	public double[] getOpenGLRepresentation() {
		double[] res = new double[MATRIX_NB_ELEMENTS];
		
		for (int j = 0; j < MATRIX_SIZE; j++) {
			res[MATRIX_SIZE * j] = values[0][j];
			res[MATRIX_SIZE * j + 1] = values[1][j];
			res[MATRIX_SIZE * j + 2] = values[2][j];
			res[MATRIX_SIZE * j + MATRIX_SIZE - 1] = values[MATRIX_SIZE - 1][j];
		}
		return res;
	}
	
	/**
	 * Print the matrix
	 * @return text of the matrix
	 */
	public String toString() {
		final String comma = ", ";
		String res = new String();
		for (int i = 0; i < MATRIX_SIZE; i++) {
			res += "[" + values[i][0] + comma + values[i][1] + comma + values[i][2] + comma + values[i][MATRIX_SIZE - 1] + "]" + "\n";
		}
		return res;
	}
}

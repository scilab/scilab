/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Represent a 4x4 matrix (homogenous coordinates)
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
 * Represent a 4x4 matrix (homogenous coordinates) 
 * @author Jean-Baptiste Silvy
 * @copyright INRIA 2007
 */
public class Matrix4D {

	/** Number of row or column of a matrix */
	public static final int MATRIX_SIZE = 4;
	/** Number of elements in a matrix */
	public static final int MATRIX_NB_ELEMENTS = MATRIX_SIZE * MATRIX_SIZE;
	
	private static final String COMMA = ", ";
	
	private double[][] values = new double[MATRIX_SIZE][MATRIX_SIZE];
	
	/**
	 * Default contructor. Set matrix to identity.
	 */
	public Matrix4D() {
		setToZero();
	}
	
	/**
	 * Create a new matrix whose data are given by an OpenGL matrix 
	 * @param openGLRepresentation array of size 16, stored column wise.
	 */
	public Matrix4D(double[] openGLRepresentation) {
		setFromOpenGLRepresentation(openGLRepresentation);
	}
	
	/**
	 * Copy constructor
	 * @param copyMatrix matrix to copy
	 */
	public Matrix4D(Matrix4D copyMatrix) {
		for (int i = 0; i < MATRIX_SIZE; i++) {
			for (int j = 0; j < MATRIX_SIZE; j++) {
				this.values[i][j] = copyMatrix.values[i][j];
			}
		}
	}
	
	/**
	 * Set the matrix to the 0 matrix.
	 */
	public void setToZero() {
		for (int i = 0; i < MATRIX_SIZE; i++) {
			for (int j = 0; j < MATRIX_SIZE; j++) {
				values[i][j] = 0.0;
			}
		}
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
	 * Set a matrix from an OpenGL representation of matrix
	 * @param openGLMatrix an array of size 16, stored column wise.
	 */
	public void setFromOpenGLRepresentation(double[] openGLMatrix) {
		for (int j = 0; j < MATRIX_SIZE; j++) {
			values[0][j] = openGLMatrix[MATRIX_SIZE * j];
			values[1][j] = openGLMatrix[MATRIX_SIZE * j + 1];
			values[2][j] = openGLMatrix[MATRIX_SIZE * j + 2];
			values[MATRIX_SIZE - 1][j] = openGLMatrix[MATRIX_SIZE * j + MATRIX_SIZE - 1];
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
	 * @return an array of size 16, stored row wise.
	 */
	public double[] getRowWiseRepresentation() {
		double[] res = new double[MATRIX_NB_ELEMENTS];
		
		for (int i = 0; i < MATRIX_SIZE; i++) {
			res[MATRIX_SIZE * i] = values[i][0];
			res[MATRIX_SIZE * i + 1] = values[i][1];
			res[MATRIX_SIZE * i + 2] = values[i][2];
			res[MATRIX_SIZE * i + MATRIX_SIZE - 1] = values[i][MATRIX_SIZE - 1];
		}
		return res;
	}
	
	/**
	 * Change an element of the matrix
	 * @param numRow index of the element row
	 * @param numCol index of the element column
	 * @param value new value to set
	 */
	public void setElement(int numRow, int numCol, double value) {
		values[numRow][numCol] = value;
	}
	
	/**
	 * Return the product of this matrix by a vector.
	 * @param vect vector to multiply considered as a 4D vector by setting its w to 1.
	 * @return new vector, result of the product
	 */
	public Vector3D mult(Vector3D vect) {
		double[] resValues = new double[MATRIX_SIZE];
		
		// w coordinate of the vector is supposed to be 1;
        for (int i = 0; i < MATRIX_SIZE; i++) {
            resValues[i]  =   vect.getX() * values[i][0] + vect.getY() * values[i][1]
                            + vect.getZ() * values[i][2] + values[i][MATRIX_SIZE - 1];
        }
        resValues[0] /= resValues[MATRIX_SIZE - 1];
        resValues[1] /= resValues[MATRIX_SIZE - 1];
        resValues[2] /= resValues[MATRIX_SIZE - 1];
        return new Vector3D(resValues);
	}
	
	/**
	 * Mutliplication using only the 3x3 upper left part of the matrix.
	 * @param vect vector to multiply
	 * @return new vector, result of the product
	 */
	public Vector3D mult3D(Vector3D vect) {
		double[] resValues = {0.0, 0.0, 0.0};
		
		// w coordinate of the vector is supposed to be 1;
        for (int i = 0; i < MATRIX_SIZE - 1; i++) {
            resValues[i]  =   vect.getX() * values[i][0] + vect.getY() * values[i][1]
                            + vect.getZ() * values[i][2];
        }
        return new Vector3D(resValues);
	}
	
	/**
	 * Return the product of this matrix by an other matrix
	 * @param mat matrix to multiply
	 * @return new matrix, result of the product
	 */
	public Matrix4D mult(Matrix4D mat) {
		Matrix4D res = new Matrix4D();

        for (int i = 0; i < MATRIX_SIZE; i++)  {
        	for (int j = 0; j < MATRIX_SIZE; j++) {
        		for (int c = 0; c < MATRIX_SIZE; c++) {
        			res.values[i][j] += this.values[i][c] * mat.values[c][j];
        		}
        	}
        }
        return res;
	}
	
	/**
	 * Return the relust of the substraction of two matrices.
	 * @param mat matrix to substract to this.
	 * @return new matrix result of substraction/
	 */
	public Matrix4D substract(Matrix4D mat) {
		Matrix4D res = new Matrix4D();
		for (int i = 0; i < MATRIX_SIZE; i++) {
			for (int j = 0; j < MATRIX_SIZE; j++) {
				res.values[i][j] = this.values[i][j] - mat.values[i][j];
			}
		}
		return res;
	}
	
	/**
	 * Return the inverse of this matrix.
	 * @return new matrix which is the inverse of this one.
	 */
	public Matrix4D getInverse() {
		Matrix4D res = new Matrix4D();

		double[][] s = new double[MATRIX_SIZE][2 * MATRIX_SIZE];
        double[] tmprow = null;

        
        int i;
        int j;
        int p;
        int jj;
        for (i = 0; i < MATRIX_SIZE; i++) {
            for (j = 0; j < MATRIX_SIZE; j++) {
                s[i][j] = values[i][j];
                if (i == j)	{
                	s[i][j + MATRIX_SIZE] = 1.0;
                } else {
                	s[i][j + MATRIX_SIZE] = 0.0;
                }
            }
        }
        
        double[] scp = new double[MATRIX_SIZE];
        for (i = 0; i < MATRIX_SIZE; i++) {
            scp[i] = Math.abs(s[i][0]);
            for (j = 1; j < MATRIX_SIZE; j++) {
                if (Math.abs(s[i][j]) > scp[i]) {
                	scp[i] = Math.abs(s[i][j]);
                }
            }
            if (scp[i] == 0.0) {
            	return res; // singular matrix! }
            }
        }

        int pivotTo;
        double scpMax;
        for (i = 0; i < MATRIX_SIZE; i++) {
            // select pivot row
            pivotTo = i;
            scpMax = Math.abs(s[i][i] / scp[i]);
            // find out which row should be on top
            for (p = i + 1; p < MATRIX_SIZE; p++) {
                if (Math.abs(s[p][i] / scp[p]) > scpMax) {
                    scpMax = Math.abs(s[p][i] / scp[p]);
                    pivotTo = p;
                }
            }
            
            // Pivot if necessary
            if (pivotTo != i) {
                tmprow = s[i];
                s[i] = s[pivotTo];
                s[pivotTo] = tmprow;
                double tmpscp;
                tmpscp = scp[i];
                scp[i] = scp[pivotTo];
                scp[pivotTo] = tmpscp;
            }

            double mji;
            // perform gaussian elimination
            for (j = i + 1; j < MATRIX_SIZE; j++) {
                mji = s[j][i] / s[i][i];
                s[j][i] = 0.0;
                for (jj = i + 1; jj < 2 * MATRIX_SIZE; jj++) {
                    s[j][jj] -= mji * s[i][jj];
                }
            }
        }
        if (s[MATRIX_SIZE - 1][MATRIX_SIZE - 1] == 0.0) {
        	return res; // singular matrix! }
        }

        //
        // Now we have an upper triangular matrix.
        //
        //  x x x x | y y y y
        //  0 x x x | y y y y 
        //  0 0 x x | y y y y
        //  0 0 0 x | y y y y
        //
        //  we'll back substitute to get the inverse
        //
        //  1 0 0 0 | z z z z
        //  0 1 0 0 | z z z z
        //  0 0 1 0 | z z z z
        //  0 0 0 1 | z z z z 
        //

        double mij;
        for (i = MATRIX_SIZE - 1; i > 0; i--) {
            for (j = i - 1; j > -1; j--) {
                mij = s[j][i] / s[i][i];
                for (jj = j + 1; jj < 2 * MATRIX_SIZE; jj++) {
                    s[j][jj] -= mij * s[i][jj];
                }
            }
        }

        for (i = 0; i < MATRIX_SIZE; i++) {
            for (j = 0; j < MATRIX_SIZE; j++) {
            	res.values[i][j] = s[i][j + MATRIX_SIZE] / s[i][i];
            }
        }

        return res;
	}
	
	/**
	 * Print the matrix
	 * @return text of the matrix
	 */
	public String toString() {
		String res = new String();
		for (int i = 0; i < MATRIX_SIZE; i++) {
			res += "[" + values[i][0] + COMMA + values[i][1] + COMMA + values[i][2] + COMMA + values[i][MATRIX_SIZE - 1] + "]" + "\n";
		}
		return res;
	}
}

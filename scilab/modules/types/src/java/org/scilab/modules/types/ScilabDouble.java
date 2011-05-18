/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2009-2009 - DIGITEO - Bruno JOFRET
 *  Copyright (C) 2010-2010 - DIGITEO - Clément DAVID
 * 
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 * 
 */

package org.scilab.modules.types;

import java.util.Arrays;

/**
 * This class provides a representation on the Scilab Double datatype<br />
 * Note that double is the default datatype in Scilab
 * <br>
 * Example (real):<br />
 * <code>
 * double [][]a={{21.2, 22.0, 42.0, 39.0},{23.2, 24.0, 44.0, 40.0}};<br />
 * ScilabDouble aMatrix = new ScilabDouble(a);<br />
 * </code>
 * <br>
 * Example (complex):<br />
 * <code>
 * double [][]a={{21.2, 22.0, 42.0, 39.0},{23.2, 24.0, 44.0, 40.0}};<br />
 * double [][]aImg={{210.2, 220.0, 420.0, 390.0},{230.2, 240.0, 440.0, 400.0}};<br />

 * ScilabDouble aMatrix = new ScilabDouble(a, aImg);
 * </code>
 * @see org.scilab.modules.javasci.Scilab
 */
public class ScilabDouble implements ScilabType {

	private static final long serialVersionUID = 879624048944109684L;
	private double[][] realPart;
	private double[][] imaginaryPart;

	/**
	 * Default constructor
	 */
	public ScilabDouble() {
		realPart = null;
		imaginaryPart = null;
	}

	/**
	 * Constructor with a unique value.
	 * @param data the unique value
	 */
	public ScilabDouble(double data) {
		realPart = new double[1][1];
		realPart[0][0] = data;
		imaginaryPart = null;
	}

	/**
	 * Constructor with a unique complex value.
	 * 
	 * @param realData the real part
	 * @param imagData the complex part 
	 */
	public ScilabDouble(double realData, double imagData) {
		realPart = new double[1][1];
		realPart[0][0] = realData;
		imaginaryPart = new double[1][1];
		imaginaryPart[0][0] = imagData;
	}

	/**
	 * Constructor with a matrix of real data.
	 * 
	 * @param data the data
	 */
	public ScilabDouble(double[][] data) {
		realPart = data;
		imaginaryPart = null;
	}

	/**
	 * Constructor with a matrix of complex numbers 
	 * 
	 * @param realData the real part of the data
	 * @param imagData the imaginary part of the data
	 */
	public ScilabDouble(double[][] realData, double[][] imagData) {
		realPart = realData;
		imaginaryPart = imagData;
	}

	/**
	 * Check the emptiness of the associated data.
	 * @return true, if the associated data array is empty. 
	 */
	public boolean isEmpty() {
		return (realPart == null && imaginaryPart == null);
	}

	/**
	 * Check if the current data doesn't have an imaginary part.
	 * 
	 * @return true, if the data are real only.
	 */
	public boolean isReal() {
		return (imaginaryPart == null);
	}

	/**
	 * Get the real part of the data.
	 * 
	 * @return the real part.
	 */
	public double[][] getRealPart() {
		return realPart;
	}

	/**
	 * Set the real part of the data.
	 * 
	 * @param realPart the real part.
	 */
	public void setRealPart(double[][] realPart) {
		this.realPart = realPart;
	}

	/**
	 * Get the imaginary part of the data.
	 * 
	 * @return the imaginary part.
	 */
	public double[][] getImaginaryPart() {
		return imaginaryPart;
	}

	/**
	 * Set the imaginary part of the data.
	 * 
	 * @param imaginaryPart the imaginary part.
	 */
	public void setImaginaryPart(double[][] imaginaryPart) {
		this.imaginaryPart = imaginaryPart;
	}


	/**
	 * Get complex matrix as a serialized form
	 * 
	 * @return the serialized matrix with complex values
	 */
	public double[] getSerializedComplexMatrix() {
		int size = this.getHeight()*this.getWidth();
		double [] serializedComplexMatrix = new double[size*2];
		for (int i = 0; i < this.getHeight(); i++) {
			for (int j = 0; j < this.getWidth(); j++) {
				serializedComplexMatrix[j * this.getHeight() + i] = realPart[i][j];
				serializedComplexMatrix[size+j * this.getHeight() + i] = imaginaryPart[i][j];
			}
		}

		return serializedComplexMatrix;
	}


	/**
	 * @return the height of the data matrix
	 * @see org.scilab.modules.types.ScilabType#getHeight()
	 */
	@Override
	public int getHeight() {
		if (isEmpty()) {
			return 0;
		}
		return realPart.length;
	}

	/**
	 * @return the width of the data matrix
	 * @see org.scilab.modules.types.ScilabType#getWidth()
	 */
	@Override
	public int getWidth() {
		if (isEmpty() || realPart.length  == 0) {
			return 0;
		}
		
		return realPart[0].length;
	}

	/**
	 * @see org.scilab.modules.types.ScilabType#equals(Object)
	 */
	public boolean equals(Object obj) {
		if (obj instanceof ScilabDouble) {
			ScilabDouble sciDouble = ((ScilabDouble)obj);
			if (this.isReal() && sciDouble.isReal()) {
				return Arrays.deepEquals(this.getRealPart(), sciDouble.getRealPart());
			} else {
				/* Complex */
				return Arrays.deepEquals(this.getRealPart(), sciDouble.getRealPart()) && Arrays.deepEquals(this.getImaginaryPart(), sciDouble.getImaginaryPart());
			}
		} else {
			return false;
		}
	}


	/**
	 * Display the representation in the Scilab language of the type<br />
	 * Note that the representation can be copied/pasted straight into Scilab 
     *
	 * @return a Scilab-like String representation of the data.
	 * @see java.lang.Object#toString()
	 */
	@Override
	public String toString() {
		StringBuilder result = new StringBuilder();
		
		if (isEmpty()) {
			result.append("[]");
			return result.toString();
		}

		result.append("[");
		for (int i = 0; i < getHeight(); ++i) {
			for (int j = 0; j < getWidth(); ++j) {
				if (isReal()) {
					result.append(getRealPart()[i][j]);
				} else {
					result.append(getRealPart()[i][j] + " + " + getImaginaryPart()[i][j] + " * %i");
				}
				if (j != getWidth() - 1) {
					result.append(", ");
				}
			}
			if (i != getHeight() - 1) {
				result.append(" ; ");
			}
		}
		result.append("]");
		
		return result.toString();
	}

}

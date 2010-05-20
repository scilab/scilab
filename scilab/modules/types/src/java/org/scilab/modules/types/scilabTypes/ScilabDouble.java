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

package org.scilab.modules.types.scilabTypes;

/**
 * This class provides a wrapping on the Scilab Double datatype<br />
 * Note that double is the default datatype in Scilab
 */
public class ScilabDouble implements ScilabType {

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
	 * @return the height of the data matrix
	 * @see org.scilab.modules.types.scilabTypes.ScilabType#getHeight()
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
	 * @see org.scilab.modules.types.scilabTypes.ScilabType#getWidth()
	 */
	@Override
	public int getWidth() {
		if (isEmpty() || realPart.length  == 0) {
			return 0;
		}
		
		return realPart[0].length;
	}

	/**
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
					result.append(getRealPart()[i][j] + " + " + getImaginaryPart()[i][j] + " %i");
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

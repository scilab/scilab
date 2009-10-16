/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2009-2009 - DIGITEO - Bruno JOFRET
 * 
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 * 
 */

package org.scilab.modules.hdf5.scilabTypes;

public class ScilabDouble implements ScilabType {

	public ScilabDouble() {
		realPart = null;
		imaginaryPart = null;
	}

	public ScilabDouble(double data) {
		realPart = new double[1][1];
		realPart[0][0] = data;
		imaginaryPart = null;
	}

	public ScilabDouble(double realData, double imagData) {
		realPart = new double[1][1];
		realPart[0][0] = realData;
		imaginaryPart = new double[1][1];
		imaginaryPart[0][0] = imagData;
	}

	public ScilabDouble(double[][] data) {
		realPart = data;
		imaginaryPart = null;
	}

	public ScilabDouble(double[][] realData, double[][] imagData) {
		realPart = realData;
		imaginaryPart = imagData;
	}

	public boolean isEmpty() {
		return (realPart == null && imaginaryPart == null);
	}

	public boolean isReal() {
		return (imaginaryPart == null);
	}

	public double[][] getRealPart() {
		return realPart;
	}

	public void setRealPart(double[][] realPart) {
		this.realPart = realPart;
	}

	public double[][] getImaginaryPart() {
		return imaginaryPart;
	}

	public void setImaginaryPart(double[][] imaginaryPart) {
		this.imaginaryPart = imaginaryPart;
	}

	private double[][] realPart;
	private double[][] imaginaryPart;

	public int getHeight() {
		if (isEmpty()) {
			return 0;
		}
		return realPart.length;
	}

	public int getWidth() {
		if (isEmpty()|| realPart.length  == 0 ) {
			return 0;
		}
		
		return realPart[0].length;
	}

	public String toString() {
		StringBuffer result = new StringBuffer();
		if (isEmpty()) {
			result.append("[]");
			return result.toString();
		}

		result.append("[");
		for (int i = 0 ; i < getHeight() ; ++i) {
			for (int j = 0 ; j < getWidth() ; ++j) {
				if(isReal()) {
					result.append(getRealPart()[i][j]);
				}
				else {
					result.append(getRealPart()[i][j]+" + "+getImaginaryPart()[i][j]+" %i");
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

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

public class ScilabDouble {

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
}

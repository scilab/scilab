/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2007 - INRIA - Sylvestre LEDRU
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package javasci;

/**
 * Scilab Array of Complex Object
 * See SCI/modules/javasci/examples/others for some simple examples 
 * @author Allan CORNET - INRIA 2006
 * @author Sylvestre LEDRU - INRIA 2007
 */
public class SciComplexArray extends javasci.SciAbstractArray implements java.io.Serializable
{
/********************************************************************************************************/
 private double [] x; /* Real part */
 private double [] y; /* Imaginary part */

	/**
	 * Constructs a Scilab Complex Array from a other SciComplexArray
	 * @param name the name of the Scilab variable
	 * @param complexArray the SciComplexArray Array you want to copy
	 */

  public SciComplexArray(String name, SciComplexArray complexArray) {
    this.name = name;
    this.m = complexArray.getNumberOfRows();
    this.n = complexArray.getNumberOfCols();

	this.x = new double[m * n];
	this.x = complexArray.getRealPartData();
    
	this.y = new double[m * n];
	this.y = complexArray.getImaginaryPartData();
    
	Send();
 }

	/**
	 * Constructs a ScilabComplexArray
	 * All cells are initialized to 0
	 * @param name  the name of the Scilab Variable
	 * @param row number of rows
	 * @param col number of columns
	 */
  public SciComplexArray(String name, int row, int col) {
    this.m = row;
    this.n = col;
    this.x = new double[row * col];
    this.y = new double[row * col];
    this.name = name;
    
    if ((Scilab.TypeVar(name) == 1) &  // real or complex constant matrix.
         (getNumberOfRowsFromScilab(name) == row) & // has the same number of rows
         (getNumberOfColsFromScilab(name) == col)) // has the same number of columns
    {
		// load it from Scilab
		Get();

    } else {
        for (int i = 0; i < row * col; i++) {
    	  x[i] = 0;
    	  y[i] = 0;
        }
		// send it to scilab
        Send();
    }
  }
	/**
	 * Constructs a Scilab Boolean Complex  from two java double array
	 * @param name  the name of the Scilab Variable
	 * @param r number of rows
	 * @param c number of columns
	 * @param x the array of double with want to copy into the Real Part
	 * @param y the array of double with want to copy into the Imaginary Part
	 */
  public SciComplexArray(String name, int r, int c, double [] x, double [] y) {
    if ((r * c != x.length) && (r * c != y.length)) {
     throw new BadDataArgumentException("Bad Matrix call, size of third argument is wrong");
    }

	this.m = r;
	this.n = c;
    
	this.x = x;
	this.y = y;
    
	this.name = name;
    
	Send();
 }

	/**
	 * Should not be called (this method is only here for polymorphism reason)
	 * @return the array
	 * @deprecated
	 * @see #getRealPartData()
	 * @see #getImaginaryPartData()
	 * @throws NoSuchMethodException if this method is called (meaning less in the case of complex)
	 */
	public double[] getData() throws NoSuchMethodException {
		throw new NoSuchMethodException("Cannot call getData on a Complex Data. See getRealPartData() / getImaginaryPartData() .");
	}

	/**
	 * Return the Real part of the data
	 * @return the real part of the data 
	 */
 public String getName() {
	return name;
 }
 
 /**
  * returns the real data of the complex array
 * @return the real data
 */
public double[] getRealPartData() {
	Get();
	return x;
 }
	
/**
 * Get only ONE element from Scilab Matrix 
 * Get Real Part
 * @param indr row indice in scilab
 * @param indc column indice in scilab 
 * @return the Real Part
 */
 public native double GetRealPartElement(int indr, int indc);

 /**
 * Get only ONE element from Scilab Matrix 
 * Get Real Part
 * @param indr row indice in scilab
 * @param indc column indice in scilab 
 * @return the Imaginary Part 
 */

 public native double GetImaginaryPartElement(int indr, int indc);
 
	/**
	 * Return the Imaginary part of the data
	 * @return the Imaginary part of the data 
	 */
 public double[] getImaginaryPartData() {
	Get();
	return y;
 }
}
/********************************************************************************************************/

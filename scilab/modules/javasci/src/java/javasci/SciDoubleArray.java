/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2005 - INRIA - Allan CORNET
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
 * Scilab Array of double object
 * See SCI/modules/javasci/examples/others for some simple examples 
 * @author Allan CORNET - INRIA 2005
 * @author Sylvestre LEDRU - INRIA 2007
 */
public class SciDoubleArray extends javasci.SciAbstractArray implements java.io.Serializable  
{
	private double [] x;

	/**
	 * Constructs a Scilab Double Array from a other SciDoubleArray
	 * @param name the name of the Scilab variable
	 * @param doubleObj the SciDoubleArray you want to copy
	 */
	public SciDoubleArray(String name, SciDoubleArray doubleObj) {
		this.name = name;
		this.m = doubleObj.getNumberOfRows();
		this.n = doubleObj.getNumberOfCols();
		this.x = new double[m * n];
   
		this.x = doubleObj.getData();
		Send();
	}

	/**
	 * Constructs a Scilab Double Array 
	 * All cells are initialized to 0
	 * @param name  the name of the Scilab Variable 
	 * @param r number of rows
	 * @param c number of columns
	 */ 
	public SciDoubleArray(String name, int r, int c) {
		this.m = r;
		this.n = c;
		this.x = new double[ r * c ];
		this.name = name;

		if ((Scilab.TypeVar(name) == 1)
			 & (getNumberOfRowsFromScilab(name) == r) 
			 & (getNumberOfColsFromScilab(name) == c)) {
				Get();
			} else {
				for (int i = 0; i < r * c; i++) {
						x[i] = 0;
				}
				Send();
			}
	}
	
	/**
	 * Constructs a Scilab Double Array from a java double array
	 * @param name  the name of the Scilab Variable 
	 * @param r number of rows
	 * @param c number of columns
	 * @param x the array of double with want to copy into
	 */
	public SciDoubleArray(String name, int r, int c, double [] x) {
	if ((r * c) != x.length) {
				throw new BadDataArgumentException("Bad Matrix call, size of third argument is wrong");
			}
		this.m = r;
		this.n = c;
		this.x = x;
		this.name = name;
		Send();
	}

	/**
	 * Get only ONE element from Scilab Matrix 
	 * indr AND indc are indices in scilab 
	 * in Scilab A=[1,2;3,4];
	 * A(1,1)=1 
	 * A(2,2)=4 
	 * @param indr row indice 
	 * @param indc column indice 
	 * @return the double value at the position [indr, indc]
	 */
	public native double GetElement(int indr, int indc);

	/**
	 * Return the data
	 * @return the data
	 */
	public double[] getData() {
		Get();
		return x;
	}
}


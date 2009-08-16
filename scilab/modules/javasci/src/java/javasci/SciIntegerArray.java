/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
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
 * Scilab Array of int object
 * See SCI/modules/javasci/examples/others for some simple examples 
 * @author Allan CORNET - INRIA 2005
 * @author Sylvestre LEDRU - INRIA 2007
 */
public class SciIntegerArray extends javasci.SciAbstractArray implements java.io.Serializable  
{
	private int [] x;

	/**
	 * Constructs a Scilab Integer Array from a other SciIntegerArray
	 * @param name the name of the Scilab variable
	 * @param intObj the SciIntegerArray you want to copy
	 */
	public SciIntegerArray(String name, SciIntegerArray intObj) {
		this.name = name;
		this.m = intObj.getNumberOfRows();
		this.n = intObj.getNumberOfCols();
		this.x = new int[m * n];
   
		this.x = intObj.getData();
		Send();
	}

	/**
	 * Constructs a Scilab Integer Array 
	 * All cells are initialized to 0
	 * @param name  the name of the Scilab Variable 
	 * @param r number of rows
	 * @param c number of columns
	 */ 
	public SciIntegerArray(String name, int r, int c) {
		this.m = r;
		this.n = c;
		this.x = new int[ r * c ];
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
	 * Constructs a Scilab Integer Array from a java int array
	 * @param name  the name of the Scilab Variable 
	 * @param r number of rows
	 * @param c number of columns
	 * @param x the array of int with want to copy into
	 */
	public SciIntegerArray(String name, int r, int c, int [] x) {
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
	 * @return the int value at the position [indr, indc]
	 */
	public native int GetElement(int indr, int indc);

	/**
	 * Return the data
	 * @return the data
	 */
	public int[] getData() {
		Get();
		return x;
	}
}


/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2007 - INRIA - Sylvestre LEDRU
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
 * Scilab Array of String object
 * See SCI/modules/javasci/examples/others for some simple examples
 * @author Allan CORNET - INRIA 2006
 * @author Sylvestre LEDRU - INRIA 2007
 * @author Allan CORNET - DIGITEO 2009
 */
public class SciStringArray extends javasci.SciAbstractArray implements java.io.Serializable {
	/********************************************************************************************************/
	private String [] x;

	/**
	 * Constructs a Scilab String Array from a other SciStringArray
	 * @param name the name of the Scilab variable
	 * @param stringArrayObj the SciStringArray you want to copy
	 */
	public SciStringArray(String name, SciStringArray stringArrayObj) {
		this.name = name;
		this.m = stringArrayObj.getNumberOfRows();
		this.n = stringArrayObj.getNumberOfCols();
		this.x = new String[m * n];

		this.x = stringArrayObj.getData();
		Send();
	}

	/**
	 * Constructs a Scilab String Array 
	 * All cells are initialized to ""
	 * @param name  the name of the Scilab Variable 
	 * @param r number of rows
	 * @param c number of columns
	 */
	public SciStringArray(String name, int r, int c) {
		this.m = r;
		this.n = c;
		this.x = new String[r * c];
		this.name = name;
    
		if ((Scilab.TypeVar(name) == 10)
			& (getNumberOfRowsFromScilab(name) == c) 
			& (getNumberOfColsFromScilab(name) == r)) {
				Get();
			} else {
				for (int i = 0; i < (r * c); i++) {
						x[i] = "";
				}
				Send();
			}
	}

	/**
	 * Constructs a Scilab String Array from a java string array
	 * @param name  the name of the Scilab Variable 
	 * @param r number of rows
	 * @param c number of columns
	 * @param stringArray the array of string with want to copy into
	 */
	public SciStringArray(String name, int r, int c, String [] stringArray) {
		if ((r * c) != stringArray.length) {
				throw new BadDataArgumentException("Bad Matrix call, size of third argument is wrong");
			}
		this.m = r;
		this.n = c;
		this.x = stringArray;
		this.name = name;
		Send();
	}

	/**
	 * Return a specific element in the Array
	 * @param indr Row index
	 * @param indc Column index
	 * @return The element
	 */
	public native String GetElement(int indr, int indc);

	/**
	 * Return the data
	 * @return the data
	 */
	public String[] getData() {
		Get();
		return x;
	}
}
/********************************************************************************************************/

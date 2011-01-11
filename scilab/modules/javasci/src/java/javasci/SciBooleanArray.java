/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
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
 * Scilab Array of Boolean object
 * See SCI/modules/javasci/examples/others for some simple examples
 * @author Allan CORNET - INRIA 2007
 * @author Sylvestre LEDRU - INRIA 2007
 */
public class SciBooleanArray extends javasci.SciAbstractArray implements java.io.Serializable {

 private boolean [] x;

	/**
	 * Constructs a Scilab Boolean Array from a other SciBooleanArray
     * @deprecated
	 * @param name the name of the Scilab variable
	 * @param booleanArray the SciBooleanArray you want to copy
	 */
  public SciBooleanArray(String name, SciBooleanArray booleanArray) {
    this.name = name;
    this.m = booleanArray.getNumberOfRows();
    this.n = booleanArray.getNumberOfCols();
    this.x = new boolean[m * n];

    this.x = booleanArray.getData();
    Send();
  }

	/**
	 * Constructs a Scilab Boolean Array 
	 * All cells are initialized to false
     * @deprecated
	 * @param name the name of the Scilab Variable 
	 * @param r number of rows
	 * @param c number of columns
	 */
  public SciBooleanArray(String name, int r, int c) {
    this.m = r;
    this.n = c;
    this.x = new boolean[r * c];
    this.name = name;
    
    if ((Scilab.TypeVar(name) == 4) // boolean matrix
         & (getNumberOfRowsFromScilab(name) == r) // has the same number of rows
         & (getNumberOfColsFromScilab(name) == c)) // has the same number of columns
    {
		// load it from Scilab
		Get();
    } else {
		// Create it
        for (int i = 0; i < (r * c); i++) { 
        		x[i] = false;
        }
		// send it to scilab
        Send(); 
    }
  }

	/**
	 * Constructs a Scilab Boolean Array from a java boolean array
     * @deprecated
	 * @param name  the name of the Scilab Variable 
	 * @param r number of rows
	 * @param c number of columns
	 * @param x the array of boolean with want to copy into
	 */
  public SciBooleanArray(String name, int r, int c, boolean[] x) {
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
  * in Scilab A=[%t,%t;%t,%f];
  * A(1,1)=%t
  * A(2,2)=%f
  * @deprecated
  * @param indr row indice
  * @param indc column indice
  * @return the boolean value at the position [indr, indc]
  */
  public native boolean GetElement(int indr, int indc);


	/**
	 * Return the data
     * @deprecated
	 * @return the data
	 */
  public boolean[] getData() {
	  Get(); // connection to the Scilab Engine
   return x;
  }
}


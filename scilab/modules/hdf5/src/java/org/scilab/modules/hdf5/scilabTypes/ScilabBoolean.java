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

/**
 * This class provides a wrapping on the Scilab boolean datatype
 */
public class ScilabBoolean implements ScilabType {

	/* the boolean data */
    private boolean[][] data;

    /**
     * Create an empty object
	 */
    public ScilabBoolean() {
		data = null;
    }
    
    /**
     * Create an object from an array of array of boolean
     *
     * @param data the array of boolean
	 */
    public ScilabBoolean(boolean[][] data) {
		this.data = data;
    }
    
    /**
     * Create a scalar boolean from a boolean
     *
     * @param value the boolean
	 */
    public ScilabBoolean(boolean value) {
		this.data = new boolean[1][1];
		this.data[0][0] = value;
    }

    /**
     * Change the value with the provided data
     *
     * @param data array of boolean
	 */
    public void setData(boolean[][] data) {
		this.data = data;
    }

    /**
     * Return the data
     *
     * @return the array of array of boolean
	 */
    public boolean[][] getData() {
		return data;
    }

    /**
     * Return the height (number of element) of the stored data
     *
     * @return the height
	 */
    public int getHeight() {
		if (data == null) {
			return 0;
		}
		return data.length;
    }

    /**
     * Return the width (number of element) of the stored data
     *
     * @return the width
	 */
    public int getWidth() {
		if (data == null) {
			return 0;
		}
		return data[0].length;
    }

    /**
     * If it is empty / uninitialized
     *
     * @return true if empty
	 */
    public boolean isEmpty(){
    	return (data == null);
    }
    

    /**
     * Display the matrix
     *
     * @return the pretty print
	 */
	public String toString() {
		StringBuffer result = new StringBuffer();
		if (isEmpty()) {
			result.append("[]");
			return result.toString();
		}

		result.append("[");
		for (int i = 0 ; i < getHeight() ; ++i) {
			for (int j = 0 ; j < getWidth() ; ++j) {
				if (getData()[i][j] == true){
					result.append("%t");
				}else{
					result.append("%f");
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

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

package org.scilab.modules.types;

import java.util.Arrays;

/**
 * This class provides a representation on the Scilab String datatype
 * <br>
 * Example:<br />
 * <code>
 * String [][]a={{"This","is","my","string"},{"and","I want to", "compare"," them"}};<br />
 * ScilabString aMatrix = new ScilabString(a);
 * </code>
 * @see org.scilab.modules.javasci.Scilab
 */
public class ScilabString implements ScilabType {

	private static final long serialVersionUID = 359802519980180085L;

	private String[][] data;

	/**
	 * Default constructor
	 */
	public ScilabString() {
		data = null;
	}
	
	/**
	 * Constructor with data.
	 * 
	 * @param data the associated data.
	 */
	public ScilabString(String[][] data) {
		this.data = data;
	}
	
	/**
	 * Constructor with vector data.
	 * 
	 * @param data the column vector data 
	 */
	public ScilabString(String[] data) {
		if (data == null || data.length == 0) {
			this.data = new String[1][1];
			this.data[0][0] = "";
		} else {
			this.data = new String[1][data.length];
			for (int i = 0; i < data.length; i++) {
				this.data[0][i] = data[i];
			}
		}
	}
	
	/**
	 * Constructor with a unique value
	 * 
	 * @param string the value
	 */
	public ScilabString(String string) {
		if (string == null) {
			throw new IllegalArgumentException("string == null");
		}
		this.data = new String[1][1];
		this.data[0][0] = string;
	}

	/**
	 * Set the values.
	 * 
	 * @param data the values
	 */
	public void setData(String[][] data) {
		this.data = data;
	}

	/**
	 * @return the associated values
	 */
	public String[][] getData() {
		return data;
	}
	
	/**
	 * @return the height of the data matrix
	 * @see org.scilab.modules.types.ScilabType#getHeight()
	 */
	public int getHeight() {
		if (data == null) {
			return 0;
		}
		return data.length;
	}

	/**
	 * @return the width of the data matrix
	 * @see org.scilab.modules.types.ScilabType#getWidth()
	 */
	public int getWidth() {
		if (data == null) {
			return 0;
		}
		return data[0].length;
	}
	
	/**
	 * Check the emptiness of the associated data.
	 * @return true, if the associated data array is empty. 
	 */
	public boolean isEmpty() {
		return (data == null);
	}
	
	/**
	 * @see org.scilab.modules.types.ScilabType#equals(Object)
	 */
	public boolean equals(Object obj) {
		if (obj instanceof ScilabString) {
			return Arrays.deepEquals(this.getData(), ((ScilabString)obj).getData());
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
	public String toString() {
		StringBuffer result = new StringBuffer();
		if (isEmpty()) {
			result.append("[]");
			return result.toString();
		}

		result.append("[");
		for (int i = 0; i < getHeight(); ++i) {
			for (int j = 0; j < getWidth(); ++j) {

				result.append('"');
				result.append(getData()[i][j]);
				result.append('"');
				
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

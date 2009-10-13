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

public class ScilabBoolean implements ScilabType {

    public ScilabBoolean() {
	data = null;
    }
    
    public ScilabBoolean(boolean[][] data) {
	this.data = data;
    }
    
    public ScilabBoolean(boolean value) {
	this.data = new boolean[1][1];
	this.data[0][0] = value;
    }

    public void setData(boolean[][] data) {
	this.data = data;
    }

    public boolean[][] getData() {
	return data;
    }
    public int getHeight() {
	if (data == null) {
	    return 0;
	}
	return data.length;
    }

    public int getWidth() {
	if (data == null) {
	    return 0;
	}
	return data[0].length;
    }

    private boolean[][] data;

    public boolean isEmpty(){
    	return (data == null);
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

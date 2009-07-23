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

public class ScilabString {

    public ScilabString() {
	data = null;
    }
    
    public ScilabString(String[][] data) {
	this.data = data;
    }
    
    public ScilabString(String string) {
	this.data = new String[1][1];
	this.data[0][0] = string;
    }

    public void setData(String[][] data) {
	this.data = data;
    }

    public String[][] getData() {
	return data;
    }
    
    private String[][] data;
 
}

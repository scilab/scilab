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

import java.util.ArrayList;

public class ScilabMList extends ArrayList<ScilabType> implements ScilabType {
   
    public ScilabMList() {
	super();
    }
    
    public ScilabMList(String []types) {
	super();
	String [][] typesData = new String[1][types.length];
	typesData[0] = types;
	add(new ScilabString(typesData));
    }
    
	public int getHeight() {
		if (isEmpty()) {
			return 0;
		}
		return 1;
	}

	public int getWidth() {
		if (isEmpty()) {
			return 0;
		}
		return size();
	}
	

	public String toString() {
		
		StringBuffer result = new StringBuffer();
		if (isEmpty()) {
			result.append("mlist()");
			return result.toString();
		}

		result.append("mlist");
		for (int i = 0 ; i < size() ; i++){
			result.append(get(i).toString());
			if (i != size() - 1) {
				result.append(", ");
			}
			
		}
		result.append(")");
	
		return result.toString();
	}
}

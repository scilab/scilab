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

package org.scilab.modules.types.scilabTypes;

import java.util.ArrayList;
import java.util.Collection;

/**
 * This class provides a wrapping on the Scilab TList datatype
 */
public class ScilabTList extends ArrayList<ScilabType> implements ScilabType {

	/**
	 * Construct an empty tlist.
	 * 
	 * Note that the first element of this collection is the header used by
	 * Scilab to find each field type.
	 */
    public ScilabTList() {
		super();
    }
    
    /**
     * Construct a tlist with a specified header.
     *  
     * @param types type names of the fields.
     */
    public ScilabTList(String []types) {
		super();
		String [][] typesData = new String[1][types.length];
		typesData[0] = types;
		add(new ScilabString(typesData));
    }

	/**
	 * Construct a tlist containing the elements of the specified collection, in
	 * the order that they are returned by the specified collection's iterator.
	 * 
	 * @param types
	 *            type names of the fields.
	 * @param c
	 *            the collection whose elements are to be placed into this
	 *            tlist.
	 */
    public ScilabTList(String[] types, Collection< ? extends ScilabType> c) {
    	super(c.size() + 1);
    	
    	String[][] typesData = new String[1][types.length];
    	typesData[0] = types;
    	add(new ScilabString(typesData));
    	
    	addAll(c);
    }
    
    /**
     * @return 1 when there is data on the list, 0 otherwise.
     * @see org.scilab.modules.types.scilabTypes.ScilabType#getHeight()
     */
	public int getHeight() {
		if (isEmpty()) {
			return 0;
		}
		return 1;
	}

	/**
     * @return 1 when there is data on the list, 0 otherwise.
	 * @see org.scilab.modules.types.scilabTypes.ScilabType#getWidth()
	 */
	public int getWidth() {
		if (isEmpty()) {
			return 0;
		}
		return size();
	}
	
	/**
	 * @return the pretty-printed data
	 * @see java.util.AbstractCollection#toString()
	 */
	public String toString() {
		
		StringBuffer result = new StringBuffer();
		if (isEmpty()) {
			result.append("tlist()");
			return result.toString();
		}

		result.append("tlist(");
		for (int i = 0; i < size(); i++) {
			result.append(get(i).toString());
			if (i != size() - 1) {
				result.append(", ");
			}
			
		}
		result.append(")");
	
		return result.toString();
	}
}

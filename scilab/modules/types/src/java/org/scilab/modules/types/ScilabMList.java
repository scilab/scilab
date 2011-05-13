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

import java.util.ArrayList;
import java.util.Collection;

/**
 * This class provides a representation on the Scilab MList datatype
 * <br>
 * Example:<br />
 * <code>
 * ScilabMList data = new ScilabMList();<br />
 * data.add(new ScilabString("hello"));<br />
 * data.add(new ScilabDouble(2));<br />
 * </code>
 * @see org.scilab.modules.javasci.Scilab
 */
public class ScilabMList extends ArrayList<ScilabType> implements ScilabType {
   
	private static final long serialVersionUID = 3224510024213901841L;

	/**
	 * Construct an empty mlist.
	 * 
	 * Note that the first element of this collection is the header used by
	 * Scilab to find each field name.
	 */
	public ScilabMList() {
		super();
	}
	
	/**
	 * Construct an mlist with a specified header.
	 *  
	 * @param types field names used by the accessors.
	 */
	public ScilabMList(String []types) {
		super();
		String [][] typesData = new String[1][types.length];
		typesData[0] = types;
		add(new ScilabString(typesData));
	}
	
	/**
	 * Construct a tlist containing the elements of the specified collection, in
	 * the order that they are returned by the specified collection's iterator.
	 * 
	 * @param names
	 *			field names, used by the accessors.
	 * @param c
	 *			the collection whose elements are to be placed into this
	 *			mlist.
	 */
	public ScilabMList(String[] names, Collection< ? extends ScilabType> c) {
		super(c.size() + 1);
		
		String[][] namesData = new String[1][names.length];
		namesData[0] = names;
		add(new ScilabString(namesData));
		
		addAll(c);
	}
	
	/**
	 * @return 1 when there is data on the list, 0 otherwise.
	 * @see org.scilab.modules.types.ScilabType#getHeight()
	 */
	@Override
	public int getHeight() {
		if (isEmpty()) {
			return 0;
		}
		return 1;
	}

	/**
	 * @return 1 when there is data on the list, 0 otherwise.
	 * @see org.scilab.modules.types.ScilabType#getWidth()
	 */
	@Override
	public int getWidth() {
		if (isEmpty()) {
			return 0;
		}
		return size();
	}
	
	/**
	 * Display the representation in the Scilab language of the type<br />
	 * Note that the representation can be copied/pasted straight into Scilab 
     *
	 * @return the pretty-printed data 
	 * @see java.util.AbstractCollection#toString()
	 */
	@Override
	public String toString() {
		
		StringBuffer result = new StringBuffer();
		if (isEmpty()) {
			result.append("mlist()");
			return result.toString();
		}

		result.append("mlist(");
		for (int i = 0; i < size(); i++) {
			result.append(get(i));
			if (i != size() - 1) {
				result.append(", ");
			}
			
		}
		result.append(")");
	
		return result.toString();
	}
}

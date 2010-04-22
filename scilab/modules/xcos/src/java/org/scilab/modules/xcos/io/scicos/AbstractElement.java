/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clément DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.io.scicos;

import org.scilab.modules.types.scilabTypes.ScilabDouble;
import org.scilab.modules.types.scilabTypes.ScilabList;
import org.scilab.modules.types.scilabTypes.ScilabString;
import org.scilab.modules.types.scilabTypes.ScilabType;

/**
 * Root implementation for any Element.
 * 
 * This class provide some useful methods to manage {@link ScilabType} classes.
 * 
 * @param <T>
 *            the chosen implementation
 * 
 */
public abstract class AbstractElement<T> implements Element<T> {
	/**
	 * Default constructor
	 */
	protected AbstractElement() { }

	/**
	 * Default implementation is empty.
	 * 
	 * @see org.scilab.modules.xcos.io.scicos.Element#beforeEncode()
	 */
	@Override
	public void beforeEncode() { }
	
	/**
	 * Encode the instance and return a new element.
	 * 
	 * Provide a default implementation which call
	 * {@link #encode(Object, ScilabType)} with a null parameter.
	 * 
	 * @param from
	 *            the source instance
	 * @return the element parameter
	 * @see org.scilab.modules.xcos.io.scicos.Element#encode(java.lang.Object)
	 */
	@Override
	public ScilabType encode(T from) {
		return encode(from, null);
	};
	
	/**
	 * Default implementation is empty.
	 * 
	 * @see org.scilab.modules.xcos.io.scicos.Element#afterEncode()
	 */
	@Override
	public void afterEncode() { }
	
	/**
	 * Default implementation is empty.
	 * 
	 * @see org.scilab.modules.xcos.io.scicos.Element#beforeDecode()
	 */
	@Override
	public void beforeDecode() { }
	
	/**
	 * Default implementation is empty.
	 * 
	 * @see org.scilab.modules.xcos.io.scicos.Element#afterDecode()
	 */
	@Override
	public void afterDecode() { }
	
	/**
	 * Check the emptiness of a type.
	 * 
	 * @param object
	 *            the data to test for emptiness
	 * @return true, if the object is empty, false otherwise
	 */
	protected static boolean isEmptyField(ScilabType object) {
		final boolean doubleEmptiness = object instanceof ScilabDouble
				&& ((ScilabDouble) object).isEmpty();
		final boolean stringEmptiness = object instanceof ScilabString
			&& ((ScilabString) object).isEmpty();
		final boolean listEmptiness = object instanceof ScilabList
				&& ((ScilabList) object).isEmpty();
		
		return doubleEmptiness || stringEmptiness || listEmptiness;
	}
	
	/**
	 * Get an array of index, for the current index.
	 * @param index the index.
	 * @param isColumnDominant the index placement flag
	 * @return the column-row index.
	 */
	protected static int[] getIndexes(int index, boolean isColumnDominant) {
		int[] ret = {0, 0};
		
		if (isColumnDominant) {
			ret[0] = index;
		} else {
			ret[1] = index;
		}
		
		return ret;
	}
	
	/**
	 * Increment the indexes according to the isColumnDominant flag.
	 * 
	 * @param indexes the current indexes (length=2)
	 * @param isColumnDominant flag to specify the field to increment.
	 */
	protected static void incrementIndexes(int[] indexes, boolean isColumnDominant) {
		if (isColumnDominant) {
			indexes[0]++;
		} else {
			indexes[1]++;
		}
	}
}

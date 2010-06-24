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

import java.io.Serializable;

/**
 * This interface specifies what is a Scilab types
 */
public interface ScilabType extends Serializable, Cloneable {

    /**
     * Return the width (number of elements) of the stored data
     *
     * @return the width
	 */
	int getWidth();
	
    /**
     * Return the height (number of elements) of the stored data
     *
     * @return the height
	 */
	int getHeight();
	
	/**
	 * Check if the current instance is empty.
	 * 
	 * @return true, the there is no data; false otherwise.
	 */
	boolean isEmpty();
	
    /**
     * Display the data
     *
     * @return the pretty print
	 */
	String toString();
}

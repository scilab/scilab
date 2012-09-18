/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.gui;

public interface SwingViewObject {

	/**
	 * Set the UID
	 * @param id the UID
	 */
    public void setId(String id);
	
	/**
	 * Get the UID
	 * @return the UID
	 */
    public String getId();

    /**
     * Generic method to force Swing objects to update their own properties.
     * @param property - the name of the property to update.
     * @param value - the new value.
     */
    public void update(int property, Object value);
}

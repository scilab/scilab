/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Bruno JOFRET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */
package org.scilab.modules.gui;

public interface SwingViewObject {

    /**
     * Set the UID
     * @param id the UID
     */
    public void setId(Integer id);

    /**
     * Get the UID
     * @return the UID
     */
    public Integer getId();

    /**
     * Generic method to force Swing objects to update their own properties.
     * @param property - the name of the property to update.
     * @param value - the new value.
     */
    public void update(int property, Object value);
}

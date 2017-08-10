/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent Couvert
 * Copyright (C) 2007 - INRIA - Bruno JOFRET
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

package org.scilab.modules.gui.utils;

/**
 * Class used to store size (width and height) of objects in Scilab GUIs
 * @author Vincent COUVERT
 * @author Bruno JOFRET
 */
public class Size {

    private int height;
    private int width;

    /**
     * Constructor
     * @param width the width of the object
     * @param height the height of the object
     */
    public Size(int width, int height) {
        this.height = height;
        this.width = width;
    }

    /**
     * Gets the height of a Scilab GUI object
     * @return the height
     */
    public int getHeight() {
        return height;
    }
    /**
     * Sets the height of a Scilab GUI object
     * @param height the height to set
     */
    public void setHeight(int height) {
        this.height = height;
    }
    /**
     * Gets the width of a Scilab GUI object
     * @return the width
     */
    public int getWidth() {
        return width;
    }
    /**
     * Sets the width of a Scilab GUI object
     * @param width the width to set
     */
    public void setWidth(int width) {
        this.width = width;
    }

    /**
     * @return string representation of the object.
     */
    public String toString() {
        return "[" + getWidth() + ", " + getHeight() + "]";
    }
}

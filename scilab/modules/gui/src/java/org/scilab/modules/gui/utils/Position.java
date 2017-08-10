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
 * Class used to store position (X-coordinate and Y-coordinate) of objects in Scilab GUIs
 * @author Vincent COUVERT
 * @author Bruno JOFRET
 */
public class Position {

    private int x;
    private int y;

    /**
     * Constructor
     * @param x X-coordinate
     * @param y Y-coordinate
     */
    public Position(int x, int y) {
        this.x = x;
        this.y = y;
    }

    /**
     * Gets the X-coordinate of objects in Scilab GUIs
     * @return the X-coordinate of the object
     */
    public int getX() {
        return x;
    }

    /**
     * Sets the X-coordinate of objects in Scilab GUIs
     * @param x the X-coordinate to set
     */
    public void setX(int x) {
        this.x = x;
    }

    /**
     * Gets the Y-coordinate of objects in Scilab GUIs
     * @return the Y-coordinate of the object
     */
    public int getY() {
        return y;
    }

    /**
     * Sets the Y-coordinate of objects in Scilab GUIs
     * @param y the Y-coordinate to set
     */
    public void setY(int y) {
        this.y = y;
    }


}

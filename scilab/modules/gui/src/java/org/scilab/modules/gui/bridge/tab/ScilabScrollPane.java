/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - 2008 Digiteo Jean-Baptiste Silvy
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


package org.scilab.modules.gui.bridge.tab;

import java.awt.Container;


/**
 * Define a scrollPane that can be used wih Scilab
 * @author Jean-Baptiste Silvy
 */
public interface ScilabScrollPane {

    /**
     * @return [x,y,w,h] Array of size 4 containing the position
     *         width and height of the viewPort
     */
    int[] getViewingRegion();

    /**
     * Set the background of the scrollPane
     * @param red red channel of the color
     * @param green green channel
     * @param blue blue channel
     */
    void setBackground(double red, double green, double blue);

    /**
     * Move the viewport
     * @param xPos the x position to scroll to
     * @param yPos the y position to scroll to
     */
    void setViewPosition(int xPos, int yPos);

    /**
     * @return Container representation of this object
     */
    Container getAsContainer();

}

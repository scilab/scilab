/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
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

package org.scilab.modules.gui.canvas;

import java.awt.image.BufferedImage;

import javax.media.opengl.GL;

import org.scilab.modules.gui.dockable.Dockable;

/**
 * Interface for Scilab canvas in GUIs
 * @author Vincent COUVERT
 */
public interface Canvas extends Dockable {

    /**
     * Gets this Bridge component object
     * @return this Bridge component object
     */
    SimpleCanvas getAsSimpleCanvas();

    /**
     * Force the canvas to render itself immediately.
     */
    void display();

    /**
     * Get the GL pipeline used by the canvas
     * @return GL pipeline
     */
    GL getGL();


    /**
     * @param onOrOff set whether the swap buffer mode is on or off.
     */
    void setAutoSwapBufferMode(boolean onOrOff);

    /**
     * @return set whether the swap buffer mode is on or off.
     */
    boolean getAutoSwapBufferMode();

    /**
     * Force the canvas to render itself with synchronisation with its OpenGL process.
     */
    void repaint();

    /**
     * Set the background of the Canvas.
     * @param red red channel
     * @param green green channel
     * @param blue blue channel
     */
    void setBackgroundColor(double red, double green, double blue);

    /**
     * Disable the canvas befor closing
     */
    void close();

    /**
     * Convert to a BufferedImage
     * @return BufferedImage
     */
    BufferedImage dumpAsBufferedImage();

    /**
     * Set double buffer mode on or Off
     * @param useSingleBuffer if true use single buffer if false use double buffering
     */
    void setSingleBuffered(boolean useSingleBuffer);

}

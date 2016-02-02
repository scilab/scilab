/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Marouane BEN JELLOUL
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

import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

import javax.media.opengl.GL;
import java.awt.image.BufferedImage;

/**
 * Interface for SimpleCanvas the associated object to Scilab GUIs Canvas
 * @author Marouane BEN JELLOUL
 */
public interface SimpleCanvas {

    /**
     * Gets the size of an Canvas (width and height)
     * @return the size of the Canvas
     */
    Size getDims();

    /**
     * Sets the size of an Canvas (width and height)
     * @param newSize the size we want to set to the Canvas
     */
    void setDims(Size newSize);

    /**
     * Gets the position of an Canvas (X-coordinate and Y-corrdinate)
     * @return the position of the Canvas
     */
    Position getPosition();

    /**
     * Sets the position of an Canvas (X-coordinate and Y-corrdinate)
     * @param newPosition the position we want to set to the Canvas
     */
    void setPosition(Position newPosition);

    /**
     * Gets the visibility status of an Canvas
     * @return the visibility status of the Canvas (true if the Canvas is visible, false if not)
     */
    boolean isVisible();

    /**
     * Sets the visibility status of an Canvas
     * @param newVisibleState the visibility status we want to set for the Canvas
     * 			(true if the Canvas is visible, false if not)
     */
    void setVisible(boolean newVisibleState);

    /**
     * Draws an Canvas
     */
    void draw();

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
     * Disable the canvas before closing
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

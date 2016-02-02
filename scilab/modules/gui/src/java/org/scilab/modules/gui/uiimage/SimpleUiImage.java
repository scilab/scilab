/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Han DONG
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
package org.scilab.modules.gui.uiimage;

import org.scilab.modules.gui.text.SimpleText;

/**
 * Interface for ScilabUiImageBridge the associated object to Scilab GUIs Image
 * @author Han DONG
 */
public interface SimpleUiImage extends SimpleText {
    /**
     * Rotates an image
     * @param angle the value for the angle to rotate
     */
    void setRotate(double angle);

    /**
     * Shears an image
     * @param indices the double array for x, y values to shear
     */
    void setShear(double[] indices);

    /**
     * Scales an image
     * @param indices the double array for x, y values to scale
     */
    void setScale(double[] indices);
}

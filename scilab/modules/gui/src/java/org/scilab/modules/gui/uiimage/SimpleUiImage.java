/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Han DONG
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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

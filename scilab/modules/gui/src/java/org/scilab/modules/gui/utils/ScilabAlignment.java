/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent Couvert
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.gui.utils;

import javax.swing.SwingConstants;

/**
 * Alignment property values for Scilab Uicontrols
 * @author Vincent COUVERT
 */
public final class ScilabAlignment {

    /**
     * Left horizontal alignment
     */
    public static final String LEFT = "left";

    /**
     * Center horizontal alignment
     */
    public static final String CENTER = "center";

    /**
     * Right horizontal alignment
     */
    public static final String RIGHT = "right";

    /**
     * Top vertical alignment
     */
    public static final String TOP = "top";

    /**
     * Middle vertical alignment
     */
    public static final String MIDDLE = "middle";

    /**
     * Bottom vertical alignment
     */
    public static final String BOTTOM = "bottom";

    /**
     * Constructor
     */
    private ScilabAlignment() {
        /* Should not be used */
        throw new UnsupportedOperationException();
    }

    /**
     * Convert the Scilab string value for alignment to Swing value
     * @param alignment the Scilab value for alignment
     * @return the Swing value
     */
    public static int toSwingAlignment(String alignment) {
        int returnValue = SwingConstants.CENTER;

        if (alignment.equals(LEFT)) {
            returnValue = SwingConstants.LEFT;
        } else if (alignment.equals(RIGHT)) {
            returnValue = SwingConstants.RIGHT;
        } else if (alignment.equals(TOP)) {
            returnValue = SwingConstants.TOP;
        } else if (alignment.equals(BOTTOM)) {
            returnValue = SwingConstants.BOTTOM;
        }

        return returnValue;
    }
}

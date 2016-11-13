/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent Couvert
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

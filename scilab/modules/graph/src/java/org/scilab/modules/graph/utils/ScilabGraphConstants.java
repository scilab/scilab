/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
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

package org.scilab.modules.graph.utils;

import com.mxgraph.util.mxConstants;

/**
 * Define all the constants used on a Scilab graph
 */
public class ScilabGraphConstants extends mxConstants {

    /**
     * Defines the size of the arcs for rounded edges. Default is 10.
     */
    public static final double LINE_ARCSIZE = 200;

    /**
     * Defines the key for the centerArrow style.
     */
    public static final String STYLE_CENTERARROW = "centerArrow";

    /**
     * Defines the key for the centerSize style. The type of this value is
     * <code>float</code> and the value represents the size of the center
     * marker in pixels.
     */
    public static final String STYLE_CENTERSIZE = "centerSize";

    /**
     * Defines the value for spline edge shape
     */
    public static final String SHAPE_SPLINE = "spline";
    /**
     * Defines the value if we want the arrow to be centered
     */
    public static final String ARROW_POSITION_CENTER = "center";

    /**
     * Defines the key for flip image .
     */
    public static final String STYLE_FLIP = "flip";

    /**
     * Defines the key for mirror image .
     */
    public static final String STYLE_MIRROR = "mirror";

    /* HTML */
    /**
     * The html begin symbol
     */
    public static final String HTML_BEGIN = "<html>";
    /**
     * The html end symbol
     */
    public static final String HTML_END = "</html>";
    /**
     * The html new line symbol
     */
    public static final String HTML_NEWLINE = "<br>";

    /**
     * The html pre symbol begin
     */
    public static final String HTML_BEGIN_CODE = "<code>";

    /**
     * The html pre symbol end
     */
    public static final String HTML_END_CODE = "</code>";

    /* Events */
    /** Name of the edit event */
    public static final String EVENT_CHANGE_EDIT = "edit";

    /** This class is a static singleton, thus it must not be instantiated */
    protected ScilabGraphConstants() { }
}

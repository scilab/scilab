/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
    
    /* Events */
    /** Name of the edit event */
    public static final String EVENT_CHANGE_EDIT = "edit";
    
    /** This class is a static singleton, thus it must not be instantiated */
    protected ScilabGraphConstants() { }
}

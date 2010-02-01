/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clément DAVID
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
public class ScilabConstants extends mxConstants {
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
    
    /* Scilab */
    
    /**
     * The tmp directory
     */
    public static final String TMPDIR = "TMPDIR";
    
    /** This class is a static singleton, thus it must not be instantiated */
    protected ScilabConstants() { }
}

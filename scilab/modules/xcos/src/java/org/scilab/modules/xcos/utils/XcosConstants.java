package org.scilab.modules.xcos.utils;

import com.mxgraph.util.mxConstants;

/**
 * Contains all the constants used trough the source code.
 */
public class XcosConstants extends mxConstants {

    public static final int PALETTE_BLOCK_WIDTH = 100;
    public static final int PALETTE_BLOCK_HEIGHT = 100;

    public static final int PALETTE_HMARGIN = 5;
    public static final int PALETTE_VMARGIN = 5;

    /**
     * Defines the key for flip image .
     */
    public static final String STYLE_FLIP = "flip";

    /**
     * Defines the key for mirror image .
     */
    public static final String STYLE_MIRROR = "mirror";
    
    /* Events */
    public static final String EVENT_CHANGE_EDIT = "edit";   
    public static final String EVENT_BLOCK_UPDATED = "block";
    public static final String EVENT_CHANGE_OLD = "old";
    public static final String EVENT_CHANGE_NEW = "new";
    
    /** This class is a static singleton, thus it must not be instantiated */
    public XcosConstants() { }
}

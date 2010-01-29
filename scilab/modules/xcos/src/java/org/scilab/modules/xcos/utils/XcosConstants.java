package org.scilab.modules.xcos.utils;

import org.scilab.modules.graph.utils.ScilabConstants;

/**
 * Contains all the constants used trough the source code.
 */
public final class XcosConstants extends ScilabConstants {

    public static final int PALETTE_BLOCK_WIDTH = 100;
    public static final int PALETTE_BLOCK_HEIGHT = 100;

    public static final int PALETTE_HMARGIN = 5;
    public static final int PALETTE_VMARGIN = 5;
    
    /* Events */
    public static final String EVENT_BLOCK_UPDATED = "block";
    public static final String EVENT_CHANGE_OLD = "old";
    public static final String EVENT_CHANGE_NEW = "new";
    
    /** This class is a static singleton, thus it must not be instantiated */
    private XcosConstants() { }
}

/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clement David
 * Copyright (C) 2011-2015 - Scilab Enterprises - Clement DAVID
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

package org.scilab.modules.xcos.utils;

import org.scilab.modules.graph.utils.ScilabGraphConstants;

/**
 * Contains all the constants used trough the source code.
 */
public final class XcosConstants extends ScilabGraphConstants {

    /** Define the default block width on the palette */
    public static final int PALETTE_BLOCK_WIDTH = 100;
    /** Define the default block height on the palette */
    public static final int PALETTE_BLOCK_HEIGHT = 100;

    /** Define the default block height on the palette */
    public static final double PALETTE_BLOCK_ICON_RATIO = 1.5;

    /** Define the default block horizontal margin on the palette */
    public static final int PALETTE_HMARGIN = 5;
    /** Define the default block vertical margin on the palette */
    public static final int PALETTE_VMARGIN = 5;

    /** Define the maximum number of char that might be represented as style */
    public static final int MAX_CHAR_IN_STYLE = 37;

    /* Events */
    /**
     * When a block changed
     */
    public static final String EVENT_BLOCK_UPDATED = "block";
    /** Change event old name */
    public static final String EVENT_CHANGE_OLD = "old";
    /** Change event new name */
    public static final String EVENT_CHANGE_NEW = "new";

    /* SCI environment */
    /** Path from SCI or SCIHOME to the Xcos configuration directory */
    public static final String XCOS_ETC = "/modules/xcos/etc";

    /** This class is a static singleton, thus it must not be instantiated */
    private XcosConstants() {
    }
}

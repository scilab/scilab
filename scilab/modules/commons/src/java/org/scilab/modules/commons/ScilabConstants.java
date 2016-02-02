/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
 * Copyright (C) 2010 - DIGITEO - Sylvestre LEDRU
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

package org.scilab.modules.commons;

import java.io.File;

/**
 * Define all the constants used on a Scilab
 */
public class ScilabConstants {

    /* Scilab */

    /**
     * The tmp directory
     * This code used to be System.getenv("TMPDIR").
     * However, this was failing with Javasci to due architecture constraints
     * Javasci (Java) => Call_scilab (C) => Scilab engine (C)
     * => Java VM (graphics, xcos, etc).
     * In this case, the Java VM is started by Javasci.
     * Since, the TMPDIR env variable is set by Call_Scilab (the second step),
     * the environnement in the Java world has not TMPDIR
     */
    public static final File TMPDIR;

    /**
     * The SCI directory (Scilab root directory)
     */
    public static final File SCI;

    /**
     * The SCI configuration directory (Scilab home directory)
     */
    public static final File SCIHOME;

    /**
     * The user home directory
     */
    public static final String USERHOME = System.getProperty("user.home");

    /** Escape double quote symbol */
    public static final char QUOTE = '\"';

    /** 2 is for GUI **/
    public static int mode;

    /*
     * Static constructor
     */
    static {
        String value;

        value = ScilabCommons.getTMPDIR();
        if (value == null || value.isEmpty()) {
            value = System.getProperty("java.io.tmpdir");
        }
        TMPDIR = new File(value);

        value = System.getenv("SCI");
        SCI = new File(value).getAbsoluteFile();

        value = ScilabCommons.getSCIHOME();
        if (value == null || "".equals(value) || "empty_SCIHOME".equals(value)) {
            // Empty java value is TMPDIR
            SCIHOME = TMPDIR;
        } else {
            SCIHOME = new File(value);
        }
    }

    /** This class is a static singleton, thus it must not be instantiated */
    protected ScilabConstants() { }

    public static void setMode(int mode) {
        ScilabConstants.mode = mode;
    }

    public static boolean isGUI() {
        return ScilabConstants.mode == 2;
    }

}

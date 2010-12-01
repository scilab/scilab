/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clément DAVID
 * Copyright (C) 2010 - DIGITEO - Sylvestre LEDRU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.commons;

import java.io.File;

import org.scilab.modules.commons.ScilabCommons;

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
    public static final File TMPDIR = new File(ScilabCommons.getTMPDIR());
    
    /**
     * The SCI directory (Scilab root directory)
     */
    public static final File SCI = new File(System.getenv("SCI"));
    
    /**
     * The SCI configuration directory (Scilab home directory)
     */
    public static final File SCIHOME = new File(ScilabCommons.getSCIHOME());
    
    /** Escape double quote symbol */
    public static final char QUOTE = '\"';
    
    /** This class is a static singleton, thus it must not be instantiated */
    protected ScilabConstants() { }

}

/*--------------------------------------------------------------------------*/
/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 * Copyright (C) 2023-2026 - Dassault Systèmes S.E. - Vincent COUVERT
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

/*--------------------------------------------------------------------------*/
package org.scilab.modules.jvm;

import java.io.File;
import java.io.IOException;

/**
 * LibraryPath to overload java.library.path.
 */
public class LibraryPath {

    private static final String JAVALIBRARYPATH = "java.library.path";
    /**
     * Constructor
     */
    protected LibraryPath() {
        /*  indicate that the requested operation is not supported */
        throw new UnsupportedOperationException();
    }
    /*--------------------------------------------------------------------------*/
    /**
     * checks if pathToAdd already exists
     * @param currentpaths list of current paths
     * @param pathToAdd path to add
     * @return a boolean true if path already exists
     */
    private static boolean pathAlreadyExists(String currentpaths, String pathToAdd) {
        String[] paths = currentpaths.split("" + File.pathSeparatorChar);
        for (String libraryPath : paths) {
            if (libraryPath.equalsIgnoreCase(pathToAdd)) {
                return true;
            }
        }
        return false;
    }
    /*--------------------------------------------------------------------------*/
    /**
     * add a path to java.library.path
     * @param p path to add
     * @throws IOException return a exception
     */
    public static void addPath(final String p) throws IOException {
        if (!pathAlreadyExists(System.getProperty(JAVALIBRARYPATH), p)) {
            /* Update the java.library.path system property so that
             * the Scilab javalibrarypath() function keeps reporting the expected value.
             */
            /* The order matter here... see bug #4022 */
            String newLibPath = System.getProperty(JAVALIBRARYPATH) + File.pathSeparator + p;
            System.setProperty(JAVALIBRARYPATH, newLibPath);

            /* Register the directory so that ScilabClassLoader.findLibrary() can
             * resolve native libraries loaded through System.loadLibrary().
             */
            ScilabClassLoader.addNativeLibrarySearchPath(p);
        }
    }

    /*--------------------------------------------------------------------------*/
    /**
     * get the scilab java.library.path.
     * @return librarypath
     */
    public static String[] getLibraryPath() {
        String librarypath = System.getProperty(JAVALIBRARYPATH);
        String[] paths = librarypath.split("" + File.pathSeparatorChar);
        return paths;
    }
}
/*--------------------------------------------------------------------------*/

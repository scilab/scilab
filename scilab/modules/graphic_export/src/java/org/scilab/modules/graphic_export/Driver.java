/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.graphic_export;

/**
 * Class to map driver function
 *
 * @author Calixte DENIZET
 */
public class Driver {

    private static String driver;
    private static String filename;

    /**
     * Set the driver
     * @param driver the driver to use
     */
    public static boolean setDriver(String driver) {
        String d = driver.toLowerCase();
        if (!d.equals("x11") && !d.equals("rec") && Export.getType(d) == -1) {
            return false;
        }
        Driver.driver = driver;

        return true;
    }

    /**
     * Set the path
     * @param path the path
     */
    public static void setPath(String path) {
        Driver.filename = path;
    }

    /**
     * Export the current figure
     * @param uid the uid of the current figure
     */
    public static void end(String uid) {
        if (!driver.equalsIgnoreCase("X11") && !driver.equalsIgnoreCase("Rec")) {
            FileExporter.fileExport(uid, filename, driver, -1, 0);
        }
    }
}

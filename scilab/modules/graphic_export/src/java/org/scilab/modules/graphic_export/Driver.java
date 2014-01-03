/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.graphic_export;

import org.scilab.modules.localization.Messages;

/**
 * Class to map driver function
 *
 * @author Calixte DENIZET
 */
public class Driver {

    private static String driver = "Rec";
    private static String filename;

    /**
     * Set the driver
     * @param driver the driver to use
     */
    public static boolean setDriver(String driver) {
        String d = driver.toLowerCase();
        if (!d.equals("x11") && !d.equals("rec") && !d.equals("null") && Export.getType(d) == -1) {
            return false;
        }
        Driver.driver = driver;

        return true;
    }

    /**
     * Get the driver
     * @return the driver used
     */
    public static String getDriver() {
        return driver;
    }

    /**
     * Set the path
     * @param path the path
     */
    public static void setPath(String path) {
        Driver.filename = path;
    }

    public static boolean isImageRendering() {
        return !driver.equalsIgnoreCase("X11") && !driver.equalsIgnoreCase("Rec") && !driver.equals("null");
    }

    public static void setDefaultVisitor(int uid) {
        if (isImageRendering()) {
            Export.setVisitor(uid, Export.getType(driver), new ExportParams(-1.0f, 0, true));
        }
    }

    /**
     * Export the current figure
     * @param uid the uid of the current figure
     */
    public static String end(int uid) {
        if (filename == null) {
            return Messages.gettext("xinit must be called before xend.");
        }

        if (isImageRendering()) {
            String ret = FileExporter.headlessFileExport(uid, filename, driver, -1, 0);
            filename = null;

            return ret;
        }

        return "";
    }
}

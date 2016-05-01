/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
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

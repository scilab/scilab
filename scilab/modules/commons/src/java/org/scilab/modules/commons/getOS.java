/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.commons;


public class getOS {

    private static final String OSNAME = "os.name";
    
    /**
     * @return true if the os is windows, false otherwise
     */
    public static boolean isWindowsPlateform() {
        // get os name
        return System.getProperty(OSNAME).toLowerCase().contains("windows");
    }

    /**
     * Find the verion of windows used on the computer if one
     * @return negative value if the OS is not windows, the version of windows otherwise
     */
    public static double findWindowsVersion() {
        // default valu enot windows
        double windowsVersion = -1.0;

        if (isWindowsPlateform()) {
            // windows plateform
            return Double.valueOf(System.getProperty("os.version"));
        }

        return windowsVersion;
    }
}
/*---------------------------------------------------------------------------*/

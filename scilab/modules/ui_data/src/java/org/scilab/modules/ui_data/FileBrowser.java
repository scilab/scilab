/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Calixte DENIZET
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

package org.scilab.modules.ui_data;

import org.scilab.modules.ui_data.filebrowser.ScilabFileBrowser;

/**
 * Main class used in the GIWS interface
 * @author Calixte DENIZET
 */
public final class FileBrowser {

    /**
     * Open the file browser
     */
    public static void openFileBrowser() {
        ScilabFileBrowser.getFileBrowser();
    }

    /**
     * Set the base directory
     * @param baseDir the base directory
     */
    public static void setBaseDir(String baseDir) {
        ScilabFileBrowser.setBaseDir(baseDir);
    }
}

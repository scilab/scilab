/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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

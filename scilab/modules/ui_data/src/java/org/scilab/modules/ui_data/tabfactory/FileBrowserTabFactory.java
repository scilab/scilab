/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) DIGITEO - 2011 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.ui_data.tabfactory;

import org.scilab.modules.gui.bridge.tab.SwingScilabDockablePanel;
import org.scilab.modules.gui.tabfactory.AbstractScilabTabFactory;
import org.scilab.modules.ui_data.filebrowser.SwingScilabFileBrowser;

/**
 * The main Tab factory.
 * A component which needs to restore a Tab with a given uuid must register its factory.
 *
 * @author Calixte DENIZET
 */
public class FileBrowserTabFactory extends AbstractScilabTabFactory {

    public static final String APPLICATION = "FileBrowser";
    public static final String PACKAGE = "";
    public static final String CLASS = "org.scilab.modules.ui_data.tabfactory.FileBrowserTabFactory";

    private static FileBrowserTabFactory instance;

    /**
     * Default constructor
     */
    public FileBrowserTabFactory() {
        if (instance == null) {
            instance = this;
        }
    }

    /**
     * {@inheritDoc}
     */
    public SwingScilabDockablePanel getTab(String uuid) {
        return FileBrowserTab.getFileBrowserInstance();
    }

    /**
     * {@inheritDoc}
     */
    public String getPackage() {
        return PACKAGE;
    }

    /**
     * {@inheritDoc}
     */
    public String getClassName() {
        return CLASS;
    }

    /**
     * {@inheritDoc}
     */
    public String getApplication() {
        return APPLICATION;
    }

    /**
     * {@inheritDoc}
     */
    public boolean isAValidUUID(String uuid) {
        return SwingScilabFileBrowser.FILEBROWSERUUID.equals(uuid);
    }

    /**
     * @return an instance of this factory
     */
    public static FileBrowserTabFactory getInstance() {
        new FileBrowserTabFactory();

        return instance;
    }
}

/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Calixte DENIZET
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

package org.scilab.modules.scinotes.tabfactory;

import org.scilab.modules.gui.bridge.tab.SwingScilabDockablePanel;
import org.scilab.modules.gui.tabfactory.AbstractScilabTabFactory;
import org.scilab.modules.gui.tabfactory.ScilabTabFactory;

import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.utils.ConfigSciNotesManager;
import org.scilab.modules.scinotes.utils.NavigatorWindow;

/**
 * The main Tab factory.
 * A component which needs to restore a Tab with a given uuid must register its factory.
 *
 * @author Calixte DENIZET
 */
public class SearchInFilesTabFactory extends AbstractScilabTabFactory {

    public static final String APPLICATION = "SearchInFiles";
    public static final String PACKAGE = "SciNotes";
    public static final String CLASS = "org.scilab.modules.scinotes.tabfactory.SearchInFilesTabFactory";

    private static SearchInFilesTabFactory instance;

    /**
     * Default constructor
     */
    public SearchInFilesTabFactory() {
        if (instance == null) {
            instance = this;
        }
    }

    /**
     * {@inheritDoc}
     */
    public SwingScilabDockablePanel getTab(String uuid) {
        String editorUUID = ConfigSciNotesManager.getSearchInFilesState(uuid);
        SciNotes editor = (SciNotes) ScilabTabFactory.getInstance().getFromCache(editorUUID);

        return SearchInFilesTab.getSearchInFilesTabInstance(editor, uuid);
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
        return ConfigSciNotesManager.getSearchInFilesState(uuid) != null;
    }

    /**
     * @return an instance of this factory
     */
    public static SearchInFilesTabFactory getInstance() {
        new SearchInFilesTabFactory();

        return instance;
    }
}

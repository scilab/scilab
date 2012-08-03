/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.scinotes.tabfactory;

import org.scilab.modules.gui.bridge.tab.SwingScilabTab;
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
    public SwingScilabTab getTab(String uuid) {
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

/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
public class CodeNavigatorTabFactory extends AbstractScilabTabFactory {

    public static final String APPLICATION = "CodeNavigator";
    public static final String PACKAGE = "SciNotes";
    public static final String CLASS = "org.scilab.modules.scinotes.tabfactory.CodeNavigatorTabFactory";

    private static CodeNavigatorTabFactory instance;

    /**
     * Default constructor
     */
    public CodeNavigatorTabFactory() {
        if (instance == null) {
            instance = this;
        }
    }

    /**
     * {@inheritDoc}
     */
    public SwingScilabDockablePanel getTab(String uuid) {
        String editorUUID = ConfigSciNotesManager.getCodeNavigatorState(uuid);
        SciNotes editor = (SciNotes) ScilabTabFactory.getInstance().getFromCache(editorUUID);

        return CodeNavigatorTab.getCodeNavigatorInstance(editor, uuid);
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
        return ConfigSciNotesManager.getCodeNavigatorState(uuid) != null;
    }

    /**
     * @return an instance of this factory
     */
    public static CodeNavigatorTabFactory getInstance() {
        new CodeNavigatorTabFactory();

        return instance;
    }
}

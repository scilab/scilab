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

package org.scilab.modules.ui_data.tabfactory;

import org.scilab.modules.gui.bridge.tab.SwingScilabTab;
import org.scilab.modules.gui.tabfactory.AbstractScilabTabFactory;
import org.scilab.modules.ui_data.variablebrowser.SwingScilabVariableBrowser;

/**
 * The main Tab factory.
 * A component which needs to restore a Tab with a given uuid must register its factory.
 *
 * @author Calixte DENIZET
 */
public class VariableBrowserTabFactory extends AbstractScilabTabFactory {

    public static final String APPLICATION = "VariableBrowser";
    public static final String PACKAGE = "";
    public static final String CLASS = "org.scilab.modules.ui_data.tabfactory.VariableBrowserTabFactory";
    
    private static VariableBrowserTabFactory instance;

    /**
     * Default constructor
     */
    public VariableBrowserTabFactory() {
	if (instance == null) {
	    instance = this;
	}
    }

    /**
     * {@inheritDoc}
     */
    public SwingScilabTab getTab(String uuid) {
	return VariableBrowserTab.getVariableBrowserInstance();
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
	return SwingScilabVariableBrowser.VARBROWSERUUID.equals(uuid);
    }

    /**
     * @return an instance of this factory
     */
    public static VariableBrowserTabFactory getInstance() {
	new VariableBrowserTabFactory();
	
	return instance;
    }
}

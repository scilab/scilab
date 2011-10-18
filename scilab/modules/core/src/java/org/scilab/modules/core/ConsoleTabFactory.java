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

package org.scilab.modules.core;

import java.util.List;
import java.util.UUID;

import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.bridge.tab.SwingScilabTab;
import org.scilab.modules.gui.tabfactory.AbstractScilabTabFactory;
import org.scilab.modules.gui.tabfactory.ScilabTabFactory;
import org.scilab.modules.gui.utils.WindowsConfigurationManager;

/**
 * The main Tab factory.
 * A component which needs to restore a Tab with a given uuid must register its factory.
 *
 * @author Calixte DENIZET
 */
public class ConsoleTabFactory extends AbstractScilabTabFactory {

    public static final String APPLICATION = "Console";
    public static final String PACKAGE = "Console";
    public static final String CLASS = "org.scilab.modules.core.ConsoleTabFactory";
    public static final String NULLUUID = new UUID(0L, 0L).toString();

    private static ConsoleTabFactory instance;

    /**
     * Default constructor
     */
    public ConsoleTabFactory() { }

    public SwingScilabTab getTab(String uuid) {
	if (isAValidUUID(uuid)) {
	    return (SwingScilabTab) ConsoleTab.getConsoleTab(uuid).getAsSimpleTab();
	}
	return null;
    }

    public String getPackage() {
	return PACKAGE;
    }

    public String getClassName() {
	return CLASS;
    }

    public String getApplication() {
	return APPLICATION;
    }

    public boolean isAValidUUID(String uuid) {
	return uuid.equals(NULLUUID);
    }

    /**
     * @return an instance of this factory
     */
    public static ConsoleTabFactory getInstance() {
        if (instance == null) {
            instance = new ConsoleTabFactory();
        }

        return instance;
    }
}

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

package org.scilab.modules.gui.tabfactory;

import org.scilab.modules.gui.bridge.tab.SwingScilabTab;

/**
 * @author Calixte DENIZET
 */
public abstract class AbstractScilabTabFactory {

    /**
     * Abstract method which must implement a way to create a Tab with a given uuid
     * @param uuid the uuid of the Tab to restore
     * @return the corresponding Tab or null if this factory is unable to create the Tab (invalid uuid for example)
     */
    public abstract SwingScilabTab getTab(String uuid);

    /**
     * Abstract method which must implement a way to return true if the factory is able to build the Tab with given uuid
     * @param uuid the uuid of the Tab to restore
     * @return true if the factory is able to build the tab
     */
    public abstract boolean isAValidUUID(String uuid);

    /**
     * Abstract method which must implement a way to return the package containing the factory
     * which is able to build the Tab with a given uuid
     * @return the corresponding package or null if this factory is unable to create the Tab (invalid uuid for example)
     */
    public abstract String getPackage();

    /**
     * Abstract method which must implement a way to return the containing factory class
     * which is able to build the Tab with a given uuid
     * @return the corresponding package or null if this factory is unable to create the Tab (invalid uuid for example)
     */
    public abstract String getClassName();

    /**
     * Abstract method which must implement a way to return the application which produces the Tab
     * @return the corresponding application or null if this factory is unable to create the Tab (invalid uuid for example)
     */
    public abstract String getApplication();
}

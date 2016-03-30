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

package org.scilab.modules.gui.tabfactory;

import org.scilab.modules.gui.bridge.tab.SwingScilabDockablePanel;

/**
 * @author Calixte DENIZET
 */
public abstract class AbstractScilabTabFactory {

    /**
     * Abstract method which must implement a way to create a Tab with a given uuid
     * @param uuid the uuid of the Tab to restore
     * @return the corresponding Tab or null if this factory is unable to create the Tab (invalid uuid for example)
     */
    public abstract SwingScilabDockablePanel getTab(String uuid);

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

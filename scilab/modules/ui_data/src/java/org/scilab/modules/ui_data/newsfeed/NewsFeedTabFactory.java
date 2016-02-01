/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2015 - Scilab Enterprises
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

package org.scilab.modules.ui_data.newsfeed;

import org.scilab.modules.gui.bridge.tab.SwingScilabDockablePanel;
import org.scilab.modules.gui.tabfactory.AbstractScilabTabFactory;


/**
 * Factory to create news feed tabs For now, manages one tab only
 */
public class NewsFeedTabFactory extends AbstractScilabTabFactory {

    public static final String APPLICATION = "NewsFeed";
    public static final String PACKAGE = "";
    public static final String CLASS = "org.scilab.modules.ui_data.newsfeed.NewsFeedTabFactory";

    private static NewsFeedTabFactory instance = null;

    public NewsFeedTabFactory() {
        if (instance == null)
            instance = this;
    }

    public SwingScilabDockablePanel getTab(String uuid) {
        return NewsFeedTab.getInstance();
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
        return NewsFeedTab.NEWSFEED_UUID.equals(uuid);
    }

    public static NewsFeedTabFactory getInstance() {
        if (instance == null) {
            instance = new NewsFeedTabFactory();
        }
        return instance;
    }
}

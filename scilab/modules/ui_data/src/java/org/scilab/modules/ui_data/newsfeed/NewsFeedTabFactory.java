/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2015 - Scilab Enterprises
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/

package org.scilab.modules.ui_data.newsfeed;

import org.scilab.modules.gui.bridge.tab.SwingScilabDockablePanel;
import org.scilab.modules.gui.tabfactory.AbstractScilabTabFactory;
import org.scilab.modules.gui.tabfactory.ScilabTabFactory;

/**
 * Factory to create news feed tabs For now, manages one tab only
 */
public class NewsFeedTabFactory extends AbstractScilabTabFactory {

    public static final String APPLICATION = "NewsFeed";
    public static final String PACKAGE = "";
    public static final String CLASS = "org.scilab.modules.ui_data.newsfeed.NewsFeedTabFactory";

    static {
        ScilabTabFactory.getInstance().addTabFactory(NewsFeedTabFactory.getInstance());
    }

    private static NewsFeedTabFactory instance = null;

    public NewsFeedTabFactory() {
        if (instance == null) {
            instance = this;
        }
    }

    public SwingScilabDockablePanel getTab(String uuid) {
        NewsFeedTab newsFeedTab = NewsFeedTab.getInstance();
        ScilabTabFactory.getInstance().addToCache(newsFeedTab);
        return newsFeedTab;
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

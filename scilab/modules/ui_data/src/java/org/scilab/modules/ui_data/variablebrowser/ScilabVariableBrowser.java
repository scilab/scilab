/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Bruno JOFRET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.ui_data.variablebrowser;

import org.scilab.modules.gui.events.callback.ScilabCallBack;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.textbox.ScilabTextBox;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.utils.MenuBarBuilder;
import org.scilab.modules.gui.window.ScilabWindow;
import org.scilab.modules.localization.Messages;

public class ScilabVariableBrowser extends ScilabWindow implements VariableBrowser {

    private static final String MENUBARXMLFILE = System.getenv("SCI") + "/modules/ui_data/etc/variablebrowser_menubar.xml";

    private static VariableBrowser instance;

    private static Tab browserTab;   
    
    private SimpleVariableBrowser component;
    
    private ScilabVariableBrowser() {
	super();
	setTitle(Messages.gettext("Variable Browser"));
	browserTab = new SwingScilabVariableBrowser();
	browserTab.setVisible(true);
	browserTab.setCallback(ScilabCallBack
		.createCallback("org.scilab.modules.ui_data.BrowseVar.closeVariableBrowser", ScilabCallBack.JAVA_OUT_OF_XCLICK_AND_XGETMOUSE));
	MenuBar menubar = MenuBarBuilder.buildMenuBar(MENUBARXMLFILE);
	browserTab.addMenuBar(menubar);

	TextBox infobar = ScilabTextBox.createTextBox();
	browserTab.addInfoBar(infobar);
	
	addTab(browserTab);
    }

    public static VariableBrowser getVariableBrowser() {
	if (instance == null) {
	    instance = new ScilabVariableBrowser();
	}
	return instance;
    }
    
    public void close() {
	browserTab.close();
    }
}

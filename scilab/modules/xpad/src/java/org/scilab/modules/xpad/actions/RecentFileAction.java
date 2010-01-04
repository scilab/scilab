/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - INRIA - Allan SIMON
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.xpad.actions;
import java.io.File;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xpad.Xpad;
import org.scilab.modules.xpad.utils.ConfigXpadManager;

public class RecentFileAction extends DefaultAction {
	
	private File recentFile ;

	private RecentFileAction(Xpad editor , File f) {
		super(f.getName(), editor);
		recentFile = f ;
	}
	
	public void doAction() {
		ConfigXpadManager.saveToRecentOpenedFiles(recentFile.getPath());
		getEditor().readFile(recentFile);
		getEditor().setTitle(recentFile.getPath() + " - Xpad");
	};

	public static MenuItem createMenu(Xpad editor, File f) {
	    return createMenu(f.getName(), null, new RecentFileAction(editor, f), null);
	}
}
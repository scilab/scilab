/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Vincent COUVERT
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.actions;

import java.awt.event.ActionEvent;
import java.io.File;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.base.DefaultAction;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xcos.Xcos;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.utils.ConfigXcosManager;


/**
 * @author Vincent COUVERT
 *
 */
public final class RecentFileAction extends DefaultAction {

	private File recentFile;

	/**
	 * @param scilabGraph graph
	 * @param file new recent file
	 */
	private RecentFileAction(ScilabGraph scilabGraph , File file) {
		super(file.getName(), scilabGraph);
		recentFile = file;
	}
	
	/**
	 * @param e parameter
	 * @see org.scilab.modules.graph.actions.base.DefaultAction#actionPerformed(java.awt.event.ActionEvent)
	 */
	@Override
	public void actionPerformed(ActionEvent e) {
		ConfigXcosManager.saveToRecentOpenedFiles(recentFile.getPath());
		if (getGraph(null) == null) { // Called from palettes 
			Xcos.xcos(recentFile.getPath());
		} else {
			((XcosDiagram) getGraph(null)).openDiagramFromFile(recentFile.getPath());
		}
	};

	/**
	 * @param scilabGraph graph
	 * @param file new recent file
	 * @return menu item
	 */
	public static MenuItem createMenu(ScilabGraph scilabGraph, File file) {
	    return createMenu(file.getName(), null, new RecentFileAction(scilabGraph, file), null);
	}
}

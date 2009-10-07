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

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.DefaultAction;
import org.scilab.modules.gui.checkboxmenuitem.CheckBoxMenuItem;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.window.Window;
import org.scilab.modules.xcos.XcosDiagram;
import org.scilab.modules.xcos.utils.XcosMessages;

public class ViewViewportAction extends DefaultAction {
	
	private static CheckBoxMenuItem menu;

	private ViewViewportAction(ScilabGraph scilabGraph) {
		super(XcosMessages.VIEWPORT, scilabGraph);
	}

	public static MenuItem createMenu(ScilabGraph scilabGraph) {
		return createMenu(XcosMessages.VIEWPORT, null, new ViewViewportAction(scilabGraph), null);
	}
	
	public static CheckBoxMenuItem createCheckBoxMenu(ScilabGraph scilabGraph) {
		menu =  createCheckBoxMenu(XcosMessages.VIEWPORT, null, new ViewViewportAction(scilabGraph), null);
		return menu;
	}
	
	public void doAction() {
		Window viewPort = ((XcosDiagram) getGraph(null)).getViewPort();
		if (viewPort.isVisible()) {
			viewPort.setVisible(false);
			menu.setChecked(false);
		} else {
			viewPort.setVisible(true);
			menu.setChecked(true);
		}
		
	}
	
}

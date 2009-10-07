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
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.xcos.XcosDiagram;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Viewport window visibility managemet 
 * @author Vincent COUVERT
 */
public final class ViewViewportAction extends DefaultAction {
	
	private static final long serialVersionUID = 1L;

	private static CheckBoxMenuItem menu;

	/**
	 * Constructor
	 * @param scilabGraph associated Scilab Graph
	 */
	private ViewViewportAction(ScilabGraph scilabGraph) {
		super(XcosMessages.VIEWPORT, scilabGraph);
	}

	/**
	 * Create menu for the graph menu bar
	 * @param scilabGraph associated Scilab Graph
	 * @return the menu
	 */
	public static MenuItem createMenu(ScilabGraph scilabGraph) {
		return createMenu(XcosMessages.VIEWPORT, null, new ViewViewportAction(scilabGraph), null);
	}
	
	/**
	 * Create checkbox menu for the graph menu bar
	 * @param scilabGraph associated Scilab Graph
	 * @return the menu
	 */
	public static CheckBoxMenuItem createCheckBoxMenu(ScilabGraph scilabGraph) {
		menu =  createCheckBoxMenu(XcosMessages.VIEWPORT, null, new ViewViewportAction(scilabGraph), null);
		return menu;
	}
	
	/**
	 * Action !
	 * @see org.scilab.modules.graph.actions.DefaultAction#doAction()
	 */
	public void doAction() {
		Tab viewPort = ((XcosDiagram) getGraph(null)).getViewPort();

		boolean newState = !viewPort.isVisible();
		// Hide/Show parent window if the viewport is the only tab
		if (viewPort.getParentWindow().getNbDockedObjects() == 1) {
			viewPort.getParentWindow().setVisible(newState);
		}
		// Hide/Show viewport tab
		viewPort.setVisible(newState);
		// Check/Uncheck the menu
		menu.setChecked(newState);
	}
	
}

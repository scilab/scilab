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

import java.util.ArrayList;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.DefaultAction;
import org.scilab.modules.gui.checkboxmenuitem.CheckBoxMenuItem;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.xcos.Xcos;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Palette Tab visibility management 
 * @author Vincent COUVERT
 */
public final class ViewPaletteBrowserAction extends DefaultAction {
	
	private static final long serialVersionUID = 1L;

	private static ArrayList<CheckBoxMenuItem> menus = new ArrayList<CheckBoxMenuItem>();

	/**
	 * Constructor
	 * @param scilabGraph associated Scilab Graph
	 */
	private ViewPaletteBrowserAction(ScilabGraph scilabGraph) {
		super(XcosMessages.PALETTE_BROWSER, scilabGraph);
	}

	/**
	 * Create checkbox menu for the graph menu bar
	 * @param scilabGraph associated Scilab Graph
	 * @return the menu
	 */
	public static CheckBoxMenuItem createCheckBoxMenu(ScilabGraph scilabGraph) {
		CheckBoxMenuItem menu =  createCheckBoxMenu(XcosMessages.PALETTE_BROWSER, null, new ViewPaletteBrowserAction(scilabGraph), null);
		menus.add(menu);
		return menu;
	}
	
	/**
	 * Action !
	 * @see org.scilab.modules.graph.actions.DefaultAction#doAction()
	 */
	public void doAction() {
		setPalettesVisible(!Xcos.getPalettes().isVisible());
	}	
	
	/**
	 * Set the visibility of the palette Window and Tab
	 * Update all menus "View/Palette browser"
	 * @param status status to be set
	 */
	public static void setPalettesVisible(boolean status) {
		Tab palette = Xcos.getPalettes();

		// Hide/Show parent window if the viewport is the only tab
		if (palette.getParentWindow().getNbDockedObjects() == 1) {
			palette.getParentWindow().setVisible(status);
		}
		// Hide/Show viewport tab
		palette.setVisible(status);
		// Check/Uncheck the menu
		for (int i = (menus.size() - 1); i >= 0; i--) {
			menus.get(i).setChecked(status);
		}
	}
}

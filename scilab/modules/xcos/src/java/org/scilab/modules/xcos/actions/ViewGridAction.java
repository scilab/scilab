/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Vincent COUVERT
 * Copyright (C) 2010 - DIGITEO - Clément DAVID
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

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.base.DefaultAction;
import org.scilab.modules.gui.checkboxmenuitem.CheckBoxMenuItem;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Grid visibility management
 */
public final class ViewGridAction extends DefaultAction {
	public static final String NAME = XcosMessages.GRID;
	public static final String SMALL_ICON = "";
	public static final int MNEMONIC_KEY = 0;
	public static final int ACCELERATOR_KEY = 0;
	
	/**
	 * Constructor
	 * @param scilabGraph associated Scilab Graph
	 */
	public ViewGridAction(ScilabGraph scilabGraph) {
		super(scilabGraph);
	}

	/**
	 * Create checkbox menu for the graph menu bar
	 * @param scilabGraph associated Scilab Graph
	 * @return the menu
	 */
	public static CheckBoxMenuItem createCheckBoxMenu(ScilabGraph scilabGraph) {
		CheckBoxMenuItem  menu = createCheckBoxMenu(scilabGraph, ViewGridAction.class);
		menu.setChecked(true);
		return  menu;
	}
	
	/**
	 * @param e parameter
	 * @see org.scilab.modules.graph.actions.base.DefaultAction#actionPerformed(java.awt.event.ActionEvent)
	 */
	@Override
	public void actionPerformed(ActionEvent e) {
		((XcosDiagram) getGraph(null)).setGridVisible(!((XcosDiagram) getGraph(null)).getAsComponent().isGridVisible());
	}
}

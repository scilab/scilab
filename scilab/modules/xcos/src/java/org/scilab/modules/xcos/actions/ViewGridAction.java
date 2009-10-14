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
import org.scilab.modules.xcos.XcosDiagram;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Grid visibility management 
 * @author Vincent COUVERT
 */
public final class ViewGridAction extends DefaultAction {
	
	private static final long serialVersionUID = 1L;

	/**
	 * Constructor
	 * @param scilabGraph associated Scilab Graph
	 */
	private ViewGridAction(ScilabGraph scilabGraph) {
		super(XcosMessages.GRID, scilabGraph);
	}

	/**
	 * Create checkbox menu for the graph menu bar
	 * @param scilabGraph associated Scilab Graph
	 * @return the menu
	 */
	public static CheckBoxMenuItem createCheckBoxMenu(ScilabGraph scilabGraph) {
		return  createCheckBoxMenu(XcosMessages.GRID, null, new ViewGridAction(scilabGraph), null);
	}
	
	/**
	 * Action !
	 * @see org.scilab.modules.graph.actions.DefaultAction#doAction()
	 */
	public void doAction() {
		((XcosDiagram) getGraph(null)).setGridVisible(!((XcosDiagram) getGraph(null)).getAsComponent().isGridVisible());
	}
}

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

package org.scilab.modules.xcos.palette.actions;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.DefaultAction;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Generate a new palette where user can edit it's own blocks and save them as palette.
 * 
 * TODO: doAction for this class 
 */
public final class NewPaletteAction extends DefaultAction {

	/**
	 * Default constructor
	 * @param scilabGraph associated graph
	 */
	private NewPaletteAction(ScilabGraph scilabGraph) {
		super(XcosMessages.NEW_PALETTE, scilabGraph);
	}

	/**
	 * Create the menu
	 * @param scilabGraph the associated graph 
	 * @return the menu associated whith this action
	 */
	public static MenuItem createMenu(ScilabGraph scilabGraph) {
		return createMenu(XcosMessages.NEW_PALETTE, null, new NewPaletteAction(scilabGraph), null);
	}
}

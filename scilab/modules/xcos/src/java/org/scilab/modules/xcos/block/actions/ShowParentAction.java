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

package org.scilab.modules.xcos.block.actions;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.DefaultAction;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xcos.graph.SuperBlockDiagram;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Block flip handling
 * @author Antoine ELIAS
 */
public class ShowParentAction extends DefaultAction {

	private static final long serialVersionUID = 1L;

	/**
	 * Constructor
	 * @param scilabGraph associated diagram
	 */
	public ShowParentAction(ScilabGraph scilabGraph) {
		super(XcosMessages.SHOW_PARENT, scilabGraph);
	}

	/**
	 * Menu added to the menubar
	 * @param scilabGraph associated diagram
	 * @return the menu
	 */
	public static MenuItem createMenu(ScilabGraph scilabGraph) {
		return createMenu(XcosMessages.SHOW_PARENT, null, new ShowParentAction(scilabGraph), null);
	}

	/**
	 * Action !!
	 * @see org.scilab.modules.graph.actions.DefaultAction#doAction()
	 */
	public void doAction() {
	    if(getGraph(null) instanceof SuperBlockDiagram) {
		SuperBlockDiagram diagram =  (SuperBlockDiagram) getGraph(null);
		diagram.getContainer().getParentDiagram().setVisible(true);
	    }
	}
}

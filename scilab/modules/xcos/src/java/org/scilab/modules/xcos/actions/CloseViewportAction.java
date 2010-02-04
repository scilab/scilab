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

import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.base.DefaultAction;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Close action for viewport tab
 */
public class CloseViewportAction extends DefaultAction {
	public static final String NAME = XcosMessages.CLOSE;
	public static final String SMALL_ICON = "";
	public static final int MNEMONIC_KEY = KeyEvent.VK_W;
	public static final int ACCELERATOR_KEY = Toolkit.getDefaultToolkit().getMenuShortcutKeyMask();
	
	/**
	 * Constructor
	 * @param scilabGraph associated Scilab Graph
	 */
	public CloseViewportAction(ScilabGraph scilabGraph) {
		super(scilabGraph);
	}

	/**
	 * Create the close menu for the menu bar 
	 * @param scilabGraph associated Scilab Graph
	 * @return the menu
	 */
	public static MenuItem createMenu(ScilabGraph scilabGraph) {
		return createMenu(scilabGraph, CloseViewportAction.class);
	}

	/**
	 * @param e parameter
	 * @see org.scilab.modules.graph.actions.base.DefaultAction#actionPerformed(java.awt.event.ActionEvent)
	 */
	@Override
	public void actionPerformed(ActionEvent e) {
		((XcosDiagram) getGraph(null)).setViewPortVisible(false);
	}
}

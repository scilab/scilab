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

import java.awt.event.KeyEvent;
import java.awt.Toolkit;

import javax.swing.KeyStroke;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.DefaultAction;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xcos.XcosDiagram;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Diagram closing management
 * @author Vincent COUVERT
 */
public class CloseAction extends DefaultAction {

	private static final long serialVersionUID = 1L;

	/**
	 * Constructor
	 * @param scilabGraph associated diagram
	 */
	public CloseAction(ScilabGraph scilabGraph) {
		super(XcosMessages.CLOSE, scilabGraph);
	}

	/**
	 * Menu for diagram menubar
	 * @param scilabGraph associated diagram
	 * @return the menu
	 */
	public static MenuItem createMenu(ScilabGraph scilabGraph) {
		return createMenu(XcosMessages.CLOSE, null, new CloseAction(scilabGraph),
				KeyStroke.getKeyStroke(KeyEvent.VK_W, Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()));
	}

	/**
	 * Action !!
	 * @see org.scilab.modules.graph.actions.DefaultAction#doAction()
	 */
	public void doAction() {
		((XcosDiagram) getGraph(null)).closeDiagram();
	}

}

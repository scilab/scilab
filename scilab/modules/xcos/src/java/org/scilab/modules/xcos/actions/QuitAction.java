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
import org.scilab.modules.xcos.Xcos;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Management of exit of Xcos
 * @author Vincent COUVERT
 */
public final class QuitAction extends DefaultAction {

	private static final long serialVersionUID = 1L;

	/**
	 * Constructor
	 * @param scilabGraph associated Xcos diagram
	 */
	private QuitAction(ScilabGraph scilabGraph) {
		super(XcosMessages.QUIT, scilabGraph);
	}

	/**
	 * Create menu for the Scilab Graph menubar
	 * @param scilabGraph associated Xcos diagram
	 * @return the menu
	 */
	public static MenuItem createMenu(ScilabGraph scilabGraph) {
		return createMenu(XcosMessages.QUIT, null, new QuitAction(scilabGraph),
				KeyStroke.getKeyStroke(KeyEvent.VK_Q, Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()));
	}
	
	/**
	 * Associated action
	 * @see org.scilab.modules.graph.actions.DefaultAction#doAction()
	 */
	public void doAction() {
		Xcos.closeSession();
	}
}

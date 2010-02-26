/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
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

package org.scilab.modules.graph.actions;

import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;

import javax.swing.TransferHandler;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.base.DefaultAction;
import org.scilab.modules.graph.utils.ScilabGraphMessages;
import org.scilab.modules.gui.menuitem.MenuItem;

/**
 * Paste manager
 * 
 * This action is enabled by the {@link CutAction} and {@link CopyAction}.
 */
public final class PasteAction extends DefaultAction {
	public static final String NAME = ScilabGraphMessages.PASTE;
	public static final String SMALL_ICON = "edit-paste.png";
	public static final int MNEMONIC_KEY = KeyEvent.VK_V;
	public static final int ACCELERATOR_KEY = Toolkit.getDefaultToolkit().getMenuShortcutKeyMask();
	
	/**
	 * Constructor
	 * @param scilabGraph corresponding Scilab Graph
	 */
	public PasteAction(ScilabGraph scilabGraph) {
		super(scilabGraph);
		setEnabled(false);
	}

	/**
	 * Create a menu for a graph menubar
	 * @param scilabGraph corresponding Scilab Graph
	 * @return the menu
	 */
	public static MenuItem pasteMenu(ScilabGraph scilabGraph) {
		return createMenu(scilabGraph, PasteAction.class);
	}

	/**
	 * Action associated
	 * @param e the event
	 * @see org.scilab.modules.gui.events.callback.CallBack#actionPerformed(java.awt.event.ActionEvent)
	 */
	public void actionPerformed(ActionEvent e) {
		TransferHandler.getPasteAction().actionPerformed(new ActionEvent(getGraph(e).getAsComponent(),
				e.getID(), e.getActionCommand()));
	}
}

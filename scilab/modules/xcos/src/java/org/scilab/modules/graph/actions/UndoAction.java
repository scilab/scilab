/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2009 - DIGITEO - Vincent COUVERT
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.graph.actions;

import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;
import java.awt.Toolkit;

import javax.swing.KeyStroke;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.utils.ScilabGraphMessages;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.pushbutton.PushButton;

/**
 * Undo manager
 * @author Bruno JOFFRET
 */
public class UndoAction extends DefaultAction {

	private static final long serialVersionUID = 1L;

	/**
	 * Constructor
	 * @param scilabgraph corresponding Scilab Graph
	 */
	public UndoAction(ScilabGraph scilabgraph) {
		super(ScilabGraphMessages.UNDO, scilabgraph);
	}

	/**
	 * Create a button for a graph toolbar
	 * @param scilabGraph corresponding Scilab Graph
	 * @return the button
	 */
	public static PushButton undoButton(ScilabGraph scilabGraph) {
		return createButton(ScilabGraphMessages.UNDO, "edit-undo.png", new UndoAction(scilabGraph));
	}

	/**
	 * Create a menu for a graph menubar
	 * @param scilabGraph corresponding Scilab Graph
	 * @return the menu
	 */
	public static MenuItem undoMenu(ScilabGraph scilabGraph) {
		return createMenu(ScilabGraphMessages.UNDO, null, new UndoAction(scilabGraph), 
				KeyStroke.getKeyStroke(KeyEvent.VK_Z, Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()));
	}

	/**
	 * Action associated
	 * @param e the event
	 * @see org.scilab.modules.gui.events.callback.CallBack#actionPerformed(java.awt.event.ActionEvent)
	 */
	public void actionPerformed(ActionEvent e) {
		getGraph(e).undo();
	}

}

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

import java.awt.Toolkit;
import java.awt.event.KeyEvent;

import javax.swing.KeyStroke;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.DefaultAction;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.xcos.Xcos;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * New Diagram creation
 * @author Vincent COUVERT
 */
public final class NewDiagramAction extends DefaultAction {

	private static final long serialVersionUID = 1L;

	/**
	 * Constructor
	 * @param scilabGraph associated Scilab Graph
	 */
	private NewDiagramAction(ScilabGraph scilabGraph) {
		super(XcosMessages.NEW_DIAGRAM, scilabGraph);
	}

	/**
	 * Create a menu item for the graph menubar
	 * @param scilabGraph associated Scilab Graph
	 * @return the menu item
	 */
	public static MenuItem createMenu(ScilabGraph scilabGraph) {
		return createMenu(XcosMessages.NEW_DIAGRAM, null, new NewDiagramAction(scilabGraph),
				KeyStroke.getKeyStroke(KeyEvent.VK_N, Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()));
	}

	/**
	 * Create a button for the graph toolbar
	 * @param scilabGraph associated Scilab Graph
	 * @return the button
	 */
	public static PushButton createButton(ScilabGraph scilabGraph) {
		return createButton(XcosMessages.NEW_DIAGRAM, "document-new.png", new NewDiagramAction(scilabGraph));
	}
	
	/**
	 * Action !!
	 * @see org.scilab.modules.graph.actions.DefaultAction#doAction()
	 */
	public void doAction() {
		Xcos.createEmptyDiagram();
	}

}

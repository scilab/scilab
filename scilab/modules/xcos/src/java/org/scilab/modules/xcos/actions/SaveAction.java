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
import org.scilab.modules.xcos.XcosDiagram;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Management of "Diagram Saving"
 * @author Vincent COUVERT
 */
public final class SaveAction extends DefaultAction {

	private static final long serialVersionUID = 455320507172108275L;

	/**
	 * Constructor
	 * @param scilabGraph associated Xcos diagram
	 */
	private SaveAction(ScilabGraph scilabGraph) {
		super(XcosMessages.SAVE, scilabGraph);
	}

	/**
	 * Create menu for saving
	 * @param scilabGraph associated Xcos diagram
	 * @return the menu
	 */
	public static MenuItem createMenu(ScilabGraph scilabGraph) {
		return createMenu(XcosMessages.SAVE, null, new SaveAction(scilabGraph),
				KeyStroke.getKeyStroke(KeyEvent.VK_S, Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()));
	}

	/**
	 * Create toolbar button for saving
	 * @param scilabGraph associated Xcos diagram
	 * @return the button
	 */
	public static PushButton createButton(ScilabGraph scilabGraph) {
		return createButton(XcosMessages.SAVE, "document-save.png", new SaveAction(scilabGraph));
	}
	
	/**
	 * Associated action
	 * @see org.scilab.modules.graph.actions.DefaultAction#doAction()
	 */
	public void doAction() {
		((XcosDiagram) getGraph(null)).saveDiagram();
	}

}

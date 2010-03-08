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
import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.base.DefaultAction;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.xcos.XcosTab;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Management of "Diagram Saving"
 * @author Vincent COUVERT
 */
public final class SaveAsAction extends DefaultAction {
	/** Name of the action */
	public static final String NAME = XcosMessages.SAVE_AS;
	/** Icon name of the action */
	public static final String SMALL_ICON = "document-save-as.png";
	/** Mnemonic key of the action */
	public static final int MNEMONIC_KEY = KeyEvent.VK_S;
	/** Accelerator key for the action */
	public static final int ACCELERATOR_KEY = KeyEvent.SHIFT_DOWN_MASK
			| Toolkit.getDefaultToolkit().getMenuShortcutKeyMask();

	/**
	 * Constructor
	 * @param scilabGraph associated Xcos diagram
	 */
	public SaveAsAction(ScilabGraph scilabGraph) {
		super(scilabGraph);
	}

	/**
	 * Create menu for saving
	 * @param scilabGraph associated Xcos diagram
	 * @return the menu
	 */
	public static MenuItem createMenu(ScilabGraph scilabGraph) {
		return createMenu(scilabGraph, SaveAsAction.class);
	}

	/**
	 * Create toolbar button for saving
	 * @param scilabGraph associated Xcos diagram
	 * @return the button
	 */
	public static PushButton createButton(ScilabGraph scilabGraph) {
		return createButton(scilabGraph, SaveAsAction.class);
	}

	/**
	 * @param e parameter
	 * @see org.scilab.modules.graph.actions.base.DefaultAction#actionPerformed(java.awt.event.ActionEvent)
	 */
	@Override
	public void actionPerformed(ActionEvent e) {
		if (((XcosDiagram) getGraph(null)).saveDiagramAs(null)) {
			((XcosDiagram) getGraph(null)).setModified(false);
			XcosTab.updateRecentOpenedFilesMenu(((XcosDiagram) getGraph(null)));
		}
		
	}
}

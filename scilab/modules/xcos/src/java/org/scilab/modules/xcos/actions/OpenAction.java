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
import org.scilab.modules.gui.bridge.filechooser.SwingScilabFileChooser;
import org.scilab.modules.gui.filechooser.FileChooser;
import org.scilab.modules.gui.filechooser.ScilabFileChooser;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.xcos.Xcos;
import org.scilab.modules.xcos.XcosDiagram;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * File opening management
 * @author Vincent COUVERT
 */
public final class OpenAction extends DefaultAction {

	private static final long serialVersionUID = 1L;

	/**
	 * Constructor
	 * @param scilabGraph associated Scilab Graph
	 */
	private OpenAction(ScilabGraph scilabGraph) {
		super(XcosMessages.OPEN, scilabGraph);
	}

	/**
	 * Create a menu to add in Scilab Graph menu bar
	 * @param scilabGraph associated Scilab Graph
	 * @return the menu
	 */
	public static MenuItem createMenu(ScilabGraph scilabGraph) {
		return createMenu(XcosMessages.OPEN, null, new OpenAction(scilabGraph),
				KeyStroke.getKeyStroke(KeyEvent.VK_O, Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()));
	}

	/**
	 * Create a button to add in Scilab Graph tool bar
	 * @param scilabGraph associated Scilab Graph
	 * @return the button
	 */
	public static PushButton createButton(ScilabGraph scilabGraph) {
		return createButton(XcosMessages.OPEN, "document-open.png", new OpenAction(scilabGraph));
	}

	/**
	 * Open file action
	 * @see org.scilab.modules.graph.actions.DefaultAction#doAction()
	 */
	public void doAction() {
		FileChooser fc = ScilabFileChooser.createFileChooser();

		/* Standard files */
		String[] mask = new String[]{"*.cos*", "*.xcos"};
		((SwingScilabFileChooser) fc.getAsSimpleFileChooser()).addMask(mask , null);
		
		fc.setMultipleSelection(false);
		fc.displayAndWait();

		if (fc.getSelection() == null || fc.getSelection().length == 0 || fc.getSelection()[0].equals("")) {
			return;
		}
		if (getGraph(null) == null) { // Called from palettes 
			Xcos.xcos(fc.getSelection()[0]);
		} else {
			((XcosDiagram) getGraph(null)).openDiagramFromFile(fc.getSelection()[0]);
		}
	}
}

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

import javax.swing.KeyStroke;
import javax.swing.TransferHandler;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.utils.ScilabGraphMessages;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.pushbutton.PushButton;

public class PasteAction extends DefaultAction {

	public PasteAction(ScilabGraph scilabGraph) {
		super(ScilabGraphMessages.PASTE, scilabGraph);
	}

	public static PushButton pasteButton(ScilabGraph scilabGraph) {
		return createButton(ScilabGraphMessages.PASTE, "edit-paste.png", new PasteAction(scilabGraph));
	}

	public static MenuItem pasteMenu(ScilabGraph scilabGraph) {
		return createMenu(ScilabGraphMessages.PASTE, "edit-paste.png", new PasteAction(scilabGraph), KeyStroke.getKeyStroke(KeyEvent.VK_V, ActionEvent.CTRL_MASK));
	}

	public void actionPerformed(ActionEvent e) {
		TransferHandler.getPasteAction().actionPerformed(new ActionEvent(getGraph(e).getAsComponent(),
				e.getID(), e.getActionCommand()));
	}
}

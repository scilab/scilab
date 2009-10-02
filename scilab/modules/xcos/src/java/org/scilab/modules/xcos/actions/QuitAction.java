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

import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;

import javax.swing.KeyStroke;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.DefaultAction;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.utils.UIElementMapper;
import org.scilab.modules.gui.window.ScilabWindow;
import org.scilab.modules.xcos.Xcos;
import org.scilab.modules.xcos.XcosDiagram;
import org.scilab.modules.xcos.utils.XcosMessages;

public class QuitAction extends DefaultAction {

	private QuitAction(ScilabGraph scilabGraph) {
		super(XcosMessages.QUIT, scilabGraph);
	}

	public static MenuItem createMenu(ScilabGraph scilabGraph) {
		return createMenu(XcosMessages.QUIT, null, new QuitAction(scilabGraph), KeyStroke.getKeyStroke(KeyEvent.VK_Q, ActionEvent.CTRL_MASK));
	}
	
	public void doAction() {
		
		((XcosDiagram) getGraph(null)).closeDiagram();

	}
}

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

package org.scilab.modules.xcos.actions;

import java.awt.event.ActionEvent;

import org.scilab.modules.action_binding.InterpreterManagement;
import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.DefaultAction;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.xcos.utils.XcosMessages;

public class StopAction extends DefaultAction {
	
	public StopAction(ScilabGraph scilabGraph) {
		super(XcosMessages.STOP, scilabGraph);
	}

	public static PushButton createButton(ScilabGraph scilabGraph) {
		return createButton(XcosMessages.STOP, "process-stop.png", new StopAction(scilabGraph));
	}

	public static MenuItem createMenu(ScilabGraph scilabGraph) {
		return createMenu(XcosMessages.STOP, null, new StopAction(scilabGraph), null);
	}

	public void actionPerformed(ActionEvent e) {
		InterpreterManagement.requestScilabExec("haltscicos");
	}
}

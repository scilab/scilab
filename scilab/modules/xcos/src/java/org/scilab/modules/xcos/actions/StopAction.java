/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
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

public class StopAction  extends DefaultAction {
    public static PushButton stopButton(ScilabGraph scilabGraph) {
	return createButton("Stop", null, new StopAction(scilabGraph));
    }

    public static MenuItem stopMenu(ScilabGraph scilabGraph) {
	return createMenu("Stop", null, new StopAction(scilabGraph));
    }

    public StopAction() {
	super();
    }

    public StopAction(ScilabGraph scilabGraph) {
	super(scilabGraph);
    }

    public void actionPerformed(ActionEvent e) {
	InterpreterManagement.requestScilabExec("haltscicos");
    }
}

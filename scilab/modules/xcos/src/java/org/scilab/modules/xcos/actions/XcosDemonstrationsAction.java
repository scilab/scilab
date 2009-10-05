/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Allan SIMON
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

/**
 * View Xcos demos
 * @author Allan SIMON
 */
public final class XcosDemonstrationsAction extends DefaultAction {

	private static final long serialVersionUID = 1L;

	/**
	 * Constructor
	 * @param scilabGraph corresponding Scilab Graph
	 */
	private XcosDemonstrationsAction(ScilabGraph scilabGraph) {
		super(XcosMessages.XCOS_DEMONSTRATIONS, scilabGraph);
	}

	/**
	 * Create a button for a graph toolbar
	 * @param scilabGraph corresponding Scilab Graph
	 * @return the button
	 */
	public static PushButton createButton(ScilabGraph scilabGraph) {
		return createButton(XcosMessages.XCOS_DEMONSTRATIONS, null, new XcosDemonstrationsAction(scilabGraph));
	}

	/**
	 * Create a menu for a graph menubar
	 * @param scilabGraph corresponding Scilab Graph
	 * @return the menu
	 */
	public static MenuItem createMenu(ScilabGraph scilabGraph) {
		return createMenu(XcosMessages.XCOS_DEMONSTRATIONS, null, new XcosDemonstrationsAction(scilabGraph), null);
	}
	
	/**
	 * Action associated
	 * @param e the event
	 * @see org.scilab.modules.gui.events.callback.CallBack#actionPerformed(java.awt.event.ActionEvent)
	 */
	public void actionPerformed(ActionEvent e) {
		InterpreterManagement.requestScilabExec("exec(\"" + System.getenv("SCI") + "/modules/xcos/demos/xcos_demos.sce\", -1)");
	}
}

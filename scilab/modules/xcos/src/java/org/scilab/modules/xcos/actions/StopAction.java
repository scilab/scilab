/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2009 - DIGITEO - Vincent COUVERT
 * Copyright (C) 2010 - DIGITEO - Clément DAVID
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

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.base.DefaultAction;
import org.scilab.modules.graph.actions.base.GraphActionManager;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.utils.XcosInterpreterManagement;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Stop the simulation
 */
public class StopAction extends DefaultAction {
	public static final String NAME = XcosMessages.STOP;
	public static final String SMALL_ICON = "process-stop.png";
	public static final int MNEMONIC_KEY = 0;
	public static final int ACCELERATOR_KEY = 0;

	/**
	 * @param scilabGraph graph
	 */
	public StopAction(ScilabGraph scilabGraph) {
		super(scilabGraph);
		setEnabled(!GraphActionManager.getEnable(StartAction.class));
	}

	/**
	 * @param scilabGraph graph
	 * @return push button
	 */
	public static PushButton createButton(ScilabGraph scilabGraph) {
		return createButton(scilabGraph, StopAction.class);
	}

	/**
	 * @param scilabGraph graph
	 * @return menu item
	 */
	public static MenuItem createMenu(ScilabGraph scilabGraph) {
		return createMenu(scilabGraph, StopAction.class);
	}

	/**
	 * Action !!!
	 * @param e source event
	 * @see org.scilab.modules.gui.events.callback.CallBack#actionPerformed(java.awt.event.ActionEvent)
	 */
	public void actionPerformed(ActionEvent e) {
		XcosInterpreterManagement.requestScilabExec("haltscicos");
		
		((XcosDiagram) getGraph(null)).info(XcosMessages.EMPTY_INFO);
		GraphActionManager.setEnable(StartAction.class, true);
		GraphActionManager.setEnable(StopAction.class, false);
		((XcosDiagram) getGraph(null)).setReadOnly(false);
	}
}

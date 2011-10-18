/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2009 - DIGITEO - Vincent COUVERT
 * Copyright (C) 2010 - DIGITEO - Clément DAVID
 * Copyright (C) 2011 - Scilab Enterprises - Clément DAVID
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
import org.scilab.modules.graph.actions.base.GraphActionManager;
import org.scilab.modules.graph.actions.base.OneBlockDependantAction;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.xcos.actions.workers.ScilabGraphWorker;
import org.scilab.modules.xcos.actions.workers.ScilabGraphWorker.Action;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Start the simulation
 */
public class StartAction extends OneBlockDependantAction {
	/** Name of the action */
	public static final String NAME = XcosMessages.START;
	/** Icon name of the action */
	public static final String SMALL_ICON = "media-playback-start.png";
	/** Mnemonic key of the action */
	public static final int MNEMONIC_KEY = 0;
	/** Accelerator key for the action */
	public static final int ACCELERATOR_KEY = 0;

	/**
	 * @param scilabGraph
	 *            graph
	 */
	public StartAction(ScilabGraph scilabGraph) {
		super(scilabGraph);
		setEnabled(GraphActionManager.getEnable(StartAction.class));
	}

	/**
	 * @param scilabGraph
	 *            graph
	 * @return push button
	 */
	public static PushButton createButton(ScilabGraph scilabGraph) {
		return createButton(scilabGraph, StartAction.class);
	}

	/**
	 * @param scilabGraph
	 *            graph
	 * @return menu item
	 */
	public static MenuItem createMenu(ScilabGraph scilabGraph) {
		return createMenu(scilabGraph, StartAction.class);
	}

	/**
	 * Action !!!
	 * 
	 * @param e
	 *            the source event
	 * @see org.scilab.modules.gui.events.callback.CallBack#actionPerformed(java.awt.event.ActionEvent)
	 */
	@Override
	public void actionPerformed(ActionEvent e) {
		final XcosDiagram diagram = ((XcosDiagram) getGraph(e))
				.getRootDiagram();

		ScilabGraphWorker.start(diagram, Action.SIMULATE);
	}
}

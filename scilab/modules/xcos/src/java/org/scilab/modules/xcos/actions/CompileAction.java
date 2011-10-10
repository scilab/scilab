/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Allan SIMON
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
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xcos.actions.workers.ScilabGraphWorker;
import org.scilab.modules.xcos.actions.workers.ScilabGraphWorker.Action;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Diagram compilation management
 */
public class CompileAction extends SimulationNotRunningAction {
	/** Name of the action */
	public static final String NAME = XcosMessages.COMPILE;
	/** Icon name of the action */
	public static final String SMALL_ICON = "";
	/** Mnemonic key of the action */
	public static final int MNEMONIC_KEY = 0;
	/** Accelerator key for the action */
	public static final int ACCELERATOR_KEY = 0;

	/**
	 * Constructor
	 * 
	 * @param scilabGraph
	 *            associated diagram
	 */
	public CompileAction(ScilabGraph scilabGraph) {
		super(scilabGraph);
	}

	/**
	 * Create associated menu
	 * 
	 * @param scilabGraph
	 *            associated diagram
	 * @return the menu
	 */
	public static MenuItem createMenu(ScilabGraph scilabGraph) {
		return createMenu(scilabGraph, CompileAction.class);
	}

	/**
	 * @param e
	 *            parameter
	 * @see org.scilab.modules.graph.actions.base.DefaultAction#actionPerformed(java.awt.event.ActionEvent)
	 */
	@Override
	public void actionPerformed(ActionEvent e) {
		final XcosDiagram diagram = ((XcosDiagram) getGraph(e))
				.getRootDiagram();

		ScilabGraphWorker.start(diagram, Action.COMPILE);
	}
}

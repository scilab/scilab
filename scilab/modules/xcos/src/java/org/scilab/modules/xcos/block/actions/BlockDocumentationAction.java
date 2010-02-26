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

package org.scilab.modules.xcos.block.actions;

import java.awt.event.ActionEvent;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.base.VertexSelectionDependantAction;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.utils.XcosDialogs;
import org.scilab.modules.xcos.utils.XcosInterpreterManagement;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * View Xcos documentation
 */
public final class BlockDocumentationAction extends VertexSelectionDependantAction {
	public static final String NAME = XcosMessages.BLOCK_DOCUMENTATION;
	public static final String SMALL_ICON = "";
	public static final int MNEMONIC_KEY = 0;
	public static final int ACCELERATOR_KEY = 0;

	/**
	 * Constructor
	 * @param scilabGraph corresponding Scilab Graph
	 */
	public BlockDocumentationAction(ScilabGraph scilabGraph) {
		super(scilabGraph);
	}

	/**
	 * Create a button for a graph toolbar
	 * @param scilabGraph corresponding Scilab Graph
	 * @return the button
	 */
	public static PushButton createButton(ScilabGraph scilabGraph) {
		return createButton(scilabGraph, BlockDocumentationAction.class);
	}

	/**
	 * Create a menu for a graph menubar
	 * @param scilabGraph corresponding Scilab Graph
	 * @return the menu
	 */
	public static MenuItem createMenu(ScilabGraph scilabGraph) {
		return createMenu(scilabGraph, BlockDocumentationAction.class);
	}
	
	/**
	 * Action associated
	 * @param e the event
	 * @see org.scilab.modules.gui.events.callback.CallBack#actionPerformed(java.awt.event.ActionEvent)
	 */
	public void actionPerformed(ActionEvent e) {
		BasicBlock selectedBlock = ((BasicBlock) getGraph(e).getSelectionCell());
		if (selectedBlock != null) {
			XcosInterpreterManagement.requestScilabExec("help " + selectedBlock.getInterfaceFunctionName());
		} else {
			XcosDialogs.noBlockSelected((XcosDiagram) getGraph(e));
		}
	}

}

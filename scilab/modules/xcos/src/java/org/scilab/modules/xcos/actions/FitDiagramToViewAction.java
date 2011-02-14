/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Vincent COUVERT
 * Copyright (C) 2009 - DIGITEO - Clément DAVID
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

import org.scilab.modules.graph.ScilabComponent;
import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.base.OneBlockDependantAction;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Use maximum of space inside the diagram
 */
public final class FitDiagramToViewAction extends OneBlockDependantAction {
	/** Name of the action */
	public static final String NAME = XcosMessages.FIT_DIAGRAM_TO_VIEW;
	/** Icon name of the action */
	public static final String SMALL_ICON = "zoom-fit-drawing";
	/** Mnemonic key of the action */
	public static final int MNEMONIC_KEY = 0;
	/** Accelerator key for the action */
	public static final int ACCELERATOR_KEY = 0;

	/**
	 * Constructor
	 * @param scilabGraph associated diagram
	 */
	public FitDiagramToViewAction(ScilabGraph scilabGraph) {
		super(scilabGraph);
	}

	/**
	 * Menu to add to the menubar
	 * @param scilabGraph associated diagram
	 * @return the menu
	 */
	public static MenuItem createMenu(ScilabGraph scilabGraph) {
		return createMenu(scilabGraph, FitDiagramToViewAction.class);
	}
	
	/**
	 * Button to add to the toolbar
	 * @param scilabGraph associated diagram
	 * @return the button
	 */
	public static PushButton createButton(ScilabGraph scilabGraph) {
		return createButton(scilabGraph, FitDiagramToViewAction.class);
	}

	/**
	 * @param e parameter
	 * @see org.scilab.modules.graph.actions.base.DefaultAction#actionPerformed(java.awt.event.ActionEvent)
	 */
	@Override
	public void actionPerformed(ActionEvent e) {
		// If diagram is empty (has one default child) : do nothing.
		if (getGraph(null).getModel().getChildCount(
				getGraph(null).getDefaultParent()) < 1) {
			return;
		}

		ScilabComponent comp = ((ScilabComponent) getGraph(null)
				.getAsComponent());

		/* Save the configuration */
		double oldZoomFactor = comp.getZoomFactor();

		comp.zoomAndCenterToCells();

		/* Restore previous configuration */
		comp.setZoomFactor(oldZoomFactor);
	}
}

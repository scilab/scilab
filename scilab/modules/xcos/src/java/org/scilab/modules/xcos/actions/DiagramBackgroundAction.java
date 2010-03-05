/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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

import java.awt.Color;
import java.awt.event.ActionEvent;

import javax.swing.JColorChooser;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.base.DefaultAction;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xcos.utils.XcosMessages;

import com.mxgraph.swing.mxGraphComponent;

/**
 * Diagram background color changing
 */
public class DiagramBackgroundAction extends DefaultAction {
	/** Name of the action */
	public static final String NAME = XcosMessages.DIAGRAM_BACKGROUND;
	/** Icon name of the action */
	public static final String SMALL_ICON = "";
	/** Mnemonic key of the action */
	public static final int MNEMONIC_KEY = 0;
	/** Accelerator key for the action */
	public static final int ACCELERATOR_KEY = 0;

	/**
	 * Constructor
	 * @param scilabGraph associated diagram
	 */
	public DiagramBackgroundAction(ScilabGraph scilabGraph) {
		super(scilabGraph);
	}

	/**
	 * Menu for diagram menubar
	 * @param scilabGraph associated diagram
	 * @return the menu
	 */
	public static MenuItem createMenu(ScilabGraph scilabGraph) {
		return createMenu(scilabGraph, DiagramBackgroundAction.class);
	}

	/**
	 * @param e parameter
	 * @see org.scilab.modules.graph.actions.base.DefaultAction#actionPerformed(java.awt.event.ActionEvent)
	 */
	@Override
	public void actionPerformed(ActionEvent e) {
		mxGraphComponent graphComponent = getGraph(null).getAsComponent();
		Color newColor = JColorChooser.showDialog(graphComponent, XcosMessages.DIAGRAM_BACKGROUND, null);

		if (newColor != null) {
			graphComponent.getViewport().setOpaque(false);
			graphComponent.setBackground(newColor);
		}	

		// Forces a repaint of the outline
		graphComponent.getGraph().repaint();
	}

}

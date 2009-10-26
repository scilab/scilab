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

import java.awt.event.KeyEvent;
import java.awt.Color;
import java.awt.Toolkit;

import javax.swing.JColorChooser;
import javax.swing.KeyStroke;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.DefaultAction;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xcos.XcosDiagram;
import org.scilab.modules.xcos.utils.XcosMessages;

import com.mxgraph.swing.mxGraphComponent;

/**
 * Diagram background color changing
 * @author Vincent COUVERT
 */
public class DiagramBackgroundAction extends DefaultAction {

	private static final long serialVersionUID = 1L;

	/**
	 * Constructor
	 * @param scilabGraph associated diagram
	 */
	public DiagramBackgroundAction(ScilabGraph scilabGraph) {
		super(XcosMessages.DIAGRAM_BACKGROUND, scilabGraph);
	}

	/**
	 * Menu for diagram menubar
	 * @param scilabGraph associated diagram
	 * @return the menu
	 */
	public static MenuItem createMenu(ScilabGraph scilabGraph) {
		return createMenu(XcosMessages.DIAGRAM_BACKGROUND, null, new DiagramBackgroundAction(scilabGraph), null);
	}

	/**
	 * Action !!
	 * @see org.scilab.modules.graph.actions.DefaultAction#doAction()
	 */
	public void doAction() {
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

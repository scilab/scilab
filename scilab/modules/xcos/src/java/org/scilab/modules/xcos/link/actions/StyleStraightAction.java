/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clément DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.link.actions;

import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xcos.link.BasicLink;
import org.scilab.modules.xcos.utils.XcosMessages;

import com.mxgraph.util.mxConstants;

/**
 * Implement the set link horizontal action
 */
public class StyleStraightAction extends StyleAction {
	/** Name of the action */
	public static final String NAME = XcosMessages.LINK_STYLE_STRAIGHT;
	/** Icon name of the action */
	public static final String SMALL_ICON = "";
	/** Mnemonic key of the action */
	public static final int MNEMONIC_KEY = KeyEvent.VK_S;

	/**
	 * Default constructor the associated graph
	 * 
	 * @param scilabGraph
	 *            the graph to associate
	 */
	public StyleStraightAction(ScilabGraph scilabGraph) {
		super(scilabGraph);
	}

	/**
	 * @param scilabGraph
	 *            graph
	 * @return menu item
	 */
	public static MenuItem createMenu(ScilabGraph scilabGraph) {
		return createMenu(scilabGraph, StyleStraightAction.class);
	}

	/**
	 * Action !!!
	 * 
	 * @param e
	 *            params
	 * @see org.scilab.modules.xcos.link.actions.StyleAction#actionPerformed(java.awt.event.ActionEvent)
	 */
	@Override
	public void actionPerformed(ActionEvent e) {
		BasicLink[] links = getLinks();
		
		getGraph(e).setCellStyles(mxConstants.STYLE_NOEDGESTYLE, "1", links);

		removePointsOnLinks(links);
	}
}

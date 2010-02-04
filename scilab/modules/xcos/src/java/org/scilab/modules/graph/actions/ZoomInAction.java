/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2009 - DIGITEO - Vincent COUVERT
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.graph.actions;

import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.base.DefaultAction;
import org.scilab.modules.graph.utils.ScilabGraphMessages;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.pushbutton.PushButton;

import com.mxgraph.swing.util.mxGraphActions;

/**
 * Zoom management
 */
public class ZoomInAction extends DefaultAction {
	public static final String NAME = ScilabGraphMessages.ZOOM_IN;
	public static final String SMALL_ICON = "list-add.png";
	public static final int MNEMONIC_KEY = KeyEvent.VK_PLUS;
	public static final int ACCELERATOR_KEY = Toolkit.getDefaultToolkit().getMenuShortcutKeyMask();

	/**
	 * Constructor
	 * @param scilabGraph corresponding Scilab Graph
	 */
	public ZoomInAction(ScilabGraph scilabGraph) {
		super(scilabGraph);
	}

	/**
	 * Create a button for a graph toolbar
	 * @param scilabGraph corresponding Scilab Graph
	 * @return the button
	 */
	public static PushButton zoominButton(ScilabGraph scilabGraph) {
		return createButton(scilabGraph, ZoomInAction.class);
	}

	/**
	 * Create a menu for a graph menubar
	 * @param scilabGraph corresponding Scilab Graph
	 * @return the menu
	 */
	public static MenuItem zoominMenu(ScilabGraph scilabGraph) {
		return createMenu(scilabGraph, ZoomInAction.class);
	}

	/**
	 * Action associated
	 * @param e the event
	 * @see org.scilab.modules.gui.events.callback.CallBack#actionPerformed(java.awt.event.ActionEvent)
	 */
	public void actionPerformed(ActionEvent e) {
		mxGraphActions.getZoomInAction().actionPerformed(new ActionEvent(getGraph(e).getAsComponent(),
				e.getID(), e.getActionCommand()));
	}

}

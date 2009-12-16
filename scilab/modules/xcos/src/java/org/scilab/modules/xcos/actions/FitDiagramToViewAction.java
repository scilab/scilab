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

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.DefaultAction;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.utils.XcosComponent;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Use maximum of space inside the diagram
 * @author Vincent COUVERT
 */
public final class FitDiagramToViewAction extends DefaultAction {

	private static final long serialVersionUID = 1L;

	/**
	 * Constructor
	 * @param scilabGraph associated diagram
	 */
	private FitDiagramToViewAction(ScilabGraph scilabGraph) {
		super(XcosMessages.FIT_DIAGRAM_TO_VIEW, scilabGraph);
	}

	/**
	 * Menu to add to the menubar
	 * @param scilabGraph associated diagram
	 * @return the menu
	 */
	public static MenuItem createMenu(ScilabGraph scilabGraph) {
		return createMenu(XcosMessages.FIT_DIAGRAM_TO_VIEW, null, new FitDiagramToViewAction(scilabGraph), null);
	}
	
	/**
	 * Button to add to the toolbar
	 * @param scilabGraph associated diagram
	 * @return the button
	 */
	public static PushButton createButton(ScilabGraph scilabGraph) {
		return createButton(XcosMessages.FIT_DIAGRAM_TO_VIEW, "view-fullscreen.png", new FitDiagramToViewAction(scilabGraph));
	}

	/**
	 * Action !
	 * @see org.scilab.modules.graph.actions.DefaultAction#doAction()
	 */
	public void doAction() {
    	// If diagram is empty (has one default child) : do nothing.
    	if (getGraph(null).getModel().getChildCount(getGraph(null).getDefaultParent()) < 2) { return; }
		
		/* Save the configuration */
		double oldZoomFactor = ((XcosDiagram) getGraph(null)).getAsComponent().getZoomFactor();
		
		((XcosComponent) getGraph(null).getAsComponent()).zoomAndCenterToCells();
		
		/* Restore previous configuration */
		((XcosDiagram) getGraph(null)).getAsComponent().setZoomFactor(oldZoomFactor);
	}
}

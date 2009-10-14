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

import java.awt.Dimension;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.DefaultAction;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.xcos.XcosDiagram;
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
		Dimension viewportSize = ((XcosDiagram) getGraph(null)).getAsComponent().getViewport().getSize();
		Dimension componentSize = ((XcosDiagram) getGraph(null)).getAsComponent().getPreferredSize();
		
		double newZoomFactor = 0;
		double oldZoomFactor = ((XcosDiagram) getGraph(null)).getAsComponent().getZoomFactor();
		
		if (viewportSize.getHeight() / componentSize.getHeight() > 1.0 || viewportSize.getWidth() / componentSize.getWidth() > 1.0) {
			// The viewport is too big
			newZoomFactor = Math.min(viewportSize.getHeight() / componentSize.getHeight(), 
					viewportSize.getWidth() / componentSize.getWidth());
			((XcosDiagram) getGraph(null)).getAsComponent().setZoomFactor(newZoomFactor);
			((XcosDiagram) getGraph(null)).getAsComponent().zoomIn();
		} else if (componentSize.getHeight() / viewportSize.getHeight() > 1.0 || componentSize.getWidth() / viewportSize.getWidth() > 1.0) {
			// The component is too big
			newZoomFactor = Math.min(componentSize.getHeight() / viewportSize.getHeight(),
					componentSize.getWidth() / viewportSize.getWidth());
			((XcosDiagram) getGraph(null)).getAsComponent().setZoomFactor(newZoomFactor);
			((XcosDiagram) getGraph(null)).getAsComponent().zoomOut();
		}
		
		((XcosDiagram) getGraph(null)).getAsComponent().setZoomFactor(oldZoomFactor);
		
	}
}

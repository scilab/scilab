/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent Couvert
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.utils;

import java.awt.event.ActionEvent;

import javax.swing.AbstractAction;

import org.flexdock.docking.Dockable;
import org.flexdock.docking.DockingManager;
import org.scilab.modules.gui.bridge.tab.SwingScilabTab;
import org.scilab.modules.gui.bridge.window.SwingScilabWindow;
import org.scilab.modules.gui.window.ScilabWindow;
import org.scilab.modules.gui.window.Window;

/**
 * Action executed when the user clicks on the "Undock button"
 * @author Vincent COUVERT
 */
public class SciUndockingAction extends AbstractAction {

	private static final long serialVersionUID = 3906773111254753683L;

	private static final int UNDOCK_OFFSET = 30;
	
	private SwingScilabTab associatedTab;
	
	/**
	 * Constructor
	 * @param tab the associated tab
	 */
	public SciUndockingAction(SwingScilabTab tab) {
		associatedTab = tab;
	}
	
	/**
	 * What do I have to do when the event accurs
	 * @param arg0 the event
	 * @see java.awt.event.ActionListener#actionPerformed(java.awt.event.ActionEvent)
	 */
	public void actionPerformed(ActionEvent arg0) {
		/** Create a new Window to dock the tab into */
		Window newWindow = ScilabWindow.createWindow();
		
		/** Save the tab dimensions to set them back after docking */
		Size oldtabSize = associatedTab.getDims();
		/** Save the old parent Window position to use it to set the new Window position */ 
		Position oldWindowPosition = UIElementMapper.getCorrespondingUIElement(associatedTab.getParentWindowId()).getPosition();
		
		/** Undock/Dock the tab */
		DockingManager.undock((Dockable) associatedTab);
		DockingManager.dock(associatedTab, ((SwingScilabWindow) newWindow.getAsSimpleWindow()).getDockingPort());
		
		/** New Window properties */
		newWindow.addMenuBar(associatedTab.getMenuBar());
		newWindow.addToolBar(associatedTab.getToolBar());
		newWindow.addInfoBar(associatedTab.getInfoBar());
		newWindow.setPosition(new Position(oldWindowPosition.getX() + UNDOCK_OFFSET, oldWindowPosition.getY() + UNDOCK_OFFSET));
		
		newWindow.setVisible(true);
		
		/** Set new Window dimensions so that the tab has the same dimensions as before */
		Size windowSize = newWindow.getDims();
		Size newTabSize = associatedTab.getDims();
		newWindow.setDims(new Size((windowSize.getWidth() - newTabSize.getWidth()) + oldtabSize.getWidth(),
				(windowSize.getHeight() - newTabSize.getHeight()) + oldtabSize.getHeight()));
		
		associatedTab.setParentWindowId(newWindow.getAsSimpleWindow().getElementId());
	}
}

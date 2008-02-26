/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent Couvert
 * Copyright (C) 2007 - INRIA - Bruno JOFRET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.utils;

import java.util.Iterator;
import java.util.Set;

import org.flexdock.docking.Dockable;
import org.flexdock.docking.event.DockingEvent;
import org.flexdock.docking.event.DockingListener;
import org.scilab.modules.gui.bridge.tab.SwingScilabTab;
import org.scilab.modules.gui.window.Window;

/**
 * Listener for docking operations in Scilab
 * @author Vincent COUVERT
 * @author Bruno JOFRET
 */
public class SciDockingListener implements DockingListener {
	
	private int associatedScilabWindowId;
	
	/**
	 * Default constructor
	 */
	public SciDockingListener() {
		super();
	}

	/**
	 * Handling docking canceled events
	 * @param e the event
	 * @see org.flexdock.docking.event.DockingListener#dockingCanceled(org.flexdock.docking.event.DockingEvent)
	 */
	public void dockingCanceled(DockingEvent e) {
		debug("dockingCanceled");
	}

	/**
	 * Handling docking complete events
	 * @param e the event
	 * @see org.flexdock.docking.event.DockingListener#dockingCanceled(org.flexdock.docking.event.DockingEvent)
	 */
	public void dockingComplete(DockingEvent e) {
		debug("dockingComplete");
		int newId = 0;

		DockingListener[] newListeners = e.getNewDockingPort().getDockingListeners();
		if (newListeners.length == 2) { 
			/* This docking port has been created when the parent window were created */
			/* So this docking port has a sciDockingListener */
			newId = ((SciDockingListener) newListeners[1]).getAssociatedWindowId();
		} else { /* Docking port created by Flexdock itself */
			/* Retrieve the ID of the parent window */
			Set<Dockable> allDockables = e.getNewDockingPort().getDockables();
			/* Have to find an other dockable than the one we just docked */
			Iterator<Dockable> it =  allDockables.iterator();
			Dockable dock = it.next();
			while (dock == e.getDockable()) {
				dock = it.next();
			}
			newId = ((SwingScilabTab) dock).getParentWindowId();
		}
		SwingScilabTab dockedTab = (SwingScilabTab) e.getDockable();
		dockedTab.setParentWindowId(newId);

	}

	/**
	 * Handling docking started events
	 * @param e the event
	 * @see org.flexdock.docking.event.DockingListener#dockingCanceled(org.flexdock.docking.event.DockingEvent)
	 */
	public void dragStarted(DockingEvent e) { 
		// DEBUG
		debug("dragStarted");
	}

	/**
	 * Handling drop started events
	 * @param e the event
	 * @see org.flexdock.docking.event.DockingListener#dockingCanceled(org.flexdock.docking.event.DockingEvent)
	 */
	public void dropStarted(DockingEvent e) {
		// DEBUG
		debug("dropStarted");
	}

	/**
	 * Handling undocking complete events
	 * @param e the event
	 * @see org.flexdock.docking.event.DockingListener#dockingCanceled(org.flexdock.docking.event.DockingEvent)
	 */
	public void undockingComplete(DockingEvent e) {
		// DEBUG
		debug("undockingComplete");
		// If the dock is empty, we close the parent Window
		if (e.getOldDockingPort().getDockables().isEmpty()) {
			((Window) UIElementMapper.getCorrespondingUIElement(associatedScilabWindowId)).getAsSimpleWindow().close();
		}
	}

	/**
	 * Handling undocking started events
	 * @param e the event
	 * @see org.flexdock.docking.event.DockingListener#dockingCanceled(org.flexdock.docking.event.DockingEvent)
	 */
	public void undockingStarted(DockingEvent e) {
		// DEBUG
		debug("undockingStarted");
	}	
	
	/**
	 * Set the window object associated to this docking listener
	 * @param id the id of the window associated
	 */
	public void setAssociatedWindowId(int id) {
		this.associatedScilabWindowId = id;
	}
	
	/**
	 * Get the window object associated to this docking listener
	 * @return the id of the window associated
	 */
	public int getAssociatedWindowId() {
		return this.associatedScilabWindowId;
	}
	
	/**
	 * Display debug information
	 * @param method the name of the method called
	 */
	private void debug(String method) {
		//System.out.println("["+method+"] on Window "+associatedScilabWindowId);
	}

}

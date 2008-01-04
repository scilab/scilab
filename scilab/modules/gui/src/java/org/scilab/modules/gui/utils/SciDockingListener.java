
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.utils;

import org.flexdock.docking.event.DockingEvent;
import org.flexdock.docking.event.DockingListener;
import org.scilab.modules.gui.bridge.tab.SwingScilabTab;

/**
 * Listener for docking operations in Scilab
 * @author Bruno JOFRET
 * @author Vincent COUVERT
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
		SwingScilabTab dockedTab = (SwingScilabTab) e.getDockable();
			dockedTab.setParentWindowId(associatedScilabWindowId);
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
	 * Display debug information
	 * @param method the name of the method called
	 */
	private void debug(String method) {
		//System.out.println("["+method+"] on Window "+associatedScilabWindowId);
	}

}

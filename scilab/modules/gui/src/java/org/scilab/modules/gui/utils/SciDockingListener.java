
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.utils;

import org.flexdock.docking.event.DockingEvent;
import org.flexdock.docking.event.DockingListener;
import org.scilab.modules.gui.bridge.tab.SwingScilabTab;
import org.scilab.modules.gui.menubar.ScilabMenuBar;
import org.scilab.modules.gui.window.ScilabWindow;

/**
 * Listener for docking operations in Scilab
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
		// TODO Auto-generated method stub

	}

	/**
	 * Handling docking complete events
	 * @param e the event
	 * @see org.flexdock.docking.event.DockingListener#dockingCanceled(org.flexdock.docking.event.DockingEvent)
	 */
	public void dockingComplete(DockingEvent e) {
		SwingScilabTab dockedTab = ((SwingScilabTab) e.getDockable());
		
		// Change the parent window id of docked tab
		dockedTab.setParentWindowId(associatedScilabWindowId);
		
		int dockedTabMenuBarId = UIElementMapper.getCorrespondingUIElement(dockedTab.getElementId()).getMenuBarId();
		ScilabMenuBar dockedTabMenuBar = (ScilabMenuBar) UIElementMapper.getCorrespondingUIElement(dockedTabMenuBarId);
		
		if (dockedTabMenuBar != null) {
			// If docked tab has a menubar, this menubar becomes current menubar for the parent window
			((ScilabWindow) UIElementMapper.getCorrespondingUIElement(associatedScilabWindowId)).addMenuBar(dockedTabMenuBar);
		}
		
		if (e.getOldDockingPort() != null) {
			e.getOldDockingPort().dockingCanceled(e);
		}
	}

	/**
	 * Handling docking started events
	 * @param e the event
	 * @see org.flexdock.docking.event.DockingListener#dockingCanceled(org.flexdock.docking.event.DockingEvent)
	 */
	public void dragStarted(DockingEvent e) {
		// TODO Auto-generated method stub

	}

	/**
	 * Handling drop started events
	 * @param e the event
	 * @see org.flexdock.docking.event.DockingListener#dockingCanceled(org.flexdock.docking.event.DockingEvent)
	 */
	public void dropStarted(DockingEvent e) {
		// TODO Auto-generated method stub

	}

	/**
	 * Handling undocking complete events
	 * @param e the event
	 * @see org.flexdock.docking.event.DockingListener#dockingCanceled(org.flexdock.docking.event.DockingEvent)
	 */
	public void undockingComplete(DockingEvent e) {
        // TODO Change the parent window id of docked tab (is it useful ?)
		SwingScilabTab dockedTab = ((SwingScilabTab) e.getDockable());
		dockedTab.setParentWindowId(associatedScilabWindowId);
	}

	/**
	 * Handling undocking started events
	 * @param e the event
	 * @see org.flexdock.docking.event.DockingListener#dockingCanceled(org.flexdock.docking.event.DockingEvent)
	 */
	public void undockingStarted(DockingEvent e) {
		// TODO Auto-generated method stub

	}
	
	/**
	 * Set the window object associated to this docking listener
	 * @param id the id of the window associated
	 */
	public void setAssociatedWindowId(int id) {
		this.associatedScilabWindowId = id;
	}

}

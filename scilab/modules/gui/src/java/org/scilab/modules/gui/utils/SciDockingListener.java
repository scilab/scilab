
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.utils;

import org.flexdock.docking.event.DockingEvent;
import org.flexdock.docking.event.DockingListener;
import org.scilab.modules.gui.bridge.tab.SwingScilabTab;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.toolbar.ToolBar;

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
	public void dockingCanceled(DockingEvent e) { }

	/**
	 * Handling docking complete events
	 * @param e the event
	 * @see org.flexdock.docking.event.DockingListener#dockingCanceled(org.flexdock.docking.event.DockingEvent)
	 */
	public void dockingComplete(DockingEvent e) {
		SwingScilabTab dockedTab = (SwingScilabTab) e.getDockable();
		int parentWindowsID = dockedTab.getParentWindowId(); 

		if (parentWindowsID != associatedScilabWindowId) {
			//DEBUG
//			System.out.println("---");
//			System.out.println("[DOCKING COMPLETE] Listener ID = "+this.associatedScilabWindowId);
//			System.out.println("[DOCKING COMPLETE] Parent ID = "+parentWindowsID);
//			System.out.println("---");
			// Change the parent window id of docked tab
			dockedTab.setParentWindowId(associatedScilabWindowId);
			MenuBar newMenuBar = dockedTab.getMenuBar();
			ToolBar newToolBar = dockedTab.getToolBar();
			BarUpdater.updateBars(associatedScilabWindowId, newMenuBar, newToolBar);
		} 
		else if (e.getOldDockingPort() != null 
				&& e.getOldDockingPort().getDockedComponent() != null
				&& e.getOldDockingPort().getDockedComponent() instanceof SwingScilabTab) {
			/** There is still a SwingScilabTab Docked */
			SwingScilabTab remainDockedTab = (SwingScilabTab) e.getOldDockingPort().getDockedComponent();
			MenuBar oldMenuBar = remainDockedTab.getMenuBar();
			ToolBar oldToolBar = remainDockedTab.getToolBar();
			BarUpdater.updateBars(parentWindowsID, oldMenuBar, oldToolBar);
		}
	}

	/**
	 * Handling docking started events
	 * @param e the event
	 * @see org.flexdock.docking.event.DockingListener#dockingCanceled(org.flexdock.docking.event.DockingEvent)
	 */
	public void dragStarted(DockingEvent e) { }

	/**
	 * Handling drop started events
	 * @param e the event
	 * @see org.flexdock.docking.event.DockingListener#dockingCanceled(org.flexdock.docking.event.DockingEvent)
	 */
	public void dropStarted(DockingEvent e) { }

	/**
	 * Handling undocking complete events
	 * @param e the event
	 * @see org.flexdock.docking.event.DockingListener#dockingCanceled(org.flexdock.docking.event.DockingEvent)
	 */
	public void undockingComplete(DockingEvent e) { }

	/**
	 * Handling undocking started events
	 * @param e the event
	 * @see org.flexdock.docking.event.DockingListener#dockingCanceled(org.flexdock.docking.event.DockingEvent)
	 */
	public void undockingStarted(DockingEvent e) {
		System.out.println("[UNDOCKING STARTED]Is there any component left : " + e.getOldDockingPort().getDockedComponent());
	}	
	
	/**
	 * Set the window object associated to this docking listener
	 * @param id the id of the window associated
	 */
	public void setAssociatedWindowId(int id) {
		this.associatedScilabWindowId = id;
	}

}

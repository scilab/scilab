
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.utils;

import java.awt.Component;

import javax.swing.JComponent;

import org.flexdock.docking.event.DockingEvent;
import org.flexdock.docking.event.DockingListener;
import org.scilab.modules.gui.bridge.tab.SwingScilabTab;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.toolbar.ToolBar;
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
		
		int parentWindowsID = dockedTab.getParentWindowId(); 
		// Change the parent window id of docked tab
		dockedTab.setParentWindowId(associatedScilabWindowId);
		MenuBar newMenuBar = UIElementMapper.getCorrespondingUIElement(dockedTab.getElementId()).getMenuBar();
		ToolBar newToolBar = UIElementMapper.getCorrespondingUIElement(dockedTab.getElementId()).getToolBar();
		updateBars(parentWindowsID, newMenuBar, newToolBar);
			
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
		System.out.println("[DRAG STARTED] Is there any component left : " + e.getOldDockingPort().getDockedComponent());
	}

	/**
	 * Handling drop started events
	 * @param e the event
	 * @see org.flexdock.docking.event.DockingListener#dockingCanceled(org.flexdock.docking.event.DockingEvent)
	 */
	public void dropStarted(DockingEvent e) {
		// TODO Auto-generated method stub
		System.out.println("[DROP STARTED]Is there any component left : " + e.getOldDockingPort().getDockedComponent());
	}

	/**
	 * Handling undocking complete events
	 * @param e the event
	 * @see org.flexdock.docking.event.DockingListener#dockingCanceled(org.flexdock.docking.event.DockingEvent)
	 */
	public void undockingComplete(DockingEvent e) {
        // TODO Change the parent window id of docked tab (is it useful ?)
		SwingScilabTab dockedTab = ((SwingScilabTab) e.getDockable());
		int oldWindowsID = dockedTab.getParentWindowId();
		dockedTab.setParentWindowId(associatedScilabWindowId);
		// TESTING !!!!!!
		System.out.println("[UNDOCK COMPLETE]Is there any component left : " + e.getOldDockingPort().getDockedComponent());
		if (e.getOldDockingPort().getDockedComponent() == null) {
			/** There are no more component docked... */
			e.getOldDockingPort().clear();
		}
		else {
			/** There is still a component Docked */
			SwingScilabTab remainDockedTab = (SwingScilabTab) e.getOldDockingPort().getDockedComponent();
			MenuBar newMenuBar = UIElementMapper.getCorrespondingUIElement(remainDockedTab.getElementId()).getMenuBar();
			ToolBar newToolBar = UIElementMapper.getCorrespondingUIElement(remainDockedTab.getElementId()).getToolBar();
			//System.out.println("[MenuBar, ToolBar] = "+ newMenuBar.getAsSimpleMenuBar() + " , " + newToolBar.getAsSimpleToolBar());
			updateBars(oldWindowsID, newMenuBar, newToolBar);
		}
	}

	/**
	 * Handling undocking started events
	 * @param e the event
	 * @see org.flexdock.docking.event.DockingListener#dockingCanceled(org.flexdock.docking.event.DockingEvent)
	 */
	public void undockingStarted(DockingEvent e) {
		System.out.println("[UNDOCK STARTED]Is there any component left : " + e.getOldDockingPort().getDockedComponent());
		// TODO Auto-generated method stub

	}
	
	private void updateBars(int parentWindowsID, MenuBar newMenuBar, ToolBar newToolBar) {
		MenuBar dockedTabMenuBar = UIElementMapper.getCorrespondingUIElement(parentWindowsID).getMenuBar();
		
		if (dockedTabMenuBar != null) {
			// If docked tab has a menubar, this menubar becomes current menubar for the parent window
			UIElementMapper.getCorrespondingUIElement(associatedScilabWindowId).addMenuBar(dockedTabMenuBar);
		}
		
		// Manage the toolbar
		//int dockedTabToolBarId = UIElementMapper.getCorrespondingUIElement(dockedTab.getElementId()).getToolBarId();
		ToolBar dockedTabToolBar = UIElementMapper.getCorrespondingUIElement(parentWindowsID).getToolBar();
		
		if (dockedTabToolBar != null) {
			// If docked tab has a toolbar, this toolbar becomes current menubar for the parent window
			UIElementMapper.getCorrespondingUIElement(associatedScilabWindowId).addToolBar(dockedTabToolBar);
		}
		
	}
	
	
	/**
	 * Set the window object associated to this docking listener
	 * @param id the id of the window associated
	 */
	public void setAssociatedWindowId(int id) {
		this.associatedScilabWindowId = id;
	}

}

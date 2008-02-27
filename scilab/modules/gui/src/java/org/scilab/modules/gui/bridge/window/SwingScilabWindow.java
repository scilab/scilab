/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent Couvert
 * Copyright (C) 2007 - INRIA - Bruno JOFRET
 * Copyright (C) 2007 - INRIA - Marouane BEN JELLOUL
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.bridge.window;

import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;

import javax.swing.ImageIcon;
import javax.swing.JFrame;

import org.flexdock.docking.DockingConstants;
import org.flexdock.docking.DockingManager;
import org.flexdock.docking.DockingPort;
import org.flexdock.docking.defaults.DefaultDockingPort;
import org.flexdock.view.View;

import org.scilab.modules.gui.bridge.menubar.SwingScilabMenuBar;
import org.scilab.modules.gui.bridge.tab.SwingScilabTab;
import org.scilab.modules.gui.bridge.textbox.SwingScilabTextBox;
import org.scilab.modules.gui.bridge.toolbar.SwingScilabToolBar;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.menubar.SimpleMenuBar;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.textbox.SimpleTextBox;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.SimpleToolBar;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.SciDockingListener;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.gui.window.SimpleWindow;

/**
 * Swing implementation for Scilab windows in GUIs
 * This implementation uses FlexDock package
 * @author Vincent COUVERT
 * @author Bruno JOFRET
 * @author Marouane BEN JELLOUL
 */
public class SwingScilabWindow extends JFrame implements SimpleWindow {

	private static final long serialVersionUID = -5661926417765805660L;

	private static final int DEFAULTWIDTH = 500;
	private static final int DEFAULTHEIGHT = 500;

	private DefaultDockingPort sciDockingPort;
	private SciDockingListener sciDockingListener;
	private SimpleMenuBar menuBar;
	private SimpleToolBar toolBar;
	private SimpleTextBox infoBar;
	
	private int elementId; // the id of the Window which contains this SimpleWindow
	
	/**
	 * Constructor
	 */
	public SwingScilabWindow() {
		super();

		// TODO : Only for testing : Must be removed
		this.setDims(new Size(DEFAULTWIDTH, DEFAULTHEIGHT));
		this.setTitle("Scilab");
		setIconImage(new ImageIcon(System.getenv("SCI") + "/modules/gui/images/icons/scilab.png").getImage());
		
		/* defining the Layout */
		super.setLayout(new java.awt.BorderLayout());
		
		/* Create automatically a docking port associated to the window */
		sciDockingPort = new DefaultDockingPort();
		/* The docking port is the center of the Layout of the Window */
		super.add(sciDockingPort, java.awt.BorderLayout.CENTER);
		
		/* there is no menuBar, no toolBar and no infoBar at creation */
		this.menuBar = null;
		this.toolBar = null;
		this.infoBar = null;
		
		sciDockingListener = new SciDockingListener();
		sciDockingPort.addDockingListener(sciDockingListener);
		
		addWindowListener(new WindowAdapter() {
			public void windowClosing(WindowEvent e) {
				Object[] dockArray = sciDockingPort.getDockables().toArray();
				for (int i = 0; i < dockArray.length; i++) {
					((View) dockArray[i]).getActionButton(DockingConstants.CLOSE_ACTION).getAction().actionPerformed(null);
				}
			}
		});
	}

	/**
	 * Creates a swing Scilab window
	 * @return the created window
	 */
	public static SimpleWindow createWindow() {
		return new SwingScilabWindow();
	}

	/**
	 * Draws a swing Scilab window
	 * @see org.scilab.modules.gui.UIElement#draw()
	 */
	public void draw() {
		this.setVisible(true);
		this.doLayout();
	}

	/**
	 * Gets the dimensions (width and height) of a swing Scilab window
	 * @return the dimensions of the window
	 * @see org.scilab.modules.gui.UIElement#getDims()
	 */
	public Size getDims() {
		return new Size(getSize().width, getSize().height);
	}

	/**
	 * Sets the dimensions (width and height) of a swing Scilab window
	 * @param newWindowSize the dimensions to set to the window
	 * @see org.scilab.modules.gui.UIElement#setDims(org.scilab.modules.gui.utils.Size)
	 */
	public void setDims(Size newWindowSize) {
		this.setSize(newWindowSize.getWidth(), newWindowSize.getHeight());
	}

	/**
	 * Gets the position (X-coordinate and Y-coordinate) of a swing Scilab window
	 * @return the position of the window
	 * @see org.scilab.modules.gui.UIElement#getPosition()
	 */
	public Position getPosition() {
		return new Position(this.getX(), this.getY());
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a swing Scilab window
	 * @param newWindowPosition the position to set to the window
	 * @see org.scilab.modules.gui.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
	 */
	public void setPosition(Position newWindowPosition) {
		this.setLocation(newWindowPosition.getX(), newWindowPosition.getY());
	}

	/**
	 * Gets the title of a swing Scilab window
	 * @return the title of the window
	 * @see java.awt.Frame#getTitle(java.lang.String)
	 */
	public String getTitle() {
		return super.getTitle();
	}

	/**
	 * Sets the title of a swing Scilab window
	 * @param newWindowTitle the title to set to the window
	 * @see java.awt.Frame#setTitle(java.lang.String)
	 */
	public void setTitle(String newWindowTitle) {
		super.setTitle(newWindowTitle);
	}

	/**
	 * Gets the docking port associated to the window (created by default at window creation)
	 * @return the docking port associated to the window
	 */
	private DockingPort getDockingPort() {
		//return (DockingPort) centerFrame.getContentPane();
		return (DockingPort) sciDockingPort;
	}

	/**
	 * Add a Scilab tab to a Scilab window
	 * @param newTab the Scilab tab to add to the Scilab window
	 * @see org.scilab.modules.gui.window.Window#addTab(org.scilab.modules.gui.tab.Tab)
	 */
	public void addTab(Tab newTab) {
		((SwingScilabTab) newTab.getAsSimpleTab()).setParentWindowId(this.elementId);
		DockingManager.dock((SwingScilabTab) newTab.getAsSimpleTab(), this.getDockingPort());
		// Adding the MenuBar of the last added Tab
		this.addMenuBar(newTab.getMenuBar());
		this.addToolBar(newTab.getToolBar());
	}
	
	/**
	 * Remove a Scilab tab from a Scilab window
	 * @param tab the Scilab tab to remove from the Scilab window
	 * @see org.scilab.modules.gui.window.Window#removeTab(org.scilab.modules.gui.tab.Tab)
	 */
	public void removeTab(Tab tab) {
		DockingManager.close((SwingScilabTab) tab.getAsSimpleTab());
	}
	
	
	
	/**
	 * Sets a Scilab MenuBar to a Scilab window
	 * @param newMenuBar the Scilab MenuBar to add to the Scilab window
	 * @see org.scilab.modules.gui.window.Window#setMenuBar(org.scilab.modules.gui.menubar.MenuBar)
	 */
	public void addMenuBar(MenuBar newMenuBar) {
		if (newMenuBar != null) {
			this.menuBar = newMenuBar.getAsSimpleMenuBar();
			super.setJMenuBar((SwingScilabMenuBar) newMenuBar.getAsSimpleMenuBar());
		} else {
			this.menuBar = null;
			super.setJMenuBar(null);
		}
	}

	/**
	 * Sets a Scilab ToolBar to a Scilab window
	 * @param newToolBar the Scilab ToolBar to set to the Scilab window
	 * @see org.scilab.modules.gui.window.Window#setToolBar(org.scilab.modules.gui.toolbar.ToolBar)
	 */
	public void addToolBar(ToolBar newToolBar) {
		// Remove old toolbar if already set
		if (this.toolBar != null) {
			super.remove((SwingScilabToolBar) this.toolBar);
		}
		if (newToolBar != null) {
			this.toolBar = newToolBar.getAsSimpleToolBar();
			super.add((SwingScilabToolBar) this.toolBar, java.awt.BorderLayout.PAGE_START);
		} else {
			this.toolBar = null;
		}
		this.repaint();
	}

	/**
	 * Sets a Scilab InfoBar to a Scilab window
	 * @param newInfoBar the Scilab InfoBar to set to the Scilab window
	 * @see org.scilab.modules.gui.window.Window#setInfoBar(org.scilab.modules.gui.textbox.TextBox)
	 */
	public void addInfoBar(TextBox newInfoBar) {
		this.infoBar = newInfoBar.getAsSimpleTextBox();
		super.add((SwingScilabTextBox) this.infoBar, java.awt.BorderLayout.PAGE_END);
	}

	/**
	 * Get the element id for this window
	 * @return id the id of the corresponding window object
	 */
	public int getElementId() {
		return elementId;
	}

	/**
	 * Set the element id for this window
	 * @param id the id of the corresponding window object
	 */
	public void setElementId(int id) {
		this.elementId = id;
		sciDockingListener.setAssociatedWindowId(id);
	}
	
	/**
	 * Close the window
	 * @see org.scilab.modules.gui.window.SimpleWindow#close()
	 */
	public void close() {
		dispose();
	}
}

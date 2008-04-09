/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 20072008 - INRIA - Vincent Couvert
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

package org.scilab.modules.gui.bridge.tab;

import javax.swing.Action;

import org.flexdock.docking.DockingConstants;
import org.flexdock.view.View;
import org.scilab.modules.gui.bridge.canvas.ScrolledSwingScilabCanvas;
import org.scilab.modules.gui.bridge.checkbox.SwingScilabCheckBox;
import org.scilab.modules.gui.bridge.console.SwingScilabConsole;
import org.scilab.modules.gui.bridge.editbox.SwingScilabEditBox;
import org.scilab.modules.gui.bridge.frame.SwingScilabFrame;
import org.scilab.modules.gui.bridge.helpbrowser.SwingScilabHelpBrowser;
import org.scilab.modules.gui.bridge.label.SwingScilabLabel;
import org.scilab.modules.gui.bridge.listbox.SwingScilabListBox;
import org.scilab.modules.gui.bridge.popupmenu.SwingScilabPopupMenu;
import org.scilab.modules.gui.bridge.pushbutton.SwingScilabPushButton;
import org.scilab.modules.gui.bridge.radiobutton.SwingScilabRadioButton;
import org.scilab.modules.gui.bridge.slider.SwingScilabSlider;
import org.scilab.modules.gui.canvas.Canvas;
import org.scilab.modules.gui.checkbox.CheckBox;
import org.scilab.modules.gui.console.Console;
import org.scilab.modules.gui.dockable.Dockable;
import org.scilab.modules.gui.editbox.EditBox;
import org.scilab.modules.gui.events.callback.CallBack;
import org.scilab.modules.gui.frame.Frame;
import org.scilab.modules.gui.helpbrowser.HelpBrowser;
import org.scilab.modules.gui.label.Label;
import org.scilab.modules.gui.listbox.ListBox;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.popupmenu.PopupMenu;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.gui.radiobutton.RadioButton;
import org.scilab.modules.gui.slider.Slider;
import org.scilab.modules.gui.tab.SimpleTab;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.BarUpdater;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

/**
 * Swing implementation for Scilab tabs in GUIs
 * This implementation uses FlexDock package
 * @author Bruno JOFRET
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public class SwingScilabTab extends View implements SimpleTab {
	
	private static final long serialVersionUID = 1L;
	
	private static final int TOP_LAYER = 0;
	
	private static final int BOTTOM_LAYER = 1;

	private int parentWindowId;
	
	private MenuBar menuBar;
	
	private ToolBar toolBar;
	
	private TextBox infoBar;

	/**
	 * Constructor
	 * @param name the name of the tab (used to identify it)
	 */
	public SwingScilabTab(String name) {
		super(name, name, name);
		// This button is "overloaded" when we add a callback
		//this.addAction(DockingConstants.CLOSE_ACTION);
		// Removed because make JOGL crash when "Unpin"
		//this.addAction(DockingConstants.PIN_ACTION);
		this.addAction(DockingConstants.ACTIVE_WINDOW);
		this.setLayout(null);
	}

	/**
     * Repaint it
     */
	public void repaint() {
		super.repaint();
		if (isActive()) {
			BarUpdater.updateBars(getParentWindowId(), getMenuBar(), getToolBar(), getInfoBar(), getName());
		}
	}
	
	/**
	 * Sets the Name of a swing Scilab tab
	 * @param newTabName the Name of the tab
	 * @see org.scilab.modules.gui.tab.Tab#setName()
	 */
	public void setName(String newTabName) {
		setTitle(newTabName, true);
		BarUpdater.updateBars(getParentWindowId(), getMenuBar(), getToolBar(), getInfoBar(), getName());
	}

	/**
	 * Gets the title of a swing Scilab tab
	 * @return the title of the tab
	 * @see org.scilab.modules.gui.tab.Tab#getTitle()
	 */
	public String getName() {
		return this.getTitle();
	}

	/**
	 * Draws a swing Scilab tab
	 * @see org.scilab.modules.gui.uielement.UIElement#draw()
	 */
	public void draw() {
		this.setVisible(true);
		this.doLayout();
	}
	
	/**
	 * Gets the dimensions (width and height) of a swing Scilab tab
	 * @return the dimensions of the tab
	 * @see org.scilab.modules.gui.uielement.UIElement#getDims()
	 */
	public Size getDims() {
		return new Size(this.getSize().width, this.getSize().height);
	}

	/**
	 * Gets the position (X-coordinate and Y-coordinate) of a swing Scilab tab
	 * @return the position of the tab
	 * @see org.scilab.modules.gui.uielement.UIElement#getPosition()
	 */
	public Position getPosition() {
		return new Position(this.getX(), this.getY());
	}

	/**
	 * Sets the dimensions (width and height) of a swing Scilab tab
	 * @param newSize the dimensions we want to set to the tab
	 * @see org.scilab.modules.gui.uielement.UIElement#setDims(org.scilab.modules.gui.utils.Size)
	 */
	public void setDims(Size newSize) {
		this.setSize(newSize.getWidth(), newSize.getHeight());
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a swing Scilab tab
	 * @param newPosition the position we want to set to the tab
	 * @see org.scilab.modules.gui.uielement.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
	 */
	public void setPosition(Position newPosition) {
		this.setLocation(newPosition.getX(), newPosition.getY());
	}

	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	public int addMember(Canvas member) {
		// TODO : Check wether we want a Canvas in a Tab or not.
		return this.addMember((ScrolledSwingScilabCanvas) member.getAsSimpleCanvas());
	}
	
	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	private int addMember(ScrolledSwingScilabCanvas member) {
		// TODO : Check wether we want a Canvas in a Tab or not.
		this.setContentPane(member);
		return this.getComponentZOrder(member);
	}
	
	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	public int addMember(Console member) {
		return this.addMember((SwingScilabConsole) member.getAsSimpleConsole());
	}

	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	private int addMember(SwingScilabConsole member) {
		this.setContentPane(member);
		return this.getComponentZOrder(member);
	}
	
	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	public int addMember(HelpBrowser member) {
		return this.addMember((SwingScilabHelpBrowser) member.getAsSimpleHelpBrowser());
	}

	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	private int addMember(SwingScilabHelpBrowser member) {
		this.setContentPane(member);
		return this.getComponentZOrder(member);
	}
	
	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	public int addMember(Frame member) {
		return this.addMember((SwingScilabFrame) member.getAsSimpleFrame());
	}

	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	private int addMember(SwingScilabFrame member) {
		this.add(member);
		setComponentZOrder(member, BOTTOM_LAYER);
		return this.getComponentZOrder(member);
	}

	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	public int addMember(PushButton member) {
		return this.addMember((SwingScilabPushButton) member.getAsSimplePushButton());
	}
	
	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	private int addMember(SwingScilabPushButton member) {
		this.add(member);
		this.revalidate(); // If do not revalidate then the component do not appear
		setComponentZOrder(member, TOP_LAYER);
		return this.getComponentZOrder(member);
	}

	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	public int addMember(EditBox member) {
		return this.addMember((SwingScilabEditBox) member.getAsSimpleEditBox());
	}
	
	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	private int addMember(SwingScilabEditBox member) {
		this.add(member);
		this.revalidate(); // If do not revalidate then the component do not appear
		setComponentZOrder(member, TOP_LAYER);
		return this.getComponentZOrder(member);
	}

	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	public int addMember(Label member) {
		return this.addMember((SwingScilabLabel) member.getAsSimpleLabel());
	}
	
	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	private int addMember(SwingScilabLabel member) {
		this.add(member);
		this.revalidate(); // If do not revalidate then the component do not appear
		setComponentZOrder(member, TOP_LAYER);
		return this.getComponentZOrder(member);
	}

	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	public int addMember(CheckBox member) {
		return this.addMember((SwingScilabCheckBox) member.getAsSimpleCheckBox());
	}
	
	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	private int addMember(SwingScilabCheckBox member) {
		this.add(member);
		this.revalidate(); // If do not revalidate then the component do not appear
		setComponentZOrder(member, TOP_LAYER);
		return this.getComponentZOrder(member);
	}

	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	public int addMember(RadioButton member) {
		return this.addMember((SwingScilabRadioButton) member.getAsSimpleRadioButton());
	}
	
	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	private int addMember(SwingScilabRadioButton member) {
		this.add(member);
		this.revalidate(); // If do not revalidate then the component do not appear
		setComponentZOrder(member, TOP_LAYER);
		return this.getComponentZOrder(member);
	}

	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	public int addMember(Slider member) {
		return this.addMember((SwingScilabSlider) member.getAsSimpleSlider());
	}
	
	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	private int addMember(SwingScilabSlider member) {
		this.add(member);
		this.revalidate(); // If do not revalidate then the component do not appear
		setComponentZOrder(member, TOP_LAYER);
		return this.getComponentZOrder(member);
	}

	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	public int addMember(ListBox member) {
		return this.addMember((SwingScilabListBox) member.getAsSimpleListBox());
	}
	
	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	private int addMember(SwingScilabListBox member) {
		//this.add(member.getScrollPane());
		this.add(member);
		this.revalidate(); // If do not revalidate then the component do not appear
		//return this.getComponentZOrder(member.getScrollPane());
		setComponentZOrder(member, TOP_LAYER);
		return this.getComponentZOrder(member);
	}

	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	public int addMember(PopupMenu member) {
		return this.addMember((SwingScilabPopupMenu) member.getAsSimplePopupMenu());
	}
	
	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	private int addMember(SwingScilabPopupMenu member) {
		this.add(member);
		this.revalidate(); // If do not revalidate then the component do not appear
		setComponentZOrder(member, TOP_LAYER);
		return this.getComponentZOrder(member);
	}

	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	public int addMember(Dockable member) {
		// TODO Auto-generated method stub
		return 0;
	}
	
	/**
	 * We want to be able to remove directly a Canvas from a Tab.
	 * @param member canvas to remove 
	 */
	public void removeMember(Canvas member) {
		this.removeMember((ScrolledSwingScilabCanvas) member.getAsSimpleCanvas());
	}
	
	/**
	 * We want to be able to remove directly a Canvas from a Tab.
	 * @param member canvas to remove 
	 */
	public void removeMember(ScrolledSwingScilabCanvas member) {
		this.remove(member);
		this.revalidate();
	}

	/**
	 * Get the current status of the Tab in its parent
	 * @return true is the tab is the tab currently "on top" in its parent
	 */
	public boolean isCurrentTab() {
        // TODO should not always return TRUE
		return true;
	}

	/**
	 * Get the parent window id for this tab
	 * @return the id of the parent window
	 */
	public int getParentWindowId() {
		return this.parentWindowId;
	}

	/**
	 * Set the parent window id for this tab
	 * @param id the id of the parent window
	 */
	public void setParentWindowId(int id) {
		this.parentWindowId = id;
	}

	/**
	 * Setter for MenuBar
	 * @param newMenuBar : the MenuBar to set.
	 * @see org.scilab.modules.gui.tab.SimpleTab#setMenuBar(org.scilab.modules.gui.menubar.MenuBar)
	 */
	public void setMenuBar(MenuBar newMenuBar) {
		this.menuBar = newMenuBar;
	}

	
	/**
	 * Getter for MenuBar
	 * @return MenuBar : the MenuBar associated to the Tab.
	 * @see org.scilab.modules.gui.tab.SimpleTab#getMenuBar()
	 */
	public MenuBar getMenuBar() {
		return this.menuBar;
	}
	
	/**
	 * Setter for ToolBar
	 * @param newToolBar : the ToolBar to set.
	 * @see org.scilab.modules.gui.tab.SimpleTab#setToolBar(org.scilab.modules.gui.toolbar.ToolBar)
	 */
	public void setToolBar(ToolBar newToolBar) {
		this.toolBar = newToolBar;
	}

	/**
	 * Getter for ToolBar
	 * @return ToolBar : the ToolBar associated to the Tab.
	 * @see org.scilab.modules.gui.tab.SimpleTab#getToolBar()
	 */
	public ToolBar getToolBar() {
		return this.toolBar;
	}
	
	/**
	 * Setter for InfoBar
	 * @param newInfoBar the InfoBar to set.
	 */
	public void setInfoBar(TextBox newInfoBar) {
		this.infoBar = newInfoBar;
	}

	/**
	 * Getter for InfoBar
	 * @return the InfoBar associated to the Tab.
	 */
	public TextBox getInfoBar() {
		return this.infoBar;
	}
	
	/**
	 * Set the callback of the tab
	 * @param callback the callback to set.
	 */
	public void setCallback(CallBack callback) {
		callback.putValue(Action.NAME, DockingConstants.CLOSE_ACTION);
		this.addAction(callback);
	}

}

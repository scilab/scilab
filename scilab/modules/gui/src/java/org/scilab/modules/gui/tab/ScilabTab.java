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

package org.scilab.modules.gui.tab;

import org.scilab.modules.gui.bridge.ScilabBridge;
import org.scilab.modules.gui.canvas.Canvas;
import org.scilab.modules.gui.checkbox.CheckBox;
import org.scilab.modules.gui.console.Console;
import org.scilab.modules.gui.container.ScilabContainer;
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
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.tree.Tree;
import org.scilab.modules.gui.uielement.UIElement;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.gui.utils.UIElementMapper;
import org.scilab.modules.gui.window.Window;

/**
 * Class for tabs in Scilab GUIs
 * @author Vincent COUVERT
 * @author Bruno JOFRET
 */
public class ScilabTab extends ScilabContainer implements Tab {

	/**
	**	Just let the Bridge do his job translating Scilab thinking to Java
	*/
	private SimpleTab component;
	
	/**
	 * Constructor
	 * @param name - name of the Tab
	 */
	protected ScilabTab(String name) {
		component = ScilabBridge.createTab(name);
	}
	
	/**
	 * Constructor for a graphical tab
	 * @param name - name of the Tab
	 * @param figureId index of the displayed figure
	 */
	protected ScilabTab(String name, int figureId) {
		component = ScilabBridge.createTab(name, figureId);
	}

	/**
	 * Creates a Scilab Tab
	 * @param name the name of the tab (used to identify it)
	 * @return the created Scilab Tab
	 */
	public static Tab createTab(String name) {
		return new ScilabTab(name);
	}
	
	/**
	 * Creates a Scilab Tab
	 * @param name the name of the tab (used to identify it)
	 * @param figureId index of the figure to display
	 * @return the created Scilab Tab
	 */
	public static Tab createTab(String name, int figureId) {
		return new ScilabTab(name, figureId);
	}

	/**
	 * Gets the GUI Tab.
	 * @return The GUI dummy Tab.
	 */
	public SimpleTab getAsSimpleTab() {
		return component;
	}

	/**
	 * Gets the title of a Scilab tab
	 * @return the title of the frame
	 * @see org.scilab.modules.gui.tab.Tab#getTitle()
	 */
	public String getName() {
		return ScilabBridge.getName(this);
	}

	/**
	 * Sets the name of a Scilab tab
	 * @param newTabName the name to set to the tab
	 * @see org.scilab.modules.gui.tab.Tab#setName(java.lang.String)
	 */
	public void setName(String newTabName) {
		ScilabBridge.setName(this, newTabName);
	}

	/**
	 * Draws a Scilab tab
	 * @see org.scilab.modules.gui.UIElement#draw()
	 */
	public void draw() {
		ScilabBridge.draw(this);
	}

	/**
	 * Gets the dimensions (width and height) of a Scilab tab
	 * @return the dimension of the tab
	 * @see org.scilab.modules.gui.UIElement#getDims()
	 */
	public Size getDims() {
		return ScilabBridge.getDims(this);
	}

	/**
	 * Gets the position (X-coordinate and Y-coordinate) of a Scilab tab
	 * @return the position of the tab
	 * @see org.scilab.modules.gui.UIElement#getPosition()
	 */
	public Position getPosition() {
		return ScilabBridge.getPosition(this);
	}

	/**
	 * Gets the visibility status of a Scilab frame
	 * @return the visibility status of the frame (true if the frame is visible, false if not)
	 * @see org.scilab.modules.gui.UIElement#isVisible()
	 */
	public boolean isVisible() {
		return ScilabBridge.isVisible(this);
	}

	/**
	 * Sets the dimensions (width and height) of a Scilab frame
	 * @param newSize the dimensiosn we want to set to the tab
	 * @see org.scilab.modules.gui.UIElement#setDims(org.scilab.modules.gui.utils.Size)
	 */
	public void setDims(Size newSize) {
		ScilabBridge.setDims(this, newSize);
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a Scilab frame
	 * @param newPosition the position we want to set to the tab
	 * @see org.scilab.modules.gui.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
	 */
	public void setPosition(Position newPosition) {
		ScilabBridge.setPosition(this, newPosition);
	}

	/**
	 * Sets the visibility status of a Scilab frame
	 * @param newVisibleState the visibility status we want to set to the frame (true to set the frame visible, false else)
	 * @see org.scilab.modules.gui.UIElement#setVisible(boolean)
	 */
	public void setVisible(boolean newVisibleState) {
		ScilabBridge.setVisible(this, newVisibleState);
	}

	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	public int addMember(Canvas member) {
		updateMenuBar(member);
		updateToolBar(member);
		updateInfoBar(member);
		return ScilabBridge.addMember(this, member);
	}

	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	public int addMember(Console member) {
		updateMenuBar(member);
		updateToolBar(member);
		updateInfoBar(member);
		return ScilabBridge.addMember(this, member);
	}
	
	/**
	 * We want to be able to add directly a HelpBrowser in a Tab.
	 * @param member the member to add
	 * @return the position of the HelpBrowser in the member list.
	 */
	public int addMember(HelpBrowser member) {
		return ScilabBridge.addMember(this, member);		
	}

	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 * @see org.scilab.modules.gui.container.Container#addMember(org.scilab.modules.gui.dockable.Dockable)
	 */
	@Override
	public int addMember(Dockable member) {
		// FIXME must be coded
		//return addMember(member);
		throw new UnsupportedOperationException();
	}
	
	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	public int addMember(Frame member) {
		return ScilabBridge.addMember(this, member);
	}

	/**
	 * Remove a Frame from a Tab.
	 * @param member the Frame to remove
	 */
	public void removeMember(Frame member) {
		ScilabBridge.removeMember(this, member);
	}

	/**
	 * We want to be able to add directly a pushbutton in a Tab.
	 * @param member the pushbutton to add
	 * @return the position of the pushbutton in the member list.
	 */
	public int addMember(PushButton member) {
		return ScilabBridge.addMember(this, member);
	}

	/**
	 * We want to be able to add directly a Tree Overview in a Tab.
	 * @param member the Tree Overview to add
	 * @return the position of the Tree Overview in the member list.
	 */
	public int addMember(Tree member) {
		return ScilabBridge.addMember(this, member);
	}
	
	/**
	 * Remove a PushButton from a Tab.
	 * @param member the pushbutton to remove
	 */
	public void removeMember(PushButton member) {
		ScilabBridge.removeMember(this, member);
	}

	/**
	 * We want to be able to add directly a editbox in a Tab.
	 * @param member the editbox to add
	 * @return the position of the editbox in the member list.
	 */
	public int addMember(EditBox member) {
		return ScilabBridge.addMember(this, member);
	}

	/**
	 * Remove an EditBox from a Tab.
	 * @param member the EditBox to remove
	 */
	public void removeMember(EditBox member) {
		ScilabBridge.removeMember(this, member);
	}

	/**
	 * We want to be able to add directly a label in a Tab.
	 * @param member the label to add
	 * @return the position of the label in the member list.
	 */
	public int addMember(Label member) {
		return ScilabBridge.addMember(this, member);
	}

	/**
	 * Remove a Label from a Tab.
	 * @param member the Label to remove
	 */
	public void removeMember(Label member) {
		ScilabBridge.removeMember(this, member);
	}

	/**
	 * We want to be able to add directly a checkbox in a Tab.
	 * @param member the checkbox to add
	 * @return the position of the checkbox in the member list.
	 */
	public int addMember(CheckBox member) {
		return ScilabBridge.addMember(this, member);
	}

	/**
	 * Remove a CheckBox from a Tab.
	 * @param member the CheckBox to remove
	 */
	public void removeMember(CheckBox member) {
		ScilabBridge.removeMember(this, member);
	}

	/**
	 * We want to be able to add directly a RadioButton in a Tab.
	 * @param member the RadioButton to add
	 * @return the position of the RadioButton in the member list.
	 */
	public int addMember(RadioButton member) {
		return ScilabBridge.addMember(this, member);
	}

	/**
	 * Remove a RadioButton from a Tab.
	 * @param member the RadioButton to remove
	 */
	public void removeMember(RadioButton member) {
		ScilabBridge.removeMember(this, member);
	}

	/**
	 * We want to be able to add directly a Slider in a Tab.
	 * @param member the Slider to add
	 * @return the position of the Slider in the member list.
	 */
	public int addMember(Slider member) {
		return ScilabBridge.addMember(this, member);
	}

	/**
	 * Remove a Slider from a Tab.
	 * @param member the Slider to remove
	 */
	public void removeMember(Slider member) {
		ScilabBridge.removeMember(this, member);
	}

	/**
	 * We want to be able to add directly a ListBox in a Tab.
	 * @param member the ListBox to add
	 * @return the position of the ListBox in the member list.
	 */
	public int addMember(ListBox member) {
		return ScilabBridge.addMember(this, member);
	}

	/**
	 * Remove a ListBox from a Tab.
	 * @param member the ListBox to remove
	 */
	public void removeMember(ListBox member) {
		ScilabBridge.removeMember(this, member);
	}

	/**
	 * We want to be able to add directly a PopupMenu in a Tab.
	 * @param member the PopupMenu to add
	 * @return the position of the PopupMenu in the member list.
	 */
	public int addMember(PopupMenu member) {
		return ScilabBridge.addMember(this, member);
	}
	
	/**
	 * Remove a PopupMenu from a Tab.
	 * @param member the PopupMenu to remove
	 */
	public void removeMember(PopupMenu member) {
		ScilabBridge.removeMember(this, member);
	}

	/**
	 * We want to be able to remove directly a Canvas from a Tab.
	 * @param member canvas to remove 
	 */
	public void removeMember(Canvas member) {
		ScilabBridge.removeMember(this, member);
	}
	
	/**
	 * Destroy the tab.
	 */
	public void close() {
		// remove the tab from the parent
		getParentWindow().removeTab(this);
		this.setParentWindowId(-1);
		
	}

	/**
	 * Get the current status of the Tab in its parent
	 * @return true is the tab is the tab currently "on top" in its parent
	 * @see org.scilab.modules.gui.tab.Tab#isCurrentTab()
	 */
	public boolean isCurrentTab() {
		return ScilabBridge.isCurrentTab(this);
	}

	/**
	 * Set the parent window id for this tab
	 * @param id the id of the parent window
	 * @see org.scilab.modules.gui.tab.Tab#setParentWindowId(int)
	 */
	public void setParentWindowId(int id) {
		ScilabBridge.setParentWindowId(this, id);
	}
	
	/**
	 * Get the parent window id for this tab
	 * @return the id of the parent window
	 * @see org.scilab.modules.gui.tab.Tab#getParentWindowId()
	 */
	public int getParentWindowId() {
		return ScilabBridge.getParentWindowId(this);
	}
	
	/**
	 * Update the Tab MenuBar.
	 * If the element added has its own, the Tab will take this
	 * one as its.
	 * @param element : the element added.
	 */
	private void updateMenuBar(UIElement element) {
		if (element.getMenuBar() != null) {
			this.addMenuBar(element.getMenuBar());
		}
	}
	
	/**
	 * Add a MenuBar to this Tab
	 * @Override the UIElement behavior
	 * Explicitly set the MenuBar in the Tab implementation.
	 * @param menuBar : The MenuBar to set.
	 * @see org.scilab.modules.gui.uielement.ScilabUIElement#addMenuBar(org.scilab.modules.gui.menubar.MenuBar)
	 */
	@Override
	public void addMenuBar(MenuBar menuBar) {
		super.addMenuBar(menuBar);
		this.getAsSimpleTab().setMenuBar(menuBar);
	}
	
	/**
	 * Update the Tab ToolBar.
	 * If the element added has its own, the Tab will take this
	 * one as its.
	 * @param element : the element added.
	 */
	private void updateToolBar(UIElement element) {
		if (element.getToolBar() != null) {
			this.addToolBar(element.getToolBar());
		}
	}
	
	/**
	 * Add a InfoBar to this Tab
	 * @Override the UIElement behavior
	 * Explicitly set the InfoBar in the Tab implementation.
	 * @param infoBar the InfoBar to set.
	 * @see org.scilab.modules.gui.uielement.ScilabUIElement#addInfoBar(org.scilab.modules.gui.textbox.TextBox)
	 */
	@Override
	public void addInfoBar(TextBox infoBar) {
		super.addInfoBar(infoBar);
		this.getAsSimpleTab().setInfoBar(infoBar);
	}
	
	/**
	 * Update the Tab InfoBar.
	 * If the element added has its own, the Tab will take this
	 * one as its.
	 * @param element : the element added.
	 */
	private void updateInfoBar(UIElement element) {
		if (element.getInfoBar() != null) {
			this.addInfoBar(element.getInfoBar());
		}
	}

	/**
	 * Add a ToolBar to this Tab
	 * @Override the UIElement behavior
	 * Explicitly set the ToolBar in the Tab implementation.
	 * @param toolBar : The ToolBar to set.
	 * @see org.scilab.modules.gui.uielement.ScilabUIElement#addToolBar(org.scilab.modules.gui.toolbar.ToolBar)
	 */
	@Override
	public void addToolBar(ToolBar toolBar) {
		super.addToolBar(toolBar);
		this.getAsSimpleTab().setToolBar(toolBar);
	}

	/**
	 * Set the callback of the tab
	 * @param callback the CallBack to set
	 */
	public void setCallback(CallBack callback) {
		ScilabBridge.setCallback(this, callback);
	}
	
	/**
	 * @return parent window of the tab object
	 */
	public Window getParentWindow() {
		return (Window) UIElementMapper.getCorrespondingUIElement(getParentWindowId());
	}
	
	/**
	 * Set this tab as the current tab of its parent Window
	 */
	public void setCurrent() {
		ScilabBridge.setCurrent(this);
	}
	
	/**
	 * Set the background color of the tab.
	 * @param red red channel of the color
	 * @param green green channel
	 * @param blue blue channel
	 */
	public void setBackground(double red, double green, double blue) {
		ScilabBridge.setBackground(this, red, green, blue);
	}
	
	/**
	  * Specify whether the canvas should fit the parent tab size
	  * (and consequently the scrollpane size) or not
	  * @param onOrOff true to enable autoresize mode
	  */
	 public void setAutoResizeMode(boolean onOrOff) {
		 ScilabBridge.setAutoResizeMode(this, onOrOff);
	 }

	 /**
	  * @return whether the resize mode is on or off
	  */
	 public boolean getAutoResizeMode() {
		 return ScilabBridge.getAutoResizeMode(this);
	 }
	 
	 /**
	  * Get the part of the axes which is currently viewed
	  * @return [x,y,w,h] array
	  */
	 public int[] getViewingRegion() {
		 return ScilabBridge.getViewingRegion(this);
	 }
	 
	 /**
	  * Specify a new viewport for the axes
	  * For SwingScilabCanvas viewport can not be modified
	  * since it match the parent tab size
	  * @param posX X coordinate of upper left point of the viewport within the canvas
	  * @param posY Y coordinate of upper left point of the viewport within the canvas
	  * @param width width of the viewport
	  * @param height height of the viewport
	  */
	 public void setViewingRegion(int posX, int posY, int width, int height) {
		 ScilabBridge.setViewingRegion(this, posX, posY, width, height);
	 }
	 
	 
	 /**
	  * @return size of the axes in pixels
	  */
	 public Size getAxesSize() {
		 return ScilabBridge.getAxesSize(this);
	 }
	 
	 
	 /**
	  * @param newSize set a new axes size
	  */
	 public void setAxesSize(Size newSize) {
		 ScilabBridge.setAxesSize(this, newSize);
	 }
	 
	 /**
	  * Set the event handler of the Canvas
	  * @param command the name of the Scilab function to call
	  */
	 public void setEventHandler(String command) {
		 ScilabBridge.setEventHandler(this, command);
	 }

	 /**
	  * Set the status of the event handler of the Canvas
	  * @param status is true to set the event handler active
	  */
	 public void setEventHandlerEnabled(boolean status) {
		 ScilabBridge.setEventHandlerEnabled(this, status);
	 }
	 
	 /**
	  * Get the displacement in pixel that should be used for rotating axes
	  * @param displacement out parameter, [x,y] array of displacement in pixels
	  * @return true if the diplacement recording continue, false otherwise
	  */
	 public boolean getRotationDisplacement(int[] displacement) {
		 return ScilabBridge.getRotationDisplacement(this, displacement);
	 }

	 /**
	  * Asynchronous stop of rotation tracking.
	  */
	 public void stopRotationRecording() {
		 ScilabBridge.stopRotationRecording(this);
	 }

}


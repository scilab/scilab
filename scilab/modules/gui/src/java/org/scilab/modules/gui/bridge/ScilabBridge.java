/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent Couvert
 * Copyright (C) 2007 - INRIA - Bruno Jofret
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


package org.scilab.modules.gui.bridge;

import java.awt.Color;
import java.awt.Cursor;
import java.awt.Font;
import java.awt.image.BufferedImage;

import javax.media.opengl.GL;

import org.scilab.modules.gui.canvas.Canvas;
import org.scilab.modules.gui.canvas.ScilabCanvas;
import org.scilab.modules.gui.canvas.ScilabCanvasBridge;
import org.scilab.modules.gui.canvas.SimpleCanvas;
import org.scilab.modules.gui.checkbox.CheckBox;
import org.scilab.modules.gui.checkbox.ScilabCheckBoxBridge;
import org.scilab.modules.gui.checkbox.SimpleCheckBox;
import org.scilab.modules.gui.checkboxmenuitem.CheckBoxMenuItem;
import org.scilab.modules.gui.checkboxmenuitem.ScilabCheckBoxMenuItemBridge;
import org.scilab.modules.gui.checkboxmenuitem.SimpleCheckBoxMenuItem;
import org.scilab.modules.gui.colorchooser.ColorChooser;
import org.scilab.modules.gui.colorchooser.ScilabColorChooserBridge;
import org.scilab.modules.gui.colorchooser.SimpleColorChooser;
import org.scilab.modules.gui.console.Console;
import org.scilab.modules.gui.console.ScilabConsoleBridge;
import org.scilab.modules.gui.console.SimpleConsole;
import org.scilab.modules.gui.contextmenu.ContextMenu;
import org.scilab.modules.gui.contextmenu.ScilabContextMenuBridge;
import org.scilab.modules.gui.contextmenu.SimpleContextMenu;
import org.scilab.modules.gui.editbox.EditBox;
import org.scilab.modules.gui.editbox.ScilabEditBoxBridge;
import org.scilab.modules.gui.editbox.SimpleEditBox;
import org.scilab.modules.gui.events.callback.CallBack;
import org.scilab.modules.gui.filechooser.FileChooser;
import org.scilab.modules.gui.filechooser.ScilabFileChooserBridge;
import org.scilab.modules.gui.filechooser.SimpleFileChooser;
import org.scilab.modules.gui.fontchooser.FontChooser;
import org.scilab.modules.gui.fontchooser.ScilabFontChooserBridge;
import org.scilab.modules.gui.fontchooser.SimpleFontChooser;
import org.scilab.modules.gui.frame.Frame;
import org.scilab.modules.gui.frame.ScilabFrameBridge;
import org.scilab.modules.gui.frame.SimpleFrame;
import org.scilab.modules.gui.helpbrowser.HelpBrowser;
import org.scilab.modules.gui.helpbrowser.ScilabHelpBrowserBridge;
import org.scilab.modules.gui.helpbrowser.SimpleHelpBrowser;
import org.scilab.modules.gui.label.Label;
import org.scilab.modules.gui.label.ScilabLabelBridge;
import org.scilab.modules.gui.label.SimpleLabel;
import org.scilab.modules.gui.layout.BorderLayout;
import org.scilab.modules.gui.layout.FlowLayout;
import org.scilab.modules.gui.layout.ScilabBorderLayoutBridge;
import org.scilab.modules.gui.layout.ScilabFlowLayoutBridge;
import org.scilab.modules.gui.listbox.ListBox;
import org.scilab.modules.gui.listbox.ScilabListBoxBridge;
import org.scilab.modules.gui.listbox.SimpleListBox;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menu.ScilabMenuBridge;
import org.scilab.modules.gui.menu.SimpleMenu;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.menubar.ScilabMenuBarBridge;
import org.scilab.modules.gui.menubar.SimpleMenuBar;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.menuitem.ScilabMenuItemBridge;
import org.scilab.modules.gui.menuitem.SimpleMenuItem;
import org.scilab.modules.gui.messagebox.MessageBox;
import org.scilab.modules.gui.messagebox.ScilabMessageBoxBridge;
import org.scilab.modules.gui.messagebox.SimpleMessageBox;
import org.scilab.modules.gui.popupmenu.PopupMenu;
import org.scilab.modules.gui.popupmenu.ScilabPopupMenuBridge;
import org.scilab.modules.gui.popupmenu.SimplePopupMenu;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.gui.pushbutton.ScilabPushButtonBridge;
import org.scilab.modules.gui.pushbutton.SimplePushButton;
import org.scilab.modules.gui.radiobutton.RadioButton;
import org.scilab.modules.gui.radiobutton.ScilabRadioButtonBridge;
import org.scilab.modules.gui.radiobutton.SimpleRadioButton;
import org.scilab.modules.gui.separator.ScilabSeparatorBridge;
import org.scilab.modules.gui.separator.Separator;
import org.scilab.modules.gui.slider.ScilabSliderBridge;
import org.scilab.modules.gui.slider.SimpleSlider;
import org.scilab.modules.gui.slider.Slider;
import org.scilab.modules.gui.tab.ScilabTabBridge;
import org.scilab.modules.gui.tab.SimpleTab;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.textbox.ScilabTextBoxBridge;
import org.scilab.modules.gui.textbox.SimpleTextBox;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ScilabToolBarBridge;
import org.scilab.modules.gui.toolbar.SimpleToolBar;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.tree.ScilabTreeBridge;
import org.scilab.modules.gui.tree.SimpleTree;
import org.scilab.modules.gui.tree.Tree;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.gui.waitbar.ScilabWaitBarBridge;
import org.scilab.modules.gui.waitbar.SimpleWaitBar;
import org.scilab.modules.gui.waitbar.WaitBar;
import org.scilab.modules.gui.window.ScilabWindowBridge;
import org.scilab.modules.gui.window.SimpleWindow;
import org.scilab.modules.gui.window.Window;

/**
 * Main bridge for Scilab Java GUIs
 * @author Vincent COUVERT
 * @author Bruno Jofret
 */
public class ScilabBridge {

	/**
	 * Constructor for ScilabBridge class
	 */
	protected ScilabBridge() {
		throw new UnsupportedOperationException(); /* Prevents calls from subclass */
	}

	/*****************/
	/* Window Bridge */
	/*****************/

	/**
	 * Creates a new window
	 * @return the created window
	 */
	public static SimpleWindow createWindow() {
		return ScilabWindowBridge.createWindow();
	}

	/**
	 * Draws a window on screen
	 * @param window the window to be drawn
	 */
	public static void draw(Window window) {
		ScilabWindowBridge.draw(window);
	}
	
	/**
	 * Deiconify the window and put it in front of other window
	 * @param window the window to raise
	 */
	public static void raise(Window window) {
		ScilabWindowBridge.raise(window);
	}

	/**
	 * Gets the dimensions (width and height) of a window
	 * @param window the window we want to get the dimensions of
	 * @return dimensions of the window
	 */
	public static Size getDims(Window window) {
		return ScilabWindowBridge.getDims(window);
	}

	/**
	 * Sets the dimensions (width and height) of a window
	 * @param window the window we want to set the dimensions of
	 * @param newWindowSize new dimensions of the window
	 */
	public static void setDims(Window window, Size newWindowSize) {
		ScilabWindowBridge.setDims(window, newWindowSize);
	}

	/**
	 * Gets the position (X-coordinate and Y-coordinate) of a window
	 * @param window the window we want to get the position of
	 * @return position of the window
	 */
	public static Position getPosition(Window window) {
		return ScilabWindowBridge.getPosition(window);
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a window
	 * @param window the window we want to set the position of
	 * @param newWindowPosition new position of the window
	 */
	public static void setPosition(Window window, Position newWindowPosition) {
		ScilabWindowBridge.setPosition(window, newWindowPosition);
	}

	/**
	 * Gets the title of a window
	 * @param window the window we want to get the title of
	 * @return the title of the window
	 */
	public static String getTitle(Window window) {
		return ScilabWindowBridge.getTitle(window);
	}

	/**
	 * Sets the title of a window
	 * @param window the window we want to set the title of
	 * @param newWindowTitle the title to set for the window
	 */
	public static void setTitle(Window window, String newWindowTitle) {
		ScilabWindowBridge.setTitle(window, newWindowTitle);
	}

	/**
	 * Gets the visibility status of a window
	 * @param window the window we want to get the status of
	 * @return visibility status (true if the window is visible, false if not)
	 */
	public static boolean isVisible(Window window) {
		return ScilabWindowBridge.isVisible(window);
	}

	/**
	 * Sets the visibility status for a window
	 * @param window the window we want to set the visibility status of
	 * @param newVisibleState new visibility status for the window
	 * 			(true set the window visible, false to set the window invisible)
	 */
	public static void setVisible(Window window, boolean newVisibleState) {
		ScilabWindowBridge.setVisible(window, newVisibleState);
	}

	/**
	 * Add a tab to a window
	 * @param window the window which we want to add the tab to
	 * @param newTab the tab to add to the window
	 */
	public static void addTab(Window window, Tab newTab) {
		ScilabWindowBridge.addTab(window, newTab);
	}
	
	/**
	 * Add a tab to a window
	 * @param window the window which we want to remove the tab from
	 * @param newTab the tab to add to the window
	 */
	public static void removeTab(Window window, Tab newTab) {
		ScilabWindowBridge.removeTab(window, newTab);
	}

	/**
	 * Sets a MenuBar to a window
	 * @param window the window which we want to add the MeunBar to
	 * @param newMenuBar the MeunBar to add to the window
	 */
	public static void addMenuBar(Window window, MenuBar newMenuBar) {
		ScilabWindowBridge.addMenuBar(window, newMenuBar);
	}
	
	/**
	 * Sets a ToolBar to a window
	 * @param window the window which we want to add the ToolBar to
	 * @param newToolBar the ToolBar to add to the window
	 */
	public static void addToolBar(Window window, ToolBar newToolBar) {
		ScilabWindowBridge.addToolBar(window, newToolBar);
	}
	
	/**
	 * Sets a InfoBar to a window
	 * @param window the window which we want to add the InfoBar to
	 * @param newInfoBar the InfoBar to add to the window
	 */
	public static void addInfoBar(Window window, TextBox newInfoBar) {
		ScilabWindowBridge.addInfoBar(window, newInfoBar);
	}
	
	/**
	 * @param window window from which we want the information
	 * @return number of objects (tabs) docked in this window
	 */
	public static int getNbDockedObjects(Window window) {
		return ScilabWindowBridge.getNbDockedObjects(window);
	}
	
	/**
	 * Update the dimension of the window and its component.
	 * Only useful when the window is not yet visible
	 * @param window window to update
	 */
	public static void updateDimensions(Window window) {
		ScilabWindowBridge.updateDimensions(window);
	}
	
	/*****************/
	/* ToolBar Bridge */
	/*****************/
	
	/**
	 * Creates a new ToolBar
	 * @return the created ToolBar
	 */
	public static SimpleToolBar createToolBar() {
		return ScilabToolBarBridge.createToolBar();
	}
	
	/**
	 * Add a PushButton to the ToolBar.
	 * @param toolBar the toolBar which we want to add the PushButton to
	 * @param pushButton the PushButton to add.
	 */
	public static void add(ToolBar toolBar, PushButton pushButton) {
		ScilabToolBarBridge.add(toolBar, pushButton);
	}

	/**
	 * Add a Separator to a Toolbar
	 * @param toolbar the Toolbar which we want to add the Separator to
	 */
	public static void addSeparator(ToolBar toolbar) {
		ScilabToolBarBridge.addSeparator(toolbar);
	}

	/**
	 * Set the visibility of the Toolbar
	 * @param toolbar the ToolBar
	 * @param newVisibleState true to set the toolbar visible
	 */
	public static void setVisible(ToolBar toolbar, boolean newVisibleState) {
		ScilabToolBarBridge.setVisible(toolbar, newVisibleState);
	}
	
	/**
	 * Get the visibility of the TooBar
	 * @param toolbar the ToolBar
	 * @return true if the ToolBar is Visible
	 */
	public static boolean isVisible(ToolBar toolbar) {
		return ScilabToolBarBridge.isVisible(toolbar);
	}

	
	/****************/
	/* Frame Bridge */
	/****************/

	/**
	 * Creates a new Frame
	 * @return the created Frame
	 */
	public static SimpleFrame createFrame() {
		return ScilabFrameBridge.createFrame();
	}
	
	/**
	 * Sets the Text of a Frame
	 * @param frame the Frame we want to set the Text of
	 * @param newText the Text to set for the Frame
	 */
	public static void setText(Frame frame, String newText) {
		ScilabFrameBridge.setText(frame, newText);
	}
	
	/**
	 * Gets the text of an Frame
	 * @param frame the Frame we want to get the text of
	 * @return the text of the Frame
	 */
	public static String getText(Frame frame) {
		return ScilabFrameBridge.getText(frame);
	}
	
	/**
	 * Draws a Scilab Frame
	 * @param frame the Frame to draw
	 * @see org.scilab.modules.ihm.UIElement#draw()
	 */
	public static void draw(Frame frame) {
		ScilabFrameBridge.draw(frame);
	}

	/**
	 * Gets the dimensions (width and height) of a Scilab Frame
	 * @param frame the Frame we want to get the dimensions of
	 * @return the size of the frame
	 * @see org.scilab.modules.ihm.UIElement#getDims()
	 */
	public static Size getDims(Frame frame) {
		return ScilabFrameBridge.getDims(frame);
	}

	/**
	 * Gets the position (X-coordinate and Y-coordinate) of a Scilab frame
	 * @param frame the frame we want to get the position of
	 * @return the position of the frame
	 * @see org.scilab.modules.ihm.UIElement#getPosition()
	 */
	public static Position getPosition(Frame frame) {
		return ScilabFrameBridge.getPosition(frame);
	}

	/**
	 * Gets the visibility status of a Scilab Frame
	 * @param frame the frame we want to get the visiblity status of
	 * @return the visibility status of the frame (true if the frame is visible, false if not)
	 * @see org.scilab.modules.ihm.UIElement#isVisible()
	 */
	public static boolean isVisible(Frame frame) {
		return ScilabFrameBridge.isVisible(frame);
	}

	/**
	 * Sets the dimensions (width and height) of a Scilab Frame
	 * @param frame the frame we want to set the dimensions of
	 * @param newSize the size we want to set to the frame
	 * @see org.scilab.modules.ihm.UIElement#setDims(org.scilab.modules.ihm.utils.Size)
	 */
	public static void setDims(Frame frame, Size newSize) {
		ScilabFrameBridge.setDims(frame, newSize);
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a Scilab frame
	 * @param frame the frame we want to set the position of
	 * @param newPosition the position we want to set to the frame
	 * @see org.scilab.modules.ihm.UIElement#setPosition(org.scilab.modules.ihm.utils.Position)
	 */
	public static void setPosition(Frame frame, Position newPosition) {
		ScilabFrameBridge.setPosition(frame, newPosition);
	}

	/**
	 * Sets the visibility status of a Scilab Frame
	 * @param frame the frame we want to set the visiblity status of
	 * @param newVisibleState the visibility status we want to set to the frame (true to set the frame visible, false else)
	 * @see org.scilab.modules.ihm.UIElement#setVisible(boolean)
	 */
	public static void setVisible(Frame frame, boolean newVisibleState) {
		ScilabFrameBridge.setVisible(frame, newVisibleState);
	}
	
	/**
	 * Set the Background color of the Frame
	 * @param frame the Frame we want to set the background of
	 * @param color the Color
	 */
	public static void setBackground(Frame frame, Color color) {
		ScilabFrameBridge.setBackground(frame, color);
	}

	/**
	 * Get the Background color of the Frame
	 * @param frame the Frame we want to get the background of
	 * @return the Color
	 */
	public static Color getBackground(Frame frame) {
		return ScilabFrameBridge.getBackground(frame);
	}

	/**
	 * Set the Foreground color of the Frame
	 * @param frame the Frame we want to set the foreground of
	 * @param color the Color
	 */
	public static void setForeground(Frame frame, Color color) {
		ScilabFrameBridge.setForeground(frame, color);
	}

	/**
	 * Get the Foreground color of the Frame
	 * @param frame the Frame we want to get the foreground of
	 * @return the Color
	 */
	public static Color getForeground(Frame frame) {
		return ScilabFrameBridge.getForeground(frame);
	}

	/**
	 * Set the font of the Frame.
	 * @param frame the Frame we want to set the font of
	 * @param font the font
	 */
	public static void setFont(Frame frame, Font font) {
		ScilabFrameBridge.setFont(frame, font);
	}
	
	/**
	 * Get the font of the Frame.
	 * @param frame the Frame we want to get the font of
	 * @return the font
	 */
	public static Font getFont(Frame frame) {
		return ScilabFrameBridge.getFont(frame);
	}

	/**
	 * Set if the Frame is enabled or not
	 * @param frame the Frame we want to set the status of
	 * @param status true if the Frame is enabled
	 */
	public static void setEnabled(Frame frame, boolean status) {
		ScilabFrameBridge.setEnabled(frame, status);
	}
	
	/**
	 * Gets the enable status of the Frame
	 * @param frame the Frame we want to get the status of
	 * @return the enable status of the Frame (true if the Frame is enabled, false if not)
	 */
	public static boolean isEnabled(Frame frame) {
		return ScilabFrameBridge.isEnabled(frame);
	}
	
	/**
	 * Add a callback to the Frame
	 * @param frame the Frame we want to set the callback of
	 * @param callback the callback to set
	 */
	public static void setCallback(Frame frame, CallBack callback) {
		ScilabFrameBridge.setCallback(frame, callback);
	}

	/**
	 * Set the Relief of the Frame
	 * @param frame the Frame we want to set the Relief of
	 * @param reliefType the type of the relief to set (See ScilabRelief.java)
	 */
	public static void setRelief(Frame frame, String reliefType) {
		ScilabFrameBridge.setRelief(frame, reliefType);
	}

	/**
	 * Delete the Frame
	 * @param frame the Frame to be destroyed
	 */
	public static void destroy(Frame frame) {
		ScilabFrameBridge.destroy(frame);
	}

	/**
	 * Give the focus to the Frame
	 * @param frame the Frame
	 */
	public static void requestFocus(Frame frame) {
		ScilabFrameBridge.requestFocus(frame);
	}

	/**************/
	/* Tab Bridge */
	/**************/

	/**
	 * Creates a Scilab tab
	 * @param name the name of the tab (used to identify it)
	 * @return the created tab
	 */
	public static SimpleTab createTab(String name) {
		return ScilabTabBridge.createTab(name);
	}
	
	/**
	 * Creates a Scilab tab able to create graphics
	 * @param name the name of the tab (used to identify it)
	 * @param figureId index of the created figure
	 * @return the created tab
	 */
	public static SimpleTab createTab(String name, int figureId) {
		return ScilabTabBridge.createTab(name, figureId);
	}

	/**
	 * Draws a tab on screen
	 * @param tab the tab to be drawn
	 */
	public static void draw(Tab tab) {
		ScilabTabBridge.draw(tab);
	}

	/**
	 * Gets the dimensions (width and height) of a tab
	 * @param tab the tab we want to get the dimensions of
	 * @return the dimensions of the tab
	 */
	public static Size getDims(Tab tab) {
		return ScilabTabBridge.getDims(tab);
	}

	/**
	 * Sets the dimensions (width and height) of a tab
	 * @param tab the tab we want to set the dimensions of
	 * @param newTabSize new dimensions of the tab
	 */
	public static void setDims(Tab tab, Size newTabSize) {
		ScilabTabBridge.setDims(tab, newTabSize);
	}

	/**
	 * Gets the position (X-coordinate and Y-coordinate) of a tab
	 * @param tab the tab we want to get the position of
	 * @return position of the tab
	 */
	public static Position getPosition(Tab tab) {
		return ScilabTabBridge.getPosition(tab);
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a tab
	 * @param tab the tab we want to set the position of
	 * @param newTabPosition new position of the tab
	 */
	public static void setPosition(Tab tab, Position newTabPosition) {
		ScilabTabBridge.setPosition(tab, newTabPosition);
	}

	/**
	 * Gets the Name of a tab
	 * @param tab the tab we want to get the Name of
	 * @return the Name of the tab
	 */
	public static String getName(Tab tab) {
		return ScilabTabBridge.getName(tab);
	}

	/**
	 * Sets the name of a tab
	 * @param tab the tab we want to set the name of
	 * @param newTabName the name to set for the tab
	 */
	public static void setName(Tab tab, String newTabName) {
		ScilabTabBridge.setName(tab, newTabName);
	}

	/**
	 * Gets the visibility status of a tab
	 * @param tab the tab we want to get the status of
	 * @return visibility status (true if the tab is visible, false if not)
	 */
	public static boolean isVisible(Tab tab) {
		return ScilabTabBridge.isVisible(tab);
	}

	/**
	 * Sets the visibility status of a tab
	 * @param tab the tab we want to set the visibility status of
	 * @param newVisibleState new visibility status for the tab
	 * 			(true set the tab visible, false to set the tab invisible)
	 */
	public static void setVisible(Tab tab, boolean newVisibleState) {
		ScilabTabBridge.setVisible(tab, newVisibleState);
	}

	/**
	 * Add a member (dockable element) to a tab and returns the index of this member
	 * @param tab the tab where we want to add the member
	 * @param member the member to add
 	 * @return index of member
 	 */
	public static int addMember(Tab tab, Frame member) {
		return ScilabTabBridge.addMember(tab, member);
	}

	/**
	 * Remove a Frame member from a tab
	 * @param tab the tab which we want to remove the Frame from
	 * @param member the Frame to remove
	 */
	public static void removeMember(Tab tab, Frame member) {
		ScilabTabBridge.removeMember(tab, member);
	}
	
	/**
	 * Add a member (dockable element) to a tab and returns the index of this member
	 * @param tab the tab where we want to add the member
	 * @param member the member to add
 	 * @return index of member
 	 */
	public static int addMember(Tab tab, Console member) {
		return ScilabTabBridge.addMember(tab, member);
	}

	/**
	 * We want to be able to add directly a HelpBrowser in a Tab.
	 * @param tab the tab where we want to add the member
	 * @param member the member to add
	 * @return the position of the HelpBrowser in the member list.
	 */
	public static int addMember(Tab tab, HelpBrowser member) {
		return ScilabTabBridge.addMember(tab, member);
	}
	
	/**
	 * Add a member (dockable element) to a tab and returns the index of this member
	 * @param tab the tab where we want to add the member
	 * @param member the member to add
 	 * @return index of member
 	 */
	public static int addMember(Tab tab, Canvas member) {
		return ScilabTabBridge.addMember(tab, member);
	}
	
	/**
	 * Sets a MenuBar to a tab
	 * @param tab the tab which we want to add the MenuBar to
	 * @param newMenuBar the MenuBar to add to the tab
	 */
	public static void addMenuBar(Tab tab, MenuBar newMenuBar) {
		ScilabTabBridge.addMenuBar(tab, newMenuBar);
	}
	
	/**
	 * Add a member (dockable element) to a tab and returns the index of this member
	 * @param tab the tab which we want to add the Tree Overview to
	 * @param member the Tree Overview to add
	 * @return the position of the Tree Overview in the member list.
	 */
	public static int addMember(Tab tab, Tree member) {
		return ScilabTabBridge.addMember(tab, member);
	}

	/**
	 * Add a member (dockable element) to a tab and returns the index of this member
	 * @param tab the tab which we want to add the pushbutton to
	 * @param member the pushbutton to add
	 * @return the position of the pushbutton in the member list.
	 */
	public static int addMember(Tab tab, PushButton member) {
		return ScilabTabBridge.addMember(tab, member);
	}
	
	/**
	 * Remove a PushButton member from a tab
	 * @param tab the tab which we want to remove the PushButton from
	 * @param member the PushButton to remove
	 */
	public static void removeMember(Tab tab, PushButton member) {
		ScilabTabBridge.removeMember(tab, member);
	}
	
	/**
	 * Add a member (dockable element) to a tab and returns the index of this member
	 * @param tab the tab which we want to add the EditBox to
	 * @param member the editbox to add
	 * @return the position of the editbox in the member list.
	 */
	public static int addMember(Tab tab, EditBox member) {
		return ScilabTabBridge.addMember(tab, member);
	}
	
	/**
	 * Remove an EditBox member from a tab
	 * @param tab the tab which we want to remove the EditBox from
	 * @param member the EditBox to remove
	 */
	public static void removeMember(Tab tab, EditBox member) {
		ScilabTabBridge.removeMember(tab, member);
	}
	
	/**
	 * Add a member (dockable element) to a tab and returns the index of this member
	 * @param tab the tab which we want to add the Label to
	 * @param member the label to add
	 * @return the position of the label in the member list.
	 */
	public static int addMember(Tab tab, Label member) {
		return ScilabTabBridge.addMember(tab, member);
	}
	
	/**
	 * Remove a Label member from a tab
	 * @param tab the tab which we want to remove the Label from
	 * @param member the Label to remove
	 */
	public static void removeMember(Tab tab, Label member) {
		ScilabTabBridge.removeMember(tab, member);
	}
	
	/**
	 * Add a member (dockable element) to a tab and returns the index of this member
	 * @param tab the tab which we want to add the CheckBox to
	 * @param member the checkbox to add
	 * @return the position of the checkbox in the member list.
	 */
	public static int addMember(Tab tab, CheckBox member) {
		return ScilabTabBridge.addMember(tab, member);
	}
	
	/**
	 * Remove a CheckBox member from a tab
	 * @param tab the tab which we want to remove the CheckBox from
	 * @param member the CheckBox to remove
	 */
	public static void removeMember(Tab tab, CheckBox member) {
		ScilabTabBridge.removeMember(tab, member);
	}
	
	/**
	 * Add a member (dockable element) to a tab and returns the index of this member
	 * @param tab the tab which we want to add the RadioButton to
	 * @param member the RadioButton to add
	 * @return the position of the RadioButton in the member list.
	 */
	public static int addMember(Tab tab, RadioButton member) {
		return ScilabTabBridge.addMember(tab, member);
	}
	
	/**
	 * Remove a RadioButton member from a tab
	 * @param tab the tab which we want to remove the RadioButton from
	 * @param member the RadioButton to remove
	 */
	public static void removeMember(Tab tab, RadioButton member) {
		ScilabTabBridge.removeMember(tab, member);
	}
	
	/**
	 * Add a member (dockable element) to a tab and returns the index of this member
	 * @param tab the tab which we want to add the Slider to
	 * @param member the Slider to add
	 * @return the position of the Slider in the member list.
	 */
	public static int addMember(Tab tab, Slider member) {
		return ScilabTabBridge.addMember(tab, member);
	}
	
	/**
	 * Remove a Slider member from a tab
	 * @param tab the tab which we want to remove the Slider from
	 * @param member the Slider to remove
	 */
	public static void removeMember(Tab tab, Slider member) {
		ScilabTabBridge.removeMember(tab, member);
	}
	
	/**
	 * Add a member (dockable element) to a tab and returns the index of this member
	 * @param tab the tab which we want to add the ListBox to
	 * @param member the ListBox to add
	 * @return the position of the ListBox in the member list.
	 */
	public static int addMember(Tab tab, ListBox member) {
		return ScilabTabBridge.addMember(tab, member);
	}
	
	/**
	 * Remove a ListBox member from a tab
	 * @param tab the tab which we want to remove the ListBox from
	 * @param member the ListBox to remove
	 */
	public static void removeMember(Tab tab, ListBox member) {
		ScilabTabBridge.removeMember(tab, member);
	}
	
	/**
	 * Add a member (dockable element) to a tab and returns the index of this member
	 * @param tab the tab which we want to add the PopupMenu to
	 * @param member the PopupMenu to add
	 * @return the position of the PopupMenu in the member list.
	 */
	public static int addMember(Tab tab, PopupMenu member) {
		return ScilabTabBridge.addMember(tab, member);
	}
	
	/**
	 * Remove a PopupMenu member from a tab
	 * @param tab the tab which we want to remove the PopupMenu from
	 * @param member the PopupMenu to remove
	 */
	public static void removeMember(Tab tab, PopupMenu member) {
		ScilabTabBridge.removeMember(tab, member);
	}
	
	/**
	 * Remove a member (dockable element) to a tab and returns the index of this member
	 * @param tab the tab where we want to add the member
	 * @param member the member to add
 	 */
	public static void removeMember(Tab tab, Canvas member) {
		ScilabTabBridge.removeMember(tab, member);
	}
	
	/**
	 * Get the current status of the Tab in its parent
	 * @param tab the tab we want to get the status of
	 * @return true is the tab is the tab currently displayed in its parent
	 */
	public static boolean isCurrentTab(Tab tab) {
		return ScilabTabBridge.isCurrentTab(tab);
	}

	/**
	 * Set the parent window id for this tab
	 * @param tab the tab we want to set the parent window id of
	 * @param id the id of the parent window
	 */
	public static void setParentWindowId(Tab tab, int id) {
		ScilabTabBridge.setParentWindowId(tab, id);
	}
	
	/**
	 * Get the parent window id for this tab
	 * @param tab the tab we want to get the parent window id of
	 * @return the id of the parent window
	 */
	public static int getParentWindowId(Tab tab) {
		return ScilabTabBridge.getParentWindowId(tab);
	}
	
	/**
	 * Set the callback of the tab
	 * @param tab the tab which we want to set the callback of
	 * @param callback the CallBack to set
	 */
	public static void setCallback(Tab tab, CallBack callback) {
		ScilabTabBridge.setCallback(tab, callback);
	}
	
	/**
	 * Set this tab as the current tab of its parent Window
	 * @param tab the tab
	 */
	public static void setCurrent(Tab tab) {
		ScilabTabBridge.setCurrent(tab);
	}
	
	/**
	 * Set the background color of a tab.
	 * @param tab tab to modify
	 * @param red red channel of the color
	 * @param green green channel
	 * @param blue blue channel
	 */
	public static void setBackground(Tab tab, double red, double green, double blue) {
		ScilabTabBridge.setBackground(tab, red, green, blue);
	}
	
	/**
	  * Specify whether the canvas should fit the parent tab size
	  * (and consequently the scrollpane size) or not
	  * @param tab tab to modify
	  * @param onOrOff true to enable autoresize mode
	  */
	 public static void setAutoResizeMode(Tab tab, boolean onOrOff) {
		 ScilabTabBridge.setAutoResizeMode(tab, onOrOff);
	 }

	 /**
	  * @param tab tab to modify
	  * @return whether the resize mode is on or off
	  */
	 public static boolean getAutoResizeMode(Tab tab) {
		 return ScilabTabBridge.getAutoResizeMode(tab);
	 }
	 
	 /**
	  * Get the part of the axes which is currently viewed
	  * @param tab tab to modify
	  * @return [x,y,w,h] array
	  */
	 public static int[] getViewingRegion(Tab tab) {
		 return ScilabTabBridge.getViewingRegion(tab);
	 }
	 
	 /**
	  * Specify a new viewport for the axes
	  * Only works if autoresize mode is off
	  * @param tab tab to modify
	  * @param posX X coordinate of upper left point of the viewport within the canvas
	  * @param posY Y coordinate of upper left point of the viewport within the canvas
	  * @param width width of the viewport
	  * @param height height of the viewport
	  */
	 public static void setViewingRegion(Tab tab, int posX, int posY, int width, int height) {
		 ScilabTabBridge.setViewingRegion(tab, posX, posY, width, height);
	 }
	 
	 
	 /**
	  * @param tab tab to modify
	  * @return size of the axes in pixels
	  */
	 public static Size getAxesSize(Tab tab) {
		 return ScilabTabBridge.getAxesSize(tab);
	 }
	 
	 
	 /**
	  * @param tab tab to modify
	  * @param newSize set a new axes size
	  */
	 public static void setAxesSize(Tab tab, Size newSize) {
		 ScilabTabBridge.setAxesSize(tab, newSize);
	 }
	 
	 /**
	  * Set the event handler of the Axes
	  * @param tab tab to modify
	  * @param command the name of the Scilab function to call
	  */
	 public static void setEventHandler(Tab tab, String command) {
		 ScilabTabBridge.setEventHandler(tab, command);
	 }

	 /**
	  * Set the status of the event handler of the Axes
	  * @param tab tab to modify
	  * @param status is true to set the event handler active
	  */
	 public static void setEventHandlerEnabled(Tab tab, boolean status) {
		 ScilabTabBridge.setEventHandlerEnabled(tab, status);
	 }
	 
	 

	/******************/
	/* Console Bridge */
	/******************/

	/**
	 * Creates a Scilab Console
	 * @return the created console
	 */
	public static SimpleConsole createConsole() {
		return ScilabConsoleBridge.createConsole();
	}

	/**
	 * Displays data in the console
	 * @param console the console to display the data in
	 * @param dataToDisplay the data to be displayed
	 * @see org.scilab.modules.console.HelpBrowser#display()
	 */
	public static void display(Console console, String dataToDisplay) {
		ScilabConsoleBridge.display(console, dataToDisplay);
	}

	/**
	 * Reads input data in the console
	 * @param console the console to read the data from
	 * @return the data entered by the user
	 * @see org.scilab.modules.console.HelpBrowser#readLine()
	 */
	public static String readLine(Console console) {
		return ScilabConsoleBridge.readLine(console);
	}

	/**
	 * Draw a console
	 * @param console the console to draw
	 * @see org.scilab.modules.ihm.UIElement#draw()
	 */
	public static void draw(Console console) {
		ScilabConsoleBridge.draw(console);
	}

	/**
	 * Gets the dimensions (width and height) of a Scilab console
	 * @param console the console which we want to get the dimensions of
	 * @return the size of the console
	 * @see org.scilab.modules.ihm.UIElement#getDims()
	 */
	public static Size getDims(Console console) {
		return ScilabConsoleBridge.getDims(console);
	}

	/**
	 * Gets the position (X-coordinate and Y-coordinates) of a Scilab console
	 * @param console the console which we want to get the position of
	 * @return the position of the console
	 * @see org.scilab.modules.ihm.UIElement#getPosition()
	 */
	public static Position getPosition(Console console) {
		return ScilabConsoleBridge.getPosition(console);
	}

	/**
	 * Gets the visibility status of a console
	 * @param console the console which we want to get the visibility status of
	 * @return the visibility status of the console (true if the console is visible, false if not)
	 * @see org.scilab.modules.ihm.UIElement#isVisible()
	 */
	public static boolean isVisible(Console console) {
		return ScilabConsoleBridge.isVisible(console);
	}

	/**
	 * Sets the visibility status of a console
	 * @param console the console which we want to get the visibility status of
 	 * @param newVisibleState the visibility status we want to set to the console (true to set the console visible, false else)
	 * @see org.scilab.modules.ihm.UIElement#isVisible()
	 */
	public static void setVisible(Console console, boolean newVisibleState) {
		ScilabConsoleBridge.setVisible(console, newVisibleState);
	}
	
	/**
	 * Sets the dimensions (width and height) of a Scilab console
	 * @param console the console which we want to set the dimensions of
	 * @param newSize the size we want to set to the console
	 * @see org.scilab.modules.ihm.UIElement#setDims(org.scilab.modules.ihm.utils.Size)
	 */
	public static void setDims(Console console, Size newSize) {
		ScilabConsoleBridge.setDims(console, newSize);
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a Scilab console
	 * @param console the console which we want to set the position of
	 * @param newPosition the position we want to set to the console
	 * @see org.scilab.modules.ihm.UIElement#setPosition(org.scilab.modules.ihm.utils.Position)
	 */
	public static void setPosition(Console console, Position newPosition) {
		ScilabConsoleBridge.setPosition(console, newPosition);
	}

	/**
	 * Sets the visibility status of a Scilab console
	 * @param console the console which we want to set the visibility status of
	 * @param newVisibleState the visibility status we want to set to the console
	 * @see org.scilab.modules.ihm.UIElement#setVisible(boolean)
	 */
	public static void setPosition(Console console, boolean newVisibleState) {
		ScilabConsoleBridge.setVisible(console, newVisibleState);
	}

	/**
	 * Clears the console (no more scroll history and prompt on top left corner)
	 * @param console the console to be cleared
	 */
	public static void clear(Console console) {
		ScilabConsoleBridge.clear(console);
	}
	
	/**
   * Clears lines from the end of the output view
   * @param console the console to be cleared
   * @param nbLines the number of lines to be deleted
   */
  public static void clear(Console console, int nbLines) {
		ScilabConsoleBridge.clear(console, nbLines);
	}
	
	/**
   * Puts the prompt in the top left corner of the console
   * @param console the console used
   */
  public static void toHome(Console console) {
  	ScilabConsoleBridge.toHome(console);
  }
	
	/**
	 * Reads one user input char
	 * @param console the console used to get the char
	 * @return the data entered by the user
	 * @see fr.scilab.console.HelpBrowser#getCharWithoutOutput()
	 */
	public static int getCharWithoutOutput(Console console) {
		return ScilabConsoleBridge.getCharWithoutOutput(console);
	}

	/**
	   * Sets the prompt displayed in the console
	   * @param console the console where the prompt is set
	   * @param prompt the prompt to be displayed in the console
	   */
	public static void setPrompt(Console console, String prompt) {
		ScilabConsoleBridge.setPrompt(console, prompt);
	}
	
	/**
	 * Updates Scilab internal variables containing the size of the console
	 * These variables are used to format data before displaying it
	 * @param console object we want to get the size of
	 */
	public static void scilabLinesUpdate(Console console) {
		ScilabConsoleBridge.scilabLinesUpdate(console);
	}

	/**
	 * Get the current status of the console
	 * If the prompt view is visible, Scilab is waiting for commands
	 * @param console object we want to get the status of
	 * @return true is Scilab is waiting for commands
	 */
	public static boolean isWaitingForInput(Console console) {
		return ScilabConsoleBridge.isWaitingForInput(console);
	}

	/**
	 * Clear the commands history
	 * @param console the console we want to clear the history of 
	 */
	public static void clearHistory(Console console) {
		ScilabConsoleBridge.clearHistory(console);
	}
	
	/**
	 * Select all the console contents
	 * @param console the console 
	 */
	public static void selectAll(Console console) {
		ScilabConsoleBridge.selectAll(console);
	}
	
	/**
	 * Put the console selected text in the clipboard
	 * @param console the console 
	 */
	public static void copyToClipboard(Console console) {
		ScilabConsoleBridge.copyToClipboard(console);
	}
	
	/**
	 * Paste clipboard contents in Console input line
	 * @param console the console 
	 */
	public static void pasteClipboard(Console console) {
		ScilabConsoleBridge.pasteClipboard(console);
	}

	/**
	 * Cut selected text in the Console input line
	 * @param console the console 
	 */
	public static void cutSelection(Console console) {
		ScilabConsoleBridge.cutSelection(console);
	}
	
	/**
	 * Set the font of the Console
	 * @param console the console
	 * @param font the font to set
	 */
	public static void setFont(Console console, Font font) {
		ScilabConsoleBridge.setFont(console, font);
	}

	/**
	 * Get the font of the Console
	 * @param console the console
	 * @return the font
	 */
	public static Font getFont(Console console) {
		return ScilabConsoleBridge.getFont(console);
	}

	/**
	 * Get the Foreground Color of the Console
	 * @param console the console
	 * @return the Foreground Color
	 */
	public static Color getForeground(Console console) {
		return ScilabConsoleBridge.getForeground(console);
	}
	
	/**
	 * Get the Background Color of the Console
	 * @param console the console
	 * @return the Background Color
	 */
	public static Color getBackground(Console console) {
		return ScilabConsoleBridge.getBackground(console);
	}

	/**
	 * Set the Foreground Color of the Console
	 * @param console the console
	 * @param color the Foreground Color
	 */
	public static void setForeground(Console console, Color color) {
		ScilabConsoleBridge.setForeground(console, color);
	}
	
	/**
	 * Set the Background Color of the Console
	 * @param console the console
	 * @param color the Background Color
	 */
	public static void setBackground(Console console, Color color) {
		ScilabConsoleBridge.setBackground(console, color);
	}

	/**
	 * Set the maximum number of lines stored in the Output
	 * @param console the console
	 * @param nbLines the number of lines 
	 */
	public static void setMaxOutputSize(Console console, int nbLines) {
		ScilabConsoleBridge.setMaxOutputSize(console, nbLines);
	}
	
	/**
	 * Set the cursor pointer over the Console
	 * @param console the console
	 * @param cursor the Cursor to set
	 */
	public static void setCursor(Console console, Cursor cursor) {
		ScilabConsoleBridge.setCursor(console, cursor);
	}

	/*****************/
	/* Canvas Bridge */
	/*****************/

	/**
	 * Creates a Scilab Canvas
	 * @param figureIndex index of the displayed figure
	 * @param antialiasingQuality Specify the number of pass to use for antialiasing.
     *                            If its value is 0, then antialiasing is disable.
	 * @return the created canvas
	 */
	public static SimpleCanvas createCanvas(int figureIndex, int antialiasingQuality) {
		return ScilabCanvasBridge.createCanvas(figureIndex, antialiasingQuality);
	}

	/**
	 * Draws a Scilab canvas
	 * @param canvas the canvas to draw
	 * @see org.scilab.modules.ihm.UIElement#draw()
	 */
	public static void draw(Canvas canvas) {
		ScilabCanvasBridge.draw(canvas);
	}

	/**
	 * Gets the dimensions (width and height) of a Scilab Canvas
	 * @param canvas the canvas we want to get the dimensions of
	 * @return the size of the canvas
	 * @see org.scilab.modules.ihm.UIElement#getDims()
	 */
	public static Size getDims(Canvas canvas) {
		return ScilabCanvasBridge.getDims(canvas);
	}

	/**
	 * Gets the position (X-coordinate and Y-coordinate) of a Scilab canvas
	 * @param canvas the canvas we want to get the position of
	 * @return the position of the canvas
	 * @see org.scilab.modules.ihm.UIElement#getPosition()
	 */
	public static Position getPosition(Canvas canvas) {
		return ScilabCanvasBridge.getPosition(canvas);
	}

	/**
	 * Gets the visibility status of a Scilab Canvas
	 * @param canvas the canvas we want to get the visiblity status of
	 * @return the visibility status of the canvas (true if the canvas is visible, false if not)
	 * @see org.scilab.modules.ihm.UIElement#isVisible()
	 */
	public static boolean isVisible(Canvas canvas) {
		return ScilabCanvasBridge.isVisible(canvas);
	}

	/**
	 * Sets the dimensions (width and height) of a Scilab Canvas
	 * @param canvas the canvas we want to set the dimensions of
	 * @param newSize the size we want to set to the canvas
	 * @see org.scilab.modules.ihm.UIElement#setDims(org.scilab.modules.ihm.utils.Size)
	 */
	public static void setDims(Canvas canvas, Size newSize) {
		ScilabCanvasBridge.setDims(canvas, newSize);
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a Scilab canvas
	 * @param canvas the canvas we want to set the position of
	 * @param newPosition the position we want to set to the canvas
	 * @see org.scilab.modules.ihm.UIElement#setPosition(org.scilab.modules.ihm.utils.Position)
	 */
	public static void setPosition(Canvas canvas, Position newPosition) {
		ScilabCanvasBridge.setPosition(canvas, newPosition);
	}

	/**
	 * Sets the visibility status of a Scilab Canvas
	 * @param canvas the canvas we want to set the visiblity status of
	 * @param newVisibleState the visibility status we want to set to the canvas (true to set the canvas visible, false else)
	 * @see org.scilab.modules.ihm.UIElement#setVisible(boolean)
	 */
	public static void setVisible(Canvas canvas, boolean newVisibleState) {
		ScilabCanvasBridge.setVisible(canvas, newVisibleState);
	}
	
	/**
	 * @param canvas the canvas we want to display
	 */
	public static void display(Canvas canvas) {
		ScilabCanvasBridge.display(canvas);
	}
	
	/**
	 * @param canvas the canvas we want to get the GL object from
	 * @return a GL object
	 */
	public static GL getGL(Canvas canvas) {
		return ScilabCanvasBridge.getGL(canvas);
	}
	
	/**
	 * @param canvas the canvas of which we want to set the swap buffer mode.
	 * @param onOrOff set wether the swap buffer mode is on or off.
	 */
	public static void setAutoSwapBufferMode(Canvas canvas, boolean onOrOff) {
		ScilabCanvasBridge.setAutoSwapBufferMode(canvas, onOrOff);
	}
	
	/**
	 * @param canvas the canvas from which we want to retrieve the swap buffer mode.
	 * @return set wether the swap buffer mode is on or off.
	 */
	public static boolean getAutoSwapBufferMode(Canvas canvas) {
		return ScilabCanvasBridge.getAutoSwapBufferMode(canvas);
	}
	
	/**
	 * @param canvas the canvas we want to repaint
	 */
	public static void repaint(Canvas canvas) {
		ScilabCanvasBridge.repaint(canvas);
	}
	
	
	/**
	 * Set the background of the Canvas.
	 * @param canvas the canvas we want to modify
	 * @param red red channel
	 * @param green green channel 
	 * @param blue blue channel
	 */
	public static void setBackgroundColor(Canvas canvas, double red, double green, double blue) {
		ScilabCanvasBridge.setBackgroundColor(canvas, red, green, blue);
	}
	
	/**
	 * Create an interactive selection rectangle and return its pixel coordinates
	 * @param canvas canvas on which the rubber box will be applied
	 * @param isClick specify wether the rubber box is selected by one click for each one of the two edge
	 *                or a sequence of press-release
	 * @param isZoom specify if the rubber box is used for a zoom and then change the mouse cursor.
	 * @param initialRect if not null specify the initial rectangle to draw
	 * @param endRect array [x1,y1,x2,y2] containing the result of rubberbox
	 * @return Scilab code of the pressed button
	 */
	public static int rubberBox(Canvas canvas, boolean isClick, boolean isZoom, int[] initialRect, int[] endRect) {
		return ScilabCanvasBridge.rubberBox(canvas, isClick, isZoom, initialRect, endRect);
	}
	
	/**
	 * Get the displacement in pixel that should be used for rotating axes
	 * @param tab tab on which the displacement is recorded
	 * @param displacement out parameter, [x,y] array of displacement in pixels
	 * @return true if the displacement recording continue, false otherwise
	 */
	public static boolean getRotationDisplacement(Tab tab, int[] displacement) {
		return ScilabTabBridge.getRotationDisplacement(tab, displacement);
	}
	
	/**
	 * Asynchronous stop of rotation tracking.
	 * @param tab tab on which the displacement is recorded
	 */
	public static void stopRotationRecording(Tab tab) {
		ScilabTabBridge.stopRotationRecording(tab);
	}
	
	/**
	  * Disable the canvas before closing
	  * @param canvas canvas to close
	  */
	public static void close(Canvas canvas) {
		ScilabCanvasBridge.close(canvas);
	}
	
	/**
	 * Dump the scilabCanvas to an BufferedImage
	 * @param scilabCanvas ScilabCanvas
	 * @return a BufferedImage
	 */
	public static BufferedImage dumpAsBufferedImage(ScilabCanvas scilabCanvas) {
		return ScilabCanvasBridge.dumpAsBufferedImage(scilabCanvas);
	}
	
	/**
	 * Set double buffer mode on or Off
	 * @param useSingleBuffer if true use single buffer if false use double buffering
	 * @param canvas canvas to modify
	 */
	public static void setSingleBuffered(Canvas canvas, boolean useSingleBuffer) {
		ScilabCanvasBridge.setSingleBuffered(canvas, useSingleBuffer);
	}
	
	/*****************/
	/* MenuBar Bridge */
	/*****************/

	/**
	 * Creates a new MenuBar
	 * @return the created MenuBar
	 */
	public static SimpleMenuBar createMenuBar() {
		return ScilabMenuBarBridge.createMenuBar();
	}
	
	/**
	 * Append a Menu to a MenuBar
	 * @param menuBar the MenuBar which we want to add the newMenu to
	 * @param newMenu the Menu to add to the MenuBar
	 */
	public static void add(MenuBar menuBar, Menu newMenu) {
		ScilabMenuBarBridge.add(menuBar, newMenu);
	}
	
	/*****************/
	/* MenuItem Bridge */
	/*****************/

	/**
	 * Creates a new MenuItem
	 * @return the created MenuItem
	 */
	public static SimpleMenuItem createMenuItem() {
		return ScilabMenuItemBridge.createMenuItem();
	}
	
	/**
	 * Sets the Text of a MenuItem
	 * @param menuItem the MenuItem we want to set the Text of
	 * @param newText the Text to set for the MenuItem
	 */
	public static void setText(MenuItem menuItem, String newText) {
		ScilabMenuItemBridge.setText(menuItem, newText);
	}
	
	/**
	 * set a mnemonic to a MenuItem
	 * @param menuItem the MenuItem which we want to add the mnemonic to
	 * @param mnemonic the mnemonic to add to the MenuItem
	 */
	public static void setMnemonic(MenuItem menuItem, char mnemonic) {
		ScilabMenuItemBridge.setMnemonic(menuItem, mnemonic);
	}
	
	/**
	 * Add a callback to the menu, this callback is a Scilab command
	 * @param menuItem the MenuItem which we want to add the mnemonic to
	 * @param callback the CallBack to set.
	 */
	public static void setCallback(MenuItem menuItem, CallBack callback) {
		ScilabMenuItemBridge.setCallback(menuItem, callback);
	}

	/**
	 * Set if the menu item is enabled or not
	 * @param menuItem the MenuItem which we want to set the status of
	 * @param status true if the menu item is enabled
	 */
	public static void setEnabled(MenuItem menuItem, boolean status) {
		ScilabMenuItemBridge.setEnabled(menuItem, status);
	}
	
	/**
	 * Get the enable status of a MenuItem
	 * @param menuItem the MenuItem which we want to get the status of
	 * @return the enable status of the MenuItem (true if the MenuItem is enabled, false if not)
	 */
	public static boolean isEnabled(MenuItem menuItem) {
		return ScilabMenuItemBridge.isEnabled(menuItem);
	}
	
	/**
	 * Get the text to a MenuItem
	 * @param menuItem the MenuItem which we want to get the text of
	 * @return the text of the MenuItem
	 */
	public static String getText(MenuItem menuItem) {
		return ScilabMenuItemBridge.getText(menuItem);
	}
	
	/**
	 * set a mnemonic to a MenuItem
	 * @param menuItem the MenuItem which we want to set the mnemonic to
	 * @param mnemonic the new mnemonic to set to the MenuItem
	 */
	public static void setMnemonic(MenuItem menuItem, int mnemonic) {
		ScilabMenuItemBridge.setMnemonic(menuItem, mnemonic);
	}

	/**
	 * Sets the visibility status of a Scilab MenuItem
	 * @param menuItem the MenuItem we want to set the visiblity status of
	 * @param newVisibleState the visibility status we want to set to the MenuItem (true to set the MenuItem visible, false else)
	 * @see org.scilab.modules.ihm.UIElement#setVisible(boolean)
	 */
	public static void setVisible(MenuItem menuItem, boolean newVisibleState) {
		ScilabMenuItemBridge.setVisible(menuItem, newVisibleState);
	}

	/**
	 * Gets the visibility status of a Scilab MenuItem
	 * @param menuItem the MenuItem we want to get the visiblity status of
	 * @return the visibility status of the MenuItem (true if the MenuItem is visible, false if not)
	 * @see org.scilab.modules.ihm.UIElement#isVisible()
	 */
	public static boolean isVisible(MenuItem menuItem) {
		return ScilabMenuItemBridge.isVisible(menuItem);
	}

	/**
	 * Set the Background color of the MenuItem
	 * @param menuItem the MenuItem we want to set the background of
	 * @param color the Color
	 */
	public static void setBackground(MenuItem menuItem, Color color) {
		ScilabMenuItemBridge.setBackground(menuItem, color);
	}
	
	/**
	 * Get the Background color of the MenuItem
	 * @param menuItem the MenuItem we want to get the background of
	 * @return the Color
	 */
	public static Color getBackground(MenuItem menuItem) {
		return ScilabMenuItemBridge.getBackground(menuItem);
	}

	/**
	 * Set the Foreground color of the MenuItem
	 * @param menuItem the MenuItem we want to set the foreground of
	 * @param color the Color
	 */
	public static void setForeground(MenuItem menuItem, Color color) {
		ScilabMenuItemBridge.setForeground(menuItem, color);
	}
	
	/**
	 * Get the Foreground color of the MenuItem
	 * @param menuItem the MenuItem we want to get the foreground of
	 * @return the Color
	 */
	public static Color getForeground(MenuItem menuItem) {
		return ScilabMenuItemBridge.getForeground(menuItem);
	}

	/**
	 * Set the font of the MenuItem.
	 * @param menuItem the MenuItem we want to set the font of
	 * @param font the font
	 */
	public static void setFont(MenuItem menuItem, Font font) {
		ScilabMenuItemBridge.setFont(menuItem, font);
	}
	
	/**
	 * Get the font of the MenuItem.
	 * @param menuItem the MenuItem we want to get the font of
	 * @return the font
	 */
	public static Font getFont(MenuItem menuItem) {
		return ScilabMenuItemBridge.getFont(menuItem);
	}

	/**
	 * Set the horizontal alignment for the MenuItem text
	 * @param menuItem the MenuItem we want to set the alignment of
	 * @param alignment the value for the alignment (See ScilabAlignment.java)
	 */
	public static void setHorizontalAlignment(MenuItem menuItem, String alignment) {
		ScilabMenuItemBridge.setHorizontalAlignment(menuItem, alignment);
	}

	/**
	 * Set the vertical alignment for the MenuItem text
	 * @param menuItem the MenuItem we want to set the alignment of
	 * @param alignment the value for the alignment (See ScilabAlignment.java)
	 */
	public static void setVerticalAlignment(MenuItem menuItem, String alignment) {
		ScilabMenuItemBridge.setVerticalAlignment(menuItem, alignment);
	}

	/**
	 * Set the Relief of the MenuItem
	 * @param menuItem the MenuItem which we want to set the Relief of
	 * @param reliefType the type of the relief to set (See ScilabRelief.java)
	 */
	public static void setRelief(MenuItem menuItem, String reliefType) {
		ScilabMenuItemBridge.setRelief(menuItem, reliefType);
	}

	/**
	 * Add a MenuItem to the MenuItem
	 * @param parentMenuItem the MenuItem where we want to add a MenuItem
	 * @param childMenuItem the MenuItem we want to add
	 */
	public static void add(MenuItem parentMenuItem, MenuItem childMenuItem) {
		ScilabMenuItemBridge.add(parentMenuItem, childMenuItem);
	}

	/**
	 * Add a Menu to the MenuItem
	 * @param parentMenuItem the MenuItem where we want to add a MenuItem
	 * @param childMenu the Menu we want to add
	 */
	public static void add(MenuItem parentMenuItem, Menu childMenu) {
		ScilabMenuItemBridge.add(parentMenuItem, childMenu);
	}

	/**
	 * Destroy the MenuItem
	 * @param menuItem the MenuItem to be destroyed
	 */
	public static void destroy(MenuItem menuItem) {
		ScilabMenuItemBridge.destroy(menuItem);
	}

	/**
	 * Give the focus to the MenuItem
	 * @param menuItem the MenuItem
	 */
	public static void requestFocus(MenuItem menuItem) {
		ScilabMenuItemBridge.requestFocus(menuItem);
	}

	/**
	 * Retrieve the CallBack associated to this MenuItem
	 * @param menuItem the MenuItem
	 * @return the CallBack
	 */
	public static CallBack getCallback(MenuItem menuItem) {
		return ScilabMenuItemBridge.getCallback(menuItem);
	}
	
	/**
	 * Set if the MenuItem is checked or not
	 * @param menuItem the MenuItem
	 * @param status true if the MenuItem is checked
	 */
	public static void setChecked(MenuItem menuItem, boolean status) {
		ScilabMenuItemBridge.setChecked(menuItem, status);
	}
	
	/**
	 * Get if the MenuItem is checked or not
	 * @param menuItem the MenuItem
	 * @return true if the MenuItem is checked
	 */
	public static boolean isChecked(MenuItem menuItem) {
		return ScilabMenuItemBridge.isChecked(menuItem);
	}

	/****************************/
	/* CheckBox MenuItem Bridge */
	/****************************/

	/**
	 * Creates a new CheckBoxMenuItem
	 * @return the created CheckBoxMenuItem
	 */
	public static SimpleCheckBoxMenuItem createCheckBoxMenuItem() {
		return ScilabCheckBoxMenuItemBridge.createCheckBoxMenuItem();
	}
	
	/**
	 * Add a MenuItem to the CheckBoxMenuItem
	 * @param parentMenuItem the MenuItem where we want to add a CheckBoxMenuItem
	 * @param childMenuItem the MenuItem we want to add
	 */
	public static void add(CheckBoxMenuItem parentMenuItem, MenuItem childMenuItem) {
		ScilabMenuItemBridge.add(parentMenuItem, childMenuItem);
	}
	
	/**
	 * Sets the Text of a CheckBoxMenuItem
	 * @param checkBoxMenuItem the CheckBoxMenuItem we want to set the Text of
	 * @param newText the Text to set for the CheckBoxMenuItem
	 */
	public static void setText(CheckBoxMenuItem checkBoxMenuItem, String newText) {
		ScilabCheckBoxMenuItemBridge.setText(checkBoxMenuItem, newText);
	}
	
	/**
	 * set a mnemonic to a CheckBoxMenuItem
	 * @param checkBoxMenuItem the CheckBoxMenuItem which we want to add the mnemonic to
	 * @param mnemonic the mnemonic to add to the CheckBoxMenuItem
	 */
	public static void setMnemonic(CheckBoxMenuItem checkBoxMenuItem, char mnemonic) {
		ScilabCheckBoxMenuItemBridge.setMnemonic(checkBoxMenuItem, mnemonic);
	}
	
	/**
	 * Add a callback to the menu, this callback is a Scilab command
	 * @param checkBoxMenuItem the CheckBoxMenuItem which we want to add the mnemonic to
	 * @param callback the CallBack to set.
	 */
	public static void setCallback(CheckBoxMenuItem checkBoxMenuItem, CallBack callback) {
		ScilabCheckBoxMenuItemBridge.setCallback(checkBoxMenuItem, callback);
	}

	/**
	 * Set if the menu item is enabled or not
	 * @param checkBoxMenuItem the CheckBoxMenuItem which we want to set the status of
	 * @param status true if the menu item is enabled
	 */
	public static void setEnabled(CheckBoxMenuItem checkBoxMenuItem, boolean status) {
		ScilabCheckBoxMenuItemBridge.setEnabled(checkBoxMenuItem, status);
	}
	
	/**
	 * Get the enable status of a CheckBoxMenuItem
	 * @param checkBoxMenuItem the CheckBoxMenuItem which we want to get the status of
	 * @return the enable status of the CheckBoxMenuItem (true if the CheckBoxMenuItem is enabled, false if not)
	 */
	public static boolean isEnabled(CheckBoxMenuItem checkBoxMenuItem) {
		return ScilabCheckBoxMenuItemBridge.isEnabled(checkBoxMenuItem);
	}
	
	/**
	 * Get the text to a CheckBoxMenuItem
	 * @param checkBoxMenuItem the CheckBoxMenuItem which we want to get the text of
	 * @return the text of the CheckBoxMenuItem
	 */
	public static String getText(CheckBoxMenuItem checkBoxMenuItem) {
		return ScilabCheckBoxMenuItemBridge.getText(checkBoxMenuItem);
	}
	
	/**
	 * set a mnemonic to a CheckBoxMenuItem
	 * @param checkBoxMenuItem the CheckBoxMenuItem which we want to set the mnemonic to
	 * @param mnemonic the new mnemonic to set to the CheckBoxMenuItem
	 */
	public static void setMnemonic(CheckBoxMenuItem checkBoxMenuItem, int mnemonic) {
		ScilabCheckBoxMenuItemBridge.setMnemonic(checkBoxMenuItem, mnemonic);
	}

	/**
	 * Sets the visibility status of a Scilab CheckBoxMenuItem
	 * @param checkBoxMenuItem the CheckBoxMenuItem we want to set the visiblity status of
	 * @param newVisibleState the visibility status we want to set to the CheckBoxMenuItem (true to set the CheckBoxMenuItem visible, false else)
	 * @see org.scilab.modules.ihm.UIElement#setVisible(boolean)
	 */
	public static void setVisible(CheckBoxMenuItem checkBoxMenuItem, boolean newVisibleState) {
		ScilabCheckBoxMenuItemBridge.setVisible(checkBoxMenuItem, newVisibleState);
	}

	/**
	 * Gets the visibility status of a Scilab CheckBoxMenuItem
	 * @param checkBoxMenuItem the CheckBoxMenuItem we want to get the visiblity status of
	 * @return the visibility status of the CheckBoxMenuItem (true if the CheckBoxMenuItem is visible, false if not)
	 * @see org.scilab.modules.ihm.UIElement#isVisible()
	 */
	public static boolean isVisible(CheckBoxMenuItem checkBoxMenuItem) {
		return ScilabCheckBoxMenuItemBridge.isVisible(checkBoxMenuItem);
	}

	/**
	 * Set the Background color of the CheckBoxMenuItem
	 * @param checkBoxMenuItem the CheckBoxMenuItem we want to set the background of
	 * @param color the Color
	 */
	public static void setBackground(CheckBoxMenuItem checkBoxMenuItem, Color color) {
		ScilabCheckBoxMenuItemBridge.setBackground(checkBoxMenuItem, color);
	}
	
	/**
	 * Get the Background color of the CheckBoxMenuItem
	 * @param checkBoxMenuItem the CheckBoxMenuItem we want to get the background of
	 * @return the Color
	 */
	public static Color getBackground(CheckBoxMenuItem checkBoxMenuItem) {
		return ScilabCheckBoxMenuItemBridge.getBackground(checkBoxMenuItem);
	}

	/**
	 * Set the Foreground color of the CheckBoxMenuItem
	 * @param checkBoxMenuItem the CheckBoxMenuItem we want to set the foreground of
	 * @param color the Color
	 */
	public static void setForeground(CheckBoxMenuItem checkBoxMenuItem, Color color) {
		ScilabCheckBoxMenuItemBridge.setForeground(checkBoxMenuItem, color);
	}
	
	/**
	 * Get the Foreground color of the CheckBoxMenuItem
	 * @param checkBoxMenuItem the CheckBoxMenuItem we want to get the foreground of
	 * @return the Color
	 */
	public static Color getForeground(CheckBoxMenuItem checkBoxMenuItem) {
		return ScilabCheckBoxMenuItemBridge.getForeground(checkBoxMenuItem);
	}

	/**
	 * Set the font of the CheckBoxMenuItem.
	 * @param checkBoxMenuItem the CheckBoxMenuItem we want to set the font of
	 * @param font the font
	 */
	public static void setFont(CheckBoxMenuItem checkBoxMenuItem, Font font) {
		ScilabCheckBoxMenuItemBridge.setFont(checkBoxMenuItem, font);
	}
	
	/**
	 * Get the font of the CheckBoxMenuItem.
	 * @param checkBoxMenuItem the CheckBoxMenuItem we want to get the font of
	 * @return the font
	 */
	public static Font getFont(CheckBoxMenuItem checkBoxMenuItem) {
		return ScilabCheckBoxMenuItemBridge.getFont(checkBoxMenuItem);
	}

	/**
	 * Set the horizontal alignment for the CheckBoxMenuItem text
	 * @param checkBoxMenuItem the CheckBoxMenuItem we want to set the alignment of
	 * @param alignment the value for the alignment (See ScilabAlignment.java)
	 */
	public static void setHorizontalAlignment(CheckBoxMenuItem checkBoxMenuItem, String alignment) {
		ScilabCheckBoxMenuItemBridge.setHorizontalAlignment(checkBoxMenuItem, alignment);
	}

	/**
	 * Set the vertical alignment for the CheckBoxMenuItem text
	 * @param checkBoxMenuItem the CheckBoxMenuItem we want to set the alignment of
	 * @param alignment the value for the alignment (See ScilabAlignment.java)
	 */
	public static void setVerticalAlignment(CheckBoxMenuItem checkBoxMenuItem, String alignment) {
		ScilabCheckBoxMenuItemBridge.setVerticalAlignment(checkBoxMenuItem, alignment);
	}

	/**
	 * Set the Relief of the CheckBoxMenuItem
	 * @param checkBoxMenuItem the CheckBoxMenuItem which we want to set the Relief of
	 * @param reliefType the type of the relief to set (See ScilabRelief.java)
	 */
	public static void setRelief(CheckBoxMenuItem checkBoxMenuItem, String reliefType) {
		ScilabCheckBoxMenuItemBridge.setRelief(checkBoxMenuItem, reliefType);
	}

	/**
	 * Add a CheckBoxMenuItem to the CheckBoxMenuItem
	 * @param parentCheckBoxMenuItem the CheckBoxMenuItem where we want to add a CheckBoxMenuItem
	 * @param childCheckBoxMenuItem the CheckBoxMenuItem we want to add
	 */
	public static void add(CheckBoxMenuItem parentCheckBoxMenuItem, CheckBoxMenuItem childCheckBoxMenuItem) {
		ScilabCheckBoxMenuItemBridge.add(parentCheckBoxMenuItem, childCheckBoxMenuItem);
	}

	/**
	 * Add a Menu to the CheckBoxMenuItem
	 * @param parentCheckBoxMenuItem the CheckBoxMenuItem where we want to add a CheckBoxMenuItem
	 * @param childMenu the Menu we want to add
	 */
	public static void add(CheckBoxMenuItem parentCheckBoxMenuItem, Menu childMenu) {
		ScilabCheckBoxMenuItemBridge.add(parentCheckBoxMenuItem, childMenu);
	}

	/**
	 * Destroy the CheckBoxMenuItem
	 * @param checkBoxMenuItem the CheckBoxMenuItem to be destroyed
	 */
	public static void destroy(CheckBoxMenuItem checkBoxMenuItem) {
		ScilabCheckBoxMenuItemBridge.destroy(checkBoxMenuItem);
	}

	/**
	 * Give the focus to the CheckBoxMenuItem
	 * @param checkBoxMenuItem the CheckBoxMenuItem
	 */
	public static void requestFocus(CheckBoxMenuItem checkBoxMenuItem) {
		ScilabCheckBoxMenuItemBridge.requestFocus(checkBoxMenuItem);
	}

	/**
	 * Retrieve the CallBack associated to this CheckBoxMenuItem
	 * @param checkBoxMenuItem the CheckBoxMenuItem
	 * @return the CallBack
	 */
	public static CallBack getCallback(CheckBoxMenuItem checkBoxMenuItem) {
		return ScilabCheckBoxMenuItemBridge.getCallback(checkBoxMenuItem);
	}

	/**
	 * Set if the CheckBoxMenuItem is checked or not
	 * @param checkBoxMenuItem the CheckBoxMenuItem
	 * @param status true if the CheckBoxMenuItem is checked
	 */
	public static void setChecked(CheckBoxMenuItem checkBoxMenuItem, boolean status) {
		ScilabCheckBoxMenuItemBridge.setChecked(checkBoxMenuItem, status);
	}
	
	/**
	 * Get if the CheckBoxMenuItem is checked or not
	 * @param checkBoxMenuItem the CheckBoxMenuItem
	 * @return true if the CheckBoxMenuItem is checked
	 */
	public static boolean isChecked(CheckBoxMenuItem checkBoxMenuItem) {
		return ScilabCheckBoxMenuItemBridge.isChecked(checkBoxMenuItem);
	}
	
	/*****************/
	/* Menu Bridge */
	/*****************/

	/**
	 * Creates a new Menu
	 * @return the created Menu
	 */
	public static SimpleMenu createMenu() {
		return ScilabMenuBridge.createMenu();
	}
	
	/**
	 * Retrieve the CallBack associated to this Menu
	 * @param menuItem the Menu
	 * @return the CallBack
	 */
	public static CallBack getCallback(Menu menu) {
		return ScilabMenuBridge.getCallback(menu);
	}
	
	/**
	 * Append a MenuItem to a Menu
	 * @param menu the Menu which we want to add the MenuItem to
	 * @param newMenuItem the MenuItem to add to the Menu
	 */
	public static void add(Menu menu, MenuItem newMenuItem) {
		ScilabMenuBridge.add(menu, newMenuItem);
	}
	
	/**
	 * Append a CheckBoxMenuItem to a Menu
	 * @param menu the Menu which we want to add the CheckBoxMenuItem to
	 * @param newCheckBoxMenuItem the MenuItem to add to the Menu
	 */
	public static void add(Menu menu, CheckBoxMenuItem newCheckBoxMenuItem) {
		ScilabMenuBridge.add(menu, newCheckBoxMenuItem);
	}
	
	/**
	 * Append a Menu to a Menu
	 * @param menu the Menu which we want to append the Menu to
	 * @param newSubMenu the Menu to append to the Menu
	 */
	public static void add(Menu menu, Menu newSubMenu) {
		ScilabMenuBridge.add(menu, newSubMenu);
	}
	
	/**
	 * set a text to a menu
	 * @param menu the Menu which we want to set the text to
	 * @param newText the new text to set to the menu
	 */
	public static void setText(Menu menu, String newText) {
		ScilabMenuBridge.setText(menu, newText);
	}
	
	/**
	 * Get the text to a menu
	 * @param menu the Menu which we want to get the text of
	 * @return the text of the menu
	 */
	public static String getText(Menu menu) {
		return ScilabMenuBridge.getText(menu);
	}
	
	/**
	 * set a mnemonic to a menu
	 * @param menu the Menu which we want to set the mnemonic to
	 * @param mnemonic the new mnemonic to set to the menu
	 */
	public static void setMnemonic(Menu menu, int mnemonic) {
		ScilabMenuBridge.setMnemonic(menu, mnemonic);
	}
	
	/**
	 * Add a Separator to a Menu
	 * @param menu the Menu which we want to add the Separator to
	 */
	public static void addSeparator(Menu menu) {
		ScilabMenuBridge.addSeparator(menu);
	}

	/**
	 * Set if the menu is enabled or not
	 * @param menu the menu which we want to set the status of
	 * @param status true if the menu item is enabled
	 */
	public static void setEnabled(Menu menu, boolean status) {
		ScilabMenuBridge.setEnabled(menu, status);
	}

	/**
	 * Gets the enable status of a Menu
	 * @param menu the menu which we want to get the status of 
	 * @return the enable status of the Menu (true if the Menu is enabled, false if not)
	 */
	public static boolean isEnabled(Menu menu) {
		return ScilabMenuBridge.isEnabled(menu);
	}
	
	/**
	 * Sets the visibility status of a Scilab menu
	 * @param menu the menu we want to set the visiblity status of
	 * @param newVisibleState the visibility status we want to set to the menu (true to set the menu visible, false else)
	 * @see org.scilab.modules.ihm.UIElement#setVisible(boolean)
	 */
	public static void setVisible(Menu menu, boolean newVisibleState) {
		ScilabMenuBridge.setVisible(menu, newVisibleState);
	}

	/**
	 * Gets the visibility status of a Scilab Menu
	 * @param menu the menu we want to get the visiblity status of
	 * @return the visibility status of the menu (true if the menu is visible, false if not)
	 * @see org.scilab.modules.ihm.UIElement#isVisible()
	 */
	public static boolean isVisible(Menu menu) {
		return ScilabMenuBridge.isVisible(menu);
	}

	/**
	 * Set the Background color of the menu
	 * @param menu the menu we want to set the background of
	 * @param color the Color
	 */
	public static void setBackground(Menu menu, Color color) {
		ScilabMenuBridge.setBackground(menu, color);
	}
	
	/**
	 * Get the Background color of the menu
	 * @param menu the menu we want to get the background of
	 * @return the Color
	 */
	public static Color getBackground(Menu menu) {
		return ScilabMenuBridge.getBackground(menu);
	}

	/**
	 * Set the Foreground color of the menu
	 * @param menu the menu we want to set the foreground of
	 * @param color the Color
	 */
	public static void setForeground(Menu menu, Color color) {
		ScilabMenuBridge.setForeground(menu, color);
	}
	
	/**
	 * Get the Foreground color of the menu
	 * @param menu the menu we want to get the foreground of
	 * @return the Color
	 */
	public static Color getForeground(Menu menu) {
		return ScilabMenuBridge.getForeground(menu);
	}

	/**
	 * Set the font of the menu.
	 * @param menu the menu we want to set the font of
	 * @param font the font
	 */
	public static void setFont(Menu menu, Font font) {
		ScilabMenuBridge.setFont(menu, font);
	}
	
	/**
	 * Get the font of the menu.
	 * @param menu the menu we want to get the font of
	 * @return the font
	 */
	public static Font getFont(Menu menu) {
		return ScilabMenuBridge.getFont(menu);
	}

	/**
	 * Add a callback to the menu, this callback is a Scilab command
	 * @param menu the menu we want to set the callback of
	 * @param callback the CallBack to set.
	 */
	public static void setCallback(Menu menu, CallBack callback) {
		ScilabMenuBridge.setCallback(menu, callback);
	}

	/**
	 * Set the horizontal alignment for the Menu text
	 * @param menu the Menu we want to set the alignment of
	 * @param alignment the value for the alignment (See ScilabAlignment.java)
	 */
	public static void setHorizontalAlignment(Menu menu, String alignment) {
		ScilabMenuBridge.setHorizontalAlignment(menu, alignment);
	}

	/**
	 * Set the vertical alignment for the Menu text
	 * @param menu the Menu we want to set the alignment of
	 * @param alignment the value for the alignment (See ScilabAlignment.java)
	 */
	public static void setVerticalAlignment(Menu menu, String alignment) {
		ScilabMenuBridge.setVerticalAlignment(menu, alignment);
	}

	/**
	 * Set the Relief of the Menu
	 * @param menu the Menu which we want to set the Relief of
	 * @param reliefType the type of the relief to set (See ScilabRelief.java)
	 */
	public static void setRelief(Menu menu, String reliefType) {
		ScilabMenuBridge.setRelief(menu, reliefType);
	}

	/**
	 * Delete the Menu
	 * @param menu the Menu to be destroyed
	 */
	public static void destroy(Menu menu) {
		ScilabMenuBridge.destroy(menu);
	}

	/**
	 * Give the focus to the Menu
	 * @param menu the Menu
	 */
	public static void requestFocus(Menu menu) {
		ScilabMenuBridge.requestFocus(menu);
	}

	/**
	 * Set if the Menu is checked or not
	 * @param menu the Menu
	 * @param status true if the Menu is checked
	 */
	public static void setChecked(Menu menu, boolean status) {
		ScilabMenuBridge.setChecked(menu, status);
	}
	
	/**
	 * Get if the Menu is checked or not
	 * @param menu the Menu
	 * @return true if the Menu is checked
	 */
	public static boolean isChecked(Menu menu) {
		return ScilabMenuBridge.isChecked(menu);
	}

	/**********************/
	/* ContextMenu Bridge */
	/**********************/

	/**
	 * Creates a new ContextMenu
	 * @return the created ContextMenu
	 */
	public static SimpleContextMenu createContextMenu() {
		return ScilabContextMenuBridge.createContextMenu();
	}
	
	/**
	 * Append a MenuItem to a ContextMenu
	 * @param contextMenu the ContextMenu which we want to add the MenuItem to
	 * @param newMenuItem the MenuItem to add to the ContextMenu
	 */
	public static void add(ContextMenu contextMenu, MenuItem newMenuItem) {
		ScilabContextMenuBridge.add(contextMenu, newMenuItem);
	}
	
	/**
	 * Append a Menu to a ContextMenu
	 * @param contextMenu the ContextMenu which we want to add the MenuItem to
	 * @param newMenu the Menu to add to the ContextMenu
	 */
	public static void add(ContextMenu contextMenu, Menu newMenu) {
		ScilabContextMenuBridge.add(contextMenu, newMenu);
	}
	
	/**
	 * set a text to a ContextMenu
	 * @param contextMenu the Menu which we want to set the text to
	 * @param newText the new text to set to the contextMenu
	 */
	public static void setText(ContextMenu contextMenu, String newText) {
		ScilabContextMenuBridge.setText(contextMenu, newText);
	}
	
	/**
	 * Get the text to a contextMenu
	 * @param contextMenu the ContextMenu which we want to get the text of
	 * @return the text of the contextMenu
	 */
	public static String getText(ContextMenu contextMenu) {
		return ScilabContextMenuBridge.getText(contextMenu);
	}

	/**
	 * Set if the contextMenu is enabled or not
	 * @param contextMenu the contextMenu which we want to set the status of
	 * @param status true if the contextMenu item is enabled
	 */
	public static void setEnabled(ContextMenu contextMenu, boolean status) {
		ScilabContextMenuBridge.setEnabled(contextMenu, status);
	}

	/**
	 * Gets the enable status of a ContextMenu
	 * @param contextMenu the contextMenu which we want to get the status of
	 * @return the enable status of the ContextMenu (true if the ContextMenu is enabled, false if not)
	 */
	public static boolean isEnabled(ContextMenu contextMenu) {
		return ScilabContextMenuBridge.isEnabled(contextMenu);
	}

	/**
	 * Sets the visibility status of a Scilab contextMenu
	 * @param contextMenu the contextMenu we want to set the visiblity status of
	 * @param newVisibleState the visibility status we want to set to the contextMenu (true to set the contextMenu visible, false else)
	 * @see org.scilab.modules.ihm.UIElement#setVisible(boolean)
	 */
	public static void setVisible(ContextMenu contextMenu, boolean newVisibleState) {
		ScilabContextMenuBridge.setVisible(contextMenu, newVisibleState);
	}

	/**
	 * Gets the visibility status of a Scilab ContextMenu
	 * @param contextMenu the contextMenu we want to get the visiblity status of
	 * @return the visibility status of the contextMenu (true if the contextMenu is visible, false if not)
	 * @see org.scilab.modules.ihm.UIElement#isVisible()
	 */
	public static boolean isVisible(ContextMenu contextMenu) {
		return ScilabContextMenuBridge.isVisible(contextMenu);
	}

	/**
	 * Set the Background color of the contextMenu
	 * @param contextMenu the contextMenu we want to set the background of
	 * @param color the Color
	 */
	public static void setBackground(ContextMenu contextMenu, Color color) {
		ScilabContextMenuBridge.setBackground(contextMenu, color);
	}
	
	/**
	 * Get the Background color of the contextMenu
	 * @param contextMenu the contextMenu we want to get the background of
	 * @return the Color
	 */
	public static Color getBackground(ContextMenu contextMenu) {
		return ScilabContextMenuBridge.getBackground(contextMenu);
	}

	/**
	 * Set the Foreground color of the contextMenu
	 * @param contextMenu the contextMenu we want to set the foreground of
	 * @param color the Color
	 */
	public static void setForeground(ContextMenu contextMenu, Color color) {
		ScilabContextMenuBridge.setForeground(contextMenu, color);
	}
	
	/**
	 * Get the Foreground color of the contextMenu
	 * @param contextMenu the contextMenu we want to get the foreground of
	 * @return the Color
	 */
	public static Color getForeground(ContextMenu contextMenu) {
		return ScilabContextMenuBridge.getForeground(contextMenu);
	}

	/**
	 * Set the font of the contextMenu.
	 * @param contextMenu the contextMenu we want to set the font of
	 * @param font the font
	 */
	public static void setFont(ContextMenu contextMenu, Font font) {
		ScilabContextMenuBridge.setFont(contextMenu, font);
	}
	
	/**
	 * Get the font of the contextMenu.
	 * @param contextMenu the contextMenu we want to get the font of
	 * @return the font
	 */
	public static Font getFont(ContextMenu contextMenu) {
		return ScilabContextMenuBridge.getFont(contextMenu);
	}

	/**
	 * Add a callback to the contextMenu, this callback is a Scilab command
	 * @param contextMenu the contextMenu we want to set the callback of
	 * @param callback the CallBack to set.
	 */
	public static void setCallback(ContextMenu contextMenu, CallBack callback) {
		ScilabContextMenuBridge.setCallback(contextMenu, callback);
	}

	/**
	 * Set the horizontal alignment for the ContextMenu text
	 * @param contextMenu the ContextMenu we want to set the alignment of
	 * @param alignment the value for the alignment (See ScilabAlignment.java)
	 */
	public static void setHorizontalAlignment(ContextMenu contextMenu, String alignment) {
		ScilabContextMenuBridge.setHorizontalAlignment(contextMenu, alignment);
	}

	/**
	 * Set the vertical alignment for the ContextMenu text
	 * @param contextMenu the ContextMenu we want to set the alignment of
	 * @param alignment the value for the alignment (See ScilabAlignment.java)
	 */
	public static void setVerticalAlignment(ContextMenu contextMenu, String alignment) {
		ScilabContextMenuBridge.setVerticalAlignment(contextMenu, alignment);
	}

	/**
	 * Set the Relief of the ContextMenu
	 * @param contextMenu the ContextMenu which we want to set the Relief of
	 * @param reliefType the type of the relief to set (See ScilabRelief.java)
	 */
	public static void setRelief(ContextMenu contextMenu, String reliefType) {
		ScilabContextMenuBridge.setRelief(contextMenu, reliefType);
	}

	/**
	 * Delete the ContextMenu
	 * @param contextMenu the ContextMenu to be destroyed
	 */
	public static void destroy(ContextMenu contextMenu) {
		ScilabContextMenuBridge.destroy(contextMenu);
	}

	/**
	 * Give the focus to the ContextMenu
	 * @param contextMenu the ContextMenu
	 */
	public static void requestFocus(ContextMenu contextMenu) {
		ScilabContextMenuBridge.requestFocus(contextMenu);
	}

	/********************/
	/* Separator Bridge */
	/********************/

	/**
	 * Creates a new Separator
	 * @return the created Separator
	 */
	public static Separator createSeparator() {
		return ScilabSeparatorBridge.createSeparator();
	}
	
	/*****************/
	/* PushButton Bridge */
	/*****************/

	/**
	 * Creates a new PushButton
	 * @return the created PushButton
	 */
	public static SimplePushButton createPushButton() {
		return ScilabPushButtonBridge.createPushButton();
	}
	
	/**
	 * Draws a pushButton on screen
	 * @param pushButton the pushButton to be drawn
	 */
	public static void draw(PushButton pushButton) {
		ScilabPushButtonBridge.draw(pushButton);
	}

	/**
	 * Gets the dimensions (width and height) of a pushButton
	 * @param pushButton the pushButton we want to get the dimensions of
	 * @return dimensions of the pushButton
	 */
	public static Size getDims(PushButton pushButton) {
		return ScilabPushButtonBridge.getDims(pushButton);
	}

	/**
	 * Sets the dimensions (width and height) of a pushButton
	 * @param pushButton the pushButton we want to set the dimensions of
	 * @param newSize new dimensions of the pushButton
	 */
	public static void setDims(PushButton pushButton, Size newSize) {
		ScilabPushButtonBridge.setDims(pushButton, newSize);
	}

	/**
	 * Gets the position (X-coordinate and Y-coordinate) of a pushButton
	 * @param pushButton the pushButton we want to get the position of
	 * @return position of the pushButton
	 */
	public static Position getPosition(PushButton pushButton) {
		return ScilabPushButtonBridge.getPosition(pushButton);
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a pushButton
	 * @param pushButton the pushButton we want to set the position of
	 * @param newPosition new position of the pushButton
	 */
	public static void setPosition(PushButton pushButton, Position newPosition) {
		ScilabPushButtonBridge.setPosition(pushButton, newPosition);
	}

	/**
	 * Gets the visibility status of a pushButton
	 * @param pushButton the pushButton we want to get the status of
	 * @return visibility status (true if the pushButton is visible, false if not)
	 */
	public static boolean isVisible(PushButton pushButton) {
		return ScilabPushButtonBridge.isVisible(pushButton);
	}

	/**
	 * Sets the visibility status for a pushButton
	 * @param pushButton the pushButton we want to set the visibility status of
	 * @param newVisibleState new visibility status for the pushButton
	 * 			(true set the pushButton visible, false to set the pushButton invisible)
	 */
	public static void setVisible(PushButton pushButton, boolean newVisibleState) {
		ScilabPushButtonBridge.setVisible(pushButton, newVisibleState);
	}
	
	/**
	 * Sets the Text of a PushButton
	 * @param pushButton the PushButton we want to set the Text of
	 * @param newText the Text to set for the PushButton
	 */
	public static void setText(PushButton pushButton, String newText) {
		ScilabPushButtonBridge.setText(pushButton, newText);
	}
	
	/**
	 * Gets the Text of a PushButton
	 * @param pushButton the PushButton we want to get the Text of
	 * @return the Text of the PushButton
	 */
	public static String getText(PushButton pushButton) {
		return ScilabPushButtonBridge.getText(pushButton);
	}
	
	/**
	 * Sets the icon of a PushButton
	 * @param pushButton the PushButton we want to set the icon of
	 * @param filename the path to the icon image to set to the PushButton
	 */
	public static void setIcon(PushButton pushButton, String filename) {
		ScilabPushButtonBridge.setIcon(pushButton, filename);
	}
	
	/**
	 * Add a callback to the pushbutton
	 * @param pushButton the PushButton we want to set the callback of
	 * @param callback the CallBack to set.
	 */
	public static void setCallback(PushButton pushButton, CallBack callback) {
		ScilabPushButtonBridge.setCallback(pushButton, callback);
	}

	/**
	 * Set if the pushbutton is enabled or not
	 * @param pushButton the PushButton we want to set the status of
	 * @param status true if the pushbutton is enabled
	 */
	public static void setEnabled(PushButton pushButton, boolean status) {
		ScilabPushButtonBridge.setEnabled(pushButton, status);
	}
	
	/**
	 * Gets the enable status of a PushButton
	 * @param pushButton the PushButton we want to get the status of
	 * @return the enable status of the PushButton (true if the PushButton is enabled, false if not)
	 */
	public static boolean isEnabled(PushButton pushButton) {
		return ScilabPushButtonBridge.isEnabled(pushButton);
	}
	
	/**
	 * Set the Background color of the pushbutton
	 * @param pushButton the PushButton we want to set the background of
	 * @param color the Color
	 */
	public static void setBackground(PushButton pushButton, Color color) {
		ScilabPushButtonBridge.setBackground(pushButton, color);
	}

	/**
	 * Get the Background color of the pushbutton
	 * @param pushButton the PushButton we want to get the background of
	 * @return the Color
	 */
	public static Color getBackground(PushButton pushButton) {
		return ScilabPushButtonBridge.getBackground(pushButton);
	}

	/**
	 * Set the Foreground color of the pushbutton
	 * @param pushButton the PushButton we want to set the foreground of
	 * @param color the Color
	 */
	public static void setForeground(PushButton pushButton, Color color) {
		ScilabPushButtonBridge.setForeground(pushButton, color);
	}

	/**
	 * Get the Foreground color of the pushbutton
	 * @param pushButton the PushButton we want to get the foreground of
	 * @return the Color
	 */
	public static Color getForeground(PushButton pushButton) {
		return ScilabPushButtonBridge.getForeground(pushButton);
	}

	/**
	 * Set the font of the pushbutton.
	 * @param pushButton the pushbutton we want to set the font of
	 * @param font the font
	 */
	public static void setFont(PushButton pushButton, Font font) {
		ScilabPushButtonBridge.setFont(pushButton, font);
	}
	
	/**
	 * Get the font of the pushbutton.
	 * @param pushButton the pushbutton we want to get the font of
	 * @return the font
	 */
	public static Font getFont(PushButton pushButton) {
		return ScilabPushButtonBridge.getFont(pushButton);
	}

	/**
	 * Set the horizontal alignment for the PushButton text
	 * @param pushButton the PushButton we want to set the alignment of
	 * @param alignment the value for the alignment (See ScilabAlignment.java)
	 */
	public static void setHorizontalAlignment(PushButton pushButton, String alignment) {
		ScilabPushButtonBridge.setHorizontalAlignment(pushButton, alignment);
	}

	/**
	 * Set the vertical alignment for the PushButton text
	 * @param pushButton the PushButton we want to set the alignment of
	 * @param alignment the value for the alignment (See ScilabAlignment.java)
	 */
	public static void setVerticalAlignment(PushButton pushButton, String alignment) {
		ScilabPushButtonBridge.setVerticalAlignment(pushButton, alignment);
	}

	/**
	 * Set the Relief of the PushButton
	 * @param pushButton the PushButton which we want to set the Relief of
	 * @param reliefType the type of the relief to set (See ScilabRelief.java)
	 */
	public static void setRelief(PushButton pushButton, String reliefType) {
		ScilabPushButtonBridge.setRelief(pushButton, reliefType);
	}

	/**
	 * Delete the PushButton
	 * @param pushButton the PushButton to be destroyed
	 */
	public static void destroy(PushButton pushButton) {
		ScilabPushButtonBridge.destroy(pushButton);
	}

	/**
	 * Set the string to be displayed when the mouse cursor is over the PushButton
	 * @param pushButton the PushButton
	 * @param toolTipText the string to set
	 */
	public static void setToolTipText(PushButton pushButton, String toolTipText) {
		ScilabPushButtonBridge.setToolTipText(pushButton, toolTipText);
	}
	
	/**
	 * Give the focus to the PushButton
	 * @param pushButton the PushButton
	 */
	public static void requestFocus(PushButton pushButton) {
		ScilabPushButtonBridge.requestFocus(pushButton);
	}

	/******************/
	/* TextBox Bridge */
	/******************/

	/**
	 * Creates a new TextBox
	 * @return the created TextBox
	 */
	public static SimpleTextBox createTextBox() {
		return ScilabTextBoxBridge.createTextBox();
	}
	
	/**
	 * Sets the Text of a TextBox
	 * @param textBox the TextBox we want to set the Text of
	 * @param newText the Text to set for the TextBox
	 */
	public static void setText(TextBox textBox, String newText) {
		ScilabTextBoxBridge.setText(textBox, newText);
	}
	
	/**
	 * Gets the text of an TextBox
	 * @param textBox the TextBox we want to get the text of
	 * @return the text of the TextBox
	 */
	public static String getText(TextBox textBox) {
		return ScilabTextBoxBridge.getText(textBox);
	}
	
	/**
	 * Draws a Scilab TextBox
	 * @param textBox the TextBox to draw
	 * @see org.scilab.modules.ihm.UIElement#draw()
	 */
	public static void draw(TextBox textBox) {
		ScilabTextBoxBridge.draw(textBox);
	}

	/**
	 * Gets the dimensions (width and height) of a Scilab TextBox
	 * @param textBox the TextBox we want to get the dimensions of
	 * @return the size of the textBox
	 * @see org.scilab.modules.ihm.UIElement#getDims()
	 */
	public static Size getDims(TextBox textBox) {
		return ScilabTextBoxBridge.getDims(textBox);
	}

	/**
	 * Gets the position (X-coordinate and Y-coordinate) of a Scilab textBox
	 * @param textBox the textBox we want to get the position of
	 * @return the position of the textBox
	 * @see org.scilab.modules.ihm.UIElement#getPosition()
	 */
	public static Position getPosition(TextBox textBox) {
		return ScilabTextBoxBridge.getPosition(textBox);
	}

	/**
	 * Gets the visibility status of a Scilab TextBox
	 * @param textBox the textBox we want to get the visiblity status of
	 * @return the visibility status of the textBox (true if the textBox is visible, false if not)
	 * @see org.scilab.modules.ihm.UIElement#isVisible()
	 */
	public static boolean isVisible(TextBox textBox) {
		return ScilabTextBoxBridge.isVisible(textBox);
	}

	/**
	 * Set the status of the TextBox
	 * @param textBox the textBox we want to set the status of
	 * @param status true if the TextBox is enabled
	 * @see org.scilab.modules.gui.widget.Widget#setEnabled(boolean)
	 */
	public static void setEnabled(TextBox textBox, boolean status) {
		ScilabTextBoxBridge.setEnabled(textBox, status);
	}

	/**
	 * Gets the enable status of a TextBox
	 * @param textBox the textBox we want to get the status of
	 * @return the enable status of the TextBox (true if the TextBox is enabled, false if not)
	 */
	public static boolean isEnabled(TextBox textBox) {
		return ScilabTextBoxBridge.isEnabled(textBox);
	}

	/**
	 * Sets the dimensions (width and height) of a Scilab TextBox
	 * @param textBox the textBox we want to set the dimensions of
	 * @param newSize the size we want to set to the textBox
	 * @see org.scilab.modules.ihm.UIElement#setDims(org.scilab.modules.ihm.utils.Size)
	 */
	public static void setDims(TextBox textBox, Size newSize) {
		ScilabTextBoxBridge.setDims(textBox, newSize);
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a Scilab textBox
	 * @param textBox the textBox we want to set the position of
	 * @param newPosition the position we want to set to the textBox
	 * @see org.scilab.modules.ihm.UIElement#setPosition(org.scilab.modules.ihm.utils.Position)
	 */
	public static void setPosition(TextBox textBox, Position newPosition) {
		ScilabTextBoxBridge.setPosition(textBox, newPosition);
	}

	/**
	 * Sets the visibility status of a Scilab TextBox
	 * @param textBox the textBox we want to set the visiblity status of
	 * @param newVisibleState the visibility status we want to set to the textBox (true to set the textBox visible, false else)
	 * @see org.scilab.modules.ihm.UIElement#setVisible(boolean)
	 */
	public static void setVisible(TextBox textBox, boolean newVisibleState) {
		ScilabTextBoxBridge.setVisible(textBox, newVisibleState);
	}

	/**
	 * Set the Background color of the textbox
	 * @param textBox the textbox we want to set the background of
	 * @param color the Color
	 */
	public static void setBackground(TextBox textBox, Color color) {
		ScilabTextBoxBridge.setBackground(textBox, color);
	}

	/**
	 * Get the Background color of the textbox
	 * @param textBox the textbox we want to get the background of
	 * @return the Color
	 */
	public static Color getBackground(TextBox textBox) {
		return ScilabTextBoxBridge.getBackground(textBox);
	}

	/**
	 * Set the Foreground color of the textbox
	 * @param textBox the textbox we want to set the foreground of
	 * @param color the Color
	 */
	public static void setForeground(TextBox textBox, Color color) {
		ScilabTextBoxBridge.setForeground(textBox, color);
	}

	/**
	 * Get the Foreground color of the textbox
	 * @param textBox the textbox we want to get the foreground of
	 * @return the Color
	 */
	public static Color getForeground(TextBox textBox) {
		return ScilabTextBoxBridge.getForeground(textBox);
	}

	/**
	 * Set the font of the textbox.
	 * @param textBox the textbox we want to set the font of
	 * @param font the font
	 */
	public static void setFont(TextBox textBox, Font font) {
		ScilabTextBoxBridge.setFont(textBox, font);
	}
	
	/**
	 * Get the font of the textbox.
	 * @param textBox the textbox we want to get the font of
	 * @return the font
	 */
	public static Font getFont(TextBox textBox) {
		return ScilabTextBoxBridge.getFont(textBox);
	}

	/**
	 * Add a callback to the TextBox
	 * @param textBox the TextBox we want to set the callback of
	 * @param callback the CallBack to set.
	 */
	public static void setCallback(TextBox textBox, CallBack callback) {
		ScilabTextBoxBridge.setCallback(textBox, callback);
	}

	/**
	 * Set the horizontal alignment for the TextBox text
	 * @param textBox the TextBox we want to set the alignment of
	 * @param alignment the value for the alignment (See ScilabAlignment.java)
	 */
	public static void setHorizontalAlignment(TextBox textBox, String alignment) {
		ScilabTextBoxBridge.setHorizontalAlignment(textBox, alignment);
	}

	/**
	 * Set the vertical alignment for the TextBox text
	 * @param textBox the TextBox we want to set the alignment of
	 * @param alignment the value for the alignment (See ScilabAlignment.java)
	 */
	public static void setVerticalAlignment(TextBox textBox, String alignment) {
		ScilabTextBoxBridge.setVerticalAlignment(textBox, alignment);
	}

	/**
	 * Set the Relief of the TextBox
	 * @param textBox the TextBox which we want to set the Relief of
	 * @param reliefType the type of the relief to set (See ScilabRelief.java)
	 */
	public static void setRelief(TextBox textBox, String reliefType) {
		ScilabTextBoxBridge.setRelief(textBox, reliefType);
	}

	/**
	 * Delete the TextBox
	 * @param textBox the TextBox to be destroyed
	 */
	public static void destroy(TextBox textBox) {
		ScilabTextBoxBridge.destroy(textBox);
	}

	/**
	 * Give the focus to the TextBox
	 * @param textBox the TextBox
	 */
	public static void requestFocus(TextBox textBox) {
		ScilabTextBoxBridge.requestFocus(textBox);
	}

	/******************/
	/* EditBox Bridge */
	/******************/

	/**
	 * Creates a new EditBox
	 * @return the created EditBox
	 */
	public static SimpleEditBox createEditBox() {
		return ScilabEditBoxBridge.createEditBox();
	}
	
	/**
	 * Sets the Text of a EditBox
	 * @param editBox the EditBox we want to set the Text of
	 * @param newText the Text to set for the EditBox
	 */
	public static void setText(EditBox editBox, String newText) {
		ScilabEditBoxBridge.setText(editBox, newText);
	}
	
	/**
	 * Gets the text of an EditBox
	 * @param editBox the EditBox we want to get the text of
	 * @return the text of the EditBox
	 */
	public static String getText(EditBox editBox) {
		return ScilabEditBoxBridge.getText(editBox);
	}
	
	/**
	 * Draws a Scilab EditBox
	 * @param editBox the EditBox to draw
	 * @see org.scilab.modules.ihm.UIElement#draw()
	 */
	public static void draw(EditBox editBox) {
		ScilabEditBoxBridge.draw(editBox);
	}

	/**
	 * Gets the dimensions (width and height) of a Scilab EditBox
	 * @param editBox the EditBox we want to get the dimensions of
	 * @return the size of the editBox
	 * @see org.scilab.modules.ihm.UIElement#getDims()
	 */
	public static Size getDims(EditBox editBox) {
		return ScilabEditBoxBridge.getDims(editBox);
	}

	/**
	 * Gets the position (X-coordinate and Y-coordinate) of a Scilab editBox
	 * @param editBox the editBox we want to get the position of
	 * @return the position of the editBox
	 * @see org.scilab.modules.ihm.UIElement#getPosition()
	 */
	public static Position getPosition(EditBox editBox) {
		return ScilabEditBoxBridge.getPosition(editBox);
	}

	/**
	 * Gets the visibility status of a Scilab EditBox
	 * @param editBox the editBox we want to get the visiblity status of
	 * @return the visibility status of the editBox (true if the editBox is visible, false if not)
	 * @see org.scilab.modules.ihm.UIElement#isVisible()
	 */
	public static boolean isVisible(EditBox editBox) {
		return ScilabEditBoxBridge.isVisible(editBox);
	}

	/**
	 * Sets the dimensions (width and height) of a Scilab EditBox
	 * @param editBox the editBox we want to set the dimensions of
	 * @param newSize the size we want to set to the editBox
	 * @see org.scilab.modules.ihm.UIElement#setDims(org.scilab.modules.ihm.utils.Size)
	 */
	public static void setDims(EditBox editBox, Size newSize) {
		ScilabEditBoxBridge.setDims(editBox, newSize);
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a Scilab editBox
	 * @param editBox the editBox we want to set the position of
	 * @param newPosition the position we want to set to the editBox
	 * @see org.scilab.modules.ihm.UIElement#setPosition(org.scilab.modules.ihm.utils.Position)
	 */
	public static void setPosition(EditBox editBox, Position newPosition) {
		ScilabEditBoxBridge.setPosition(editBox, newPosition);
	}

	/**
	 * Sets the visibility status of a Scilab EditBox
	 * @param editBox the editBox we want to set the visiblity status of
	 * @param newVisibleState the visibility status we want to set to the editBox (true to set the editBox visible, false else)
	 * @see org.scilab.modules.ihm.UIElement#setVisible(boolean)
	 */
	public static void setVisible(EditBox editBox, boolean newVisibleState) {
		ScilabEditBoxBridge.setVisible(editBox, newVisibleState);
	}
	
	/**
	 * Set the Background color of the EditBox
	 * @param editBox the EditBox we want to set the background of
	 * @param color the Color
	 */
	public static void setBackground(EditBox editBox, Color color) {
		ScilabEditBoxBridge.setBackground(editBox, color);
	}

	/**
	 * Get the Background color of the EditBox
	 * @param editBox the EditBox we want to get the background of
	 * @return the Color
	 */
	public static Color getBackground(EditBox editBox) {
		return ScilabEditBoxBridge.getBackground(editBox);
	}

	/**
	 * Set the Foreground color of the EditBox
	 * @param editBox the EditBox we want to set the foreground of
	 * @param color the Color
	 */
	public static void setForeground(EditBox editBox, Color color) {
		ScilabEditBoxBridge.setForeground(editBox, color);
	}

	/**
	 * Get the Foreground color of the EditBox
	 * @param editBox the EditBox we want to get the foreground of
	 * @return the Color
	 */
	public static Color getForeground(EditBox editBox) {
		return ScilabEditBoxBridge.getForeground(editBox);
	}

	/**
	 * Set the font of the EditBox.
	 * @param editBox the EditBox we want to set the font of
	 * @param font the font
	 */
	public static void setFont(EditBox editBox, Font font) {
		ScilabEditBoxBridge.setFont(editBox, font);
	}
	
	/**
	 * Get the font of the EditBox.
	 * @param editBox the EditBox we want to get the font of
	 * @return the font
	 */
	public static Font getFont(EditBox editBox) {
		return ScilabEditBoxBridge.getFont(editBox);
	}

	/**
	 * Set if the EditBox is enabled or not
	 * @param editBox the EditBox we want to set the status of
	 * @param status true if the EditBox is enabled
	 */
	public static void setEnabled(EditBox editBox, boolean status) {
		ScilabEditBoxBridge.setEnabled(editBox, status);
	}
	
	/**
	 * Gets the enable status of an EditBox
	 * @param editBox the EditBox we want to get the status of
	 * @return the enable status of the EditBox (true if the EditBox is enabled, false if not)
	 */
	public static boolean isEnabled(EditBox editBox) {
		return ScilabEditBoxBridge.isEnabled(editBox);
	}
	
	/**
	 * Add a callback to the EditBox
	 * @param editBox the EditBox we want to set the callback of
	 * @param callback the Callback to set.
	 */
	public static void setCallback(EditBox editBox, CallBack callback) {
		ScilabEditBoxBridge.setCallback(editBox, callback);
	}

	/**
	 * Set the horizontal alignment for the EditBox text
	 * @param editBox the EditBox we want to set the alignment of
	 * @param alignment the value for the alignment (See ScilabAlignment.java)
	 */
	public static void setHorizontalAlignment(EditBox editBox, String alignment) {
		ScilabEditBoxBridge.setHorizontalAlignment(editBox, alignment);
	}

	/**
	 * Set the vertical alignment for the EditBox text
	 * @param editBox the EditBox we want to set the alignment of
	 * @param alignment the value for the alignment (See ScilabAlignment.java)
	 */
	public static void setVerticalAlignment(EditBox editBox, String alignment) {
		ScilabEditBoxBridge.setVerticalAlignment(editBox, alignment);
	}

	/**
	 * Set the Relief of the EditBox
	 * @param editBox the EditBox which we want to set the Relief of
	 * @param reliefType the type of the relief to set (See ScilabRelief.java)
	 */
	public static void setRelief(EditBox editBox, String reliefType) {
		ScilabEditBoxBridge.setRelief(editBox, reliefType);
	}

	/**
	 * Delete the EditBox
	 * @param editBox the EditBox to be destroyed
	 */
	public static void destroy(EditBox editBox) {
		ScilabEditBoxBridge.destroy(editBox);
	}

	/**
	 * Give the focus to the EditBox
	 * @param editBox the EditBox
	 */
	public static void requestFocus(EditBox editBox) {
		ScilabEditBoxBridge.requestFocus(editBox);
	}

	/****************/
	/* Label Bridge */
	/****************/

	/**
	 * Creates a new Label
	 * @return the created Label
	 */
	public static SimpleLabel createLabel() {
		return ScilabLabelBridge.createLabel();
	}
	
	/**
	 * Sets the Text of a Label
	 * @param label the Label we want to set the Text of
	 * @param newText the Text to set for the Label
	 */
	public static void setText(Label label, String newText) {
		ScilabLabelBridge.setText(label, newText);
	}
	
	/**
	 * Gets the text of an Label
	 * @param label the Label we want to get the text of
	 * @return the text of the Label
	 */
	public static String getText(Label label) {
		return ScilabLabelBridge.getText(label);
	}
	
	/**
	 * Draws a Scilab Label
	 * @param label the Label to draw
	 * @see org.scilab.modules.ihm.UIElement#draw()
	 */
	public static void draw(Label label) {
		ScilabLabelBridge.draw(label);
	}

	/**
	 * Gets the dimensions (width and height) of a Scilab Label
	 * @param label the Label we want to get the dimensions of
	 * @return the size of the label
	 * @see org.scilab.modules.ihm.UIElement#getDims()
	 */
	public static Size getDims(Label label) {
		return ScilabLabelBridge.getDims(label);
	}

	/**
	 * Gets the position (X-coordinate and Y-coordinate) of a Scilab label
	 * @param label the label we want to get the position of
	 * @return the position of the label
	 * @see org.scilab.modules.ihm.UIElement#getPosition()
	 */
	public static Position getPosition(Label label) {
		return ScilabLabelBridge.getPosition(label);
	}

	/**
	 * Gets the visibility status of a Scilab Label
	 * @param label the label we want to get the visiblity status of
	 * @return the visibility status of the label (true if the label is visible, false if not)
	 * @see org.scilab.modules.ihm.UIElement#isVisible()
	 */
	public static boolean isVisible(Label label) {
		return ScilabLabelBridge.isVisible(label);
	}

	/**
	 * Sets the dimensions (width and height) of a Scilab Label
	 * @param label the label we want to set the dimensions of
	 * @param newSize the size we want to set to the label
	 * @see org.scilab.modules.ihm.UIElement#setDims(org.scilab.modules.ihm.utils.Size)
	 */
	public static void setDims(Label label, Size newSize) {
		ScilabLabelBridge.setDims(label, newSize);
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a Scilab label
	 * @param label the label we want to set the position of
	 * @param newPosition the position we want to set to the label
	 * @see org.scilab.modules.ihm.UIElement#setPosition(org.scilab.modules.ihm.utils.Position)
	 */
	public static void setPosition(Label label, Position newPosition) {
		ScilabLabelBridge.setPosition(label, newPosition);
	}

	/**
	 * Sets the visibility status of a Scilab Label
	 * @param label the label we want to set the visiblity status of
	 * @param newVisibleState the visibility status we want to set to the label (true to set the label visible, false else)
	 * @see org.scilab.modules.ihm.UIElement#setVisible(boolean)
	 */
	public static void setVisible(Label label, boolean newVisibleState) {
		ScilabLabelBridge.setVisible(label, newVisibleState);
	}
	
	/**
	 * Set the Background color of the Label
	 * @param label the Label we want to set the background of
	 * @param color the Color
	 */
	public static void setBackground(Label label, Color color) {
		ScilabLabelBridge.setBackground(label, color);
	}

	/**
	 * Get the Background color of the Label
	 * @param label the Label we want to get the background of
	 * @return the Color
	 */
	public static Color getBackground(Label label) {
		return ScilabLabelBridge.getBackground(label);
	}

	/**
	 * Set the Foreground color of the Label
	 * @param label the Label we want to set the foreground of
	 * @param color the Color
	 */
	public static void setForeground(Label label, Color color) {
		ScilabLabelBridge.setForeground(label, color);
	}

	/**
	 * Get the Foreground color of the Label
	 * @param label the Label we want to get the foreground of
	 * @return the Color
	 */
	public static Color getForeground(Label label) {
		return ScilabLabelBridge.getForeground(label);
	}

	/**
	 * Set the font of the Label.
	 * @param label the Label we want to set the font of
	 * @param font the font
	 */
	public static void setFont(Label label, Font font) {
		ScilabLabelBridge.setFont(label, font);
	}
	
	/**
	 * Get the font of the Label.
	 * @param label the Label we want to get the font of
	 * @return the font
	 */
	public static Font getFont(Label label) {
		return ScilabLabelBridge.getFont(label);
	}

	/**
	 * Set if the Label is enabled or not
	 * @param label the Label we want to set the status of
	 * @param status true if the Label is enabled
	 */
	public static void setEnabled(Label label, boolean status) {
		ScilabLabelBridge.setEnabled(label, status);
	}
	
	/**
	 * Gets the enable status of a Label
	 * @param label the Label we want to get the status of
	 * @return the enable status of the Label (true if the Label is enabled, false if not)
	 */
	public static boolean isEnabled(Label label) {
		return ScilabLabelBridge.isEnabled(label);
	}
	
	/**
	 * Add a callback to the Label
	 * @param label the Label we want to set the callback of
	 * @param callback the Callback to set.
	 */
	public static void setCallback(Label label, CallBack callback) {
		ScilabLabelBridge.setCallback(label, callback);
	}

	/**
	 * Set the horizontal alignment for the Label text
	 * @param label the Label we want to set the alignment of
	 * @param alignment the value for the alignment (See ScilabAlignment.java)
	 */
	public static void setHorizontalAlignment(Label label, String alignment) {
		ScilabLabelBridge.setHorizontalAlignment(label, alignment);
	}

	/**
	 * Set the vertical alignment for the Label text
	 * @param label the Label we want to set the alignment of
	 * @param alignment the value for the alignment (See ScilabAlignment.java)
	 */
	public static void setVerticalAlignment(Label label, String alignment) {
		ScilabLabelBridge.setVerticalAlignment(label, alignment);
	}
	
	/**
	 * Set the Relief of the Label
	 * @param label the Label which we want to set the Relief of
	 * @param reliefType the type of the relief to set (See ScilabRelief.java)
	 */
	public static void setRelief(Label label, String reliefType) {
		ScilabLabelBridge.setRelief(label, reliefType);
	}

	/**
	 * Delete the Label
	 * @param label the Label to be destroyed
	 */
	public static void destroy(Label label) {
		ScilabLabelBridge.destroy(label);
	}

	/**
	 * Give the focus to the Label
	 * @param label the Label
	 */
	public static void requestFocus(Label label) {
		ScilabLabelBridge.requestFocus(label);
	}

	/*******************/
	/* CheckBox Bridge */
	/*******************/

	/**
	 * Creates a new CheckBox
	 * @return the created CheckBox
	 */
	public static SimpleCheckBox createCheckBox() {
		return ScilabCheckBoxBridge.createCheckBox();
	}
	
	/**
	 * Sets the Text of a CheckBox
	 * @param checkBox the CheckBox we want to set the Text of
	 * @param newText the Text to set for the CheckBox
	 */
	public static void setText(CheckBox checkBox, String newText) {
		ScilabCheckBoxBridge.setText(checkBox, newText);
	}
	
	/**
	 * Gets the text of an CheckBox
	 * @param checkBox the CheckBox we want to get the text of
	 * @return the text of the CheckBox
	 */
	public static String getText(CheckBox checkBox) {
		return ScilabCheckBoxBridge.getText(checkBox);
	}
	
	/**
	 * Draws a Scilab CheckBox
	 * @param checkBox the CheckBox to draw
	 * @see org.scilab.modules.ihm.UIElement#draw()
	 */
	public static void draw(CheckBox checkBox) {
		ScilabCheckBoxBridge.draw(checkBox);
	}

	/**
	 * Gets the dimensions (width and height) of a Scilab CheckBox
	 * @param checkBox the CheckBox we want to get the dimensions of
	 * @return the size of the checkBox
	 * @see org.scilab.modules.ihm.UIElement#getDims()
	 */
	public static Size getDims(CheckBox checkBox) {
		return ScilabCheckBoxBridge.getDims(checkBox);
	}

	/**
	 * Gets the position (X-coordinate and Y-coordinate) of a Scilab checkBox
	 * @param checkBox the checkBox we want to get the position of
	 * @return the position of the checkBox
	 * @see org.scilab.modules.ihm.UIElement#getPosition()
	 */
	public static Position getPosition(CheckBox checkBox) {
		return ScilabCheckBoxBridge.getPosition(checkBox);
	}

	/**
	 * Gets the visibility status of a Scilab CheckBox
	 * @param checkBox the checkBox we want to get the visiblity status of
	 * @return the visibility status of the checkBox (true if the checkBox is visible, false if not)
	 * @see org.scilab.modules.ihm.UIElement#isVisible()
	 */
	public static boolean isVisible(CheckBox checkBox) {
		return ScilabCheckBoxBridge.isVisible(checkBox);
	}

	/**
	 * Sets the dimensions (width and height) of a Scilab CheckBox
	 * @param checkBox the checkBox we want to set the dimensions of
	 * @param newSize the size we want to set to the checkBox
	 * @see org.scilab.modules.ihm.UIElement#setDims(org.scilab.modules.ihm.utils.Size)
	 */
	public static void setDims(CheckBox checkBox, Size newSize) {
		ScilabCheckBoxBridge.setDims(checkBox, newSize);
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a Scilab checkBox
	 * @param checkBox the checkBox we want to set the position of
	 * @param newPosition the position we want to set to the checkBox
	 * @see org.scilab.modules.ihm.UIElement#setPosition(org.scilab.modules.ihm.utils.Position)
	 */
	public static void setPosition(CheckBox checkBox, Position newPosition) {
		ScilabCheckBoxBridge.setPosition(checkBox, newPosition);
	}

	/**
	 * Sets the visibility status of a Scilab CheckBox
	 * @param checkBox the checkBox we want to set the visiblity status of
	 * @param newVisibleState the visibility status we want to set to the checkBox (true to set the checkBox visible, false else)
	 * @see org.scilab.modules.ihm.UIElement#setVisible(boolean)
	 */
	public static void setVisible(CheckBox checkBox, boolean newVisibleState) {
		ScilabCheckBoxBridge.setVisible(checkBox, newVisibleState);
	}
	
	/**
	 * Set the Background color of the CheckBox
	 * @param checkBox the CheckBox we want to set the background of
	 * @param color the Color
	 */
	public static void setBackground(CheckBox checkBox, Color color) {
		ScilabCheckBoxBridge.setBackground(checkBox, color);
	}

	/**
	 * Get the Background color of the CheckBox
	 * @param checkBox the CheckBox we want to get the background of
	 * @return the Color
	 */
	public static Color getBackground(CheckBox checkBox) {
		return ScilabCheckBoxBridge.getBackground(checkBox);
	}

	/**
	 * Set the Foreground color of the CheckBox
	 * @param checkBox the CheckBox we want to set the foreground of
	 * @param color the Color
	 */
	public static void setForeground(CheckBox checkBox, Color color) {
		ScilabCheckBoxBridge.setForeground(checkBox, color);
	}

	/**
	 * Get the Foreground color of the CheckBox
	 * @param checkBox the CheckBox we want to get the foreground of
	 * @return the Color
	 */
	public static Color getForeground(CheckBox checkBox) {
		return ScilabCheckBoxBridge.getForeground(checkBox);
	}

	/**
	 * Set the font of the CheckBox.
	 * @param checkBox the CheckBox we want to set the font of
	 * @param font the font
	 */
	public static void setFont(CheckBox checkBox, Font font) {
		ScilabCheckBoxBridge.setFont(checkBox, font);
	}
	
	/**
	 * Get the font of the CheckBox.
	 * @param checkBox the CheckBox we want to get the font of
	 * @return the font
	 */
	public static Font getFont(CheckBox checkBox) {
		return ScilabCheckBoxBridge.getFont(checkBox);
	}

	/**
	 * Set if the CheckBox is enabled or not
	 * @param checkBox the CheckBox we want to set the status of
	 * @param status true if the CheckBox is enabled
	 */
	public static void setEnabled(CheckBox checkBox, boolean status) {
		ScilabCheckBoxBridge.setEnabled(checkBox, status);
	}
	
	/**
	 * Gets the enable status of the CheckBox
	 * @param checkBox the CheckBox we want to get the status of
	 * @return the enable status of the CheckBox (true if the CheckBox is enabled, false if not)
	 */
	public static boolean isEnabled(CheckBox checkBox) {
		return ScilabCheckBoxBridge.isEnabled(checkBox);
	}

	/**
	 * Add a callback to the CheckBox
	 * @param checkBox the CheckBox we want to set the callback of
	 * @param callback the CallBack to set.
	 */
	public static void setCallback(CheckBox checkBox, CallBack callback) {
		ScilabCheckBoxBridge.setCallback(checkBox, callback);
	}

	/**
	 * Set the horizontal alignment for the CheckBox text
	 * @param checkBox the CheckBox we want to set the alignment of
	 * @param alignment the value for the alignment (See ScilabAlignment.java)
	 */
	public static void setHorizontalAlignment(CheckBox checkBox, String alignment) {
		ScilabCheckBoxBridge.setHorizontalAlignment(checkBox, alignment);
	}

	/**
	 * Set the vertical alignment for the CheckBox text
	 * @param checkBox the CheckBox we want to set the alignment of
	 * @param alignment the value for the alignment (See ScilabAlignment.java)
	 */
	public static void setVerticalAlignment(CheckBox checkBox, String alignment) {
		ScilabCheckBoxBridge.setVerticalAlignment(checkBox, alignment);
	}

	/**
	 * Set if the CheckBox is checked or not
	 * @param checkBox the CheckBox we want to set the status of
	 * @param status true to set the CheckBox checked
	 */
	public static void setChecked(CheckBox checkBox, boolean status) {
		ScilabCheckBoxBridge.setChecked(checkBox, status);
	}
	
	/**
	 * Get the status of the CheckBox
	 * @param checkBox the CheckBox we want to get the status of
	 * @return true if the CheckBox is checked
	 */
	public static boolean isChecked(CheckBox checkBox) {
		return ScilabCheckBoxBridge.isChecked(checkBox);
	}

	/**
	 * Set the Relief of the CheckBox
	 * @param checkBox the CheckBox which we want to set the Relief of
	 * @param reliefType the type of the relief to set (See ScilabRelief.java)
	 */
	public static void setRelief(CheckBox checkBox, String reliefType) {
		ScilabCheckBoxBridge.setRelief(checkBox, reliefType);
	}

	/**
	 * Delete the CheckBox
	 * @param checkBox the CheckBox to be destroyed
	 */
	public static void destroy(CheckBox checkBox) {
		ScilabCheckBoxBridge.destroy(checkBox);
	}

	/**
	 * Give the focus to the CheckBox
	 * @param checkBox the CheckBox
	 */
	public static void requestFocus(CheckBox checkBox) {
		ScilabCheckBoxBridge.requestFocus(checkBox);
	}

	/**********************/
	/* RadioButton Bridge */
	/**********************/

	/**
	 * Creates a new RadioButton
	 * @return the created RadioButton
	 */
	public static SimpleRadioButton createRadioButton() {
		return ScilabRadioButtonBridge.createRadioButton();
	}
	
	/**
	 * Sets the Text of a RadioButton
	 * @param radioButton the RadioButton we want to set the Text of
	 * @param newText the Text to set for the RadioButton
	 */
	public static void setText(RadioButton radioButton, String newText) {
		ScilabRadioButtonBridge.setText(radioButton, newText);
	}
	
	/**
	 * Gets the text of an RadioButton
	 * @param radioButton the RadioButton we want to get the text of
	 * @return the text of the RadioButton
	 */
	public static String getText(RadioButton radioButton) {
		return ScilabRadioButtonBridge.getText(radioButton);
	}
	
	/**
	 * Draws a Scilab RadioButton
	 * @param radioButton the RadioButton to draw
	 * @see org.scilab.modules.ihm.UIElement#draw()
	 */
	public static void draw(RadioButton radioButton) {
		ScilabRadioButtonBridge.draw(radioButton);
	}

	/**
	 * Gets the dimensions (width and height) of a Scilab RadioButton
	 * @param radioButton the RadioButton we want to get the dimensions of
	 * @return the size of the radioButton
	 * @see org.scilab.modules.ihm.UIElement#getDims()
	 */
	public static Size getDims(RadioButton radioButton) {
		return ScilabRadioButtonBridge.getDims(radioButton);
	}

	/**
	 * Gets the position (X-coordinate and Y-coordinate) of a Scilab radioButton
	 * @param radioButton the radioButton we want to get the position of
	 * @return the position of the radioButton
	 * @see org.scilab.modules.ihm.UIElement#getPosition()
	 */
	public static Position getPosition(RadioButton radioButton) {
		return ScilabRadioButtonBridge.getPosition(radioButton);
	}

	/**
	 * Gets the visibility status of a Scilab RadioButton
	 * @param radioButton the radioButton we want to get the visiblity status of
	 * @return the visibility status of the radioButton (true if the radioButton is visible, false if not)
	 * @see org.scilab.modules.ihm.UIElement#isVisible()
	 */
	public static boolean isVisible(RadioButton radioButton) {
		return ScilabRadioButtonBridge.isVisible(radioButton);
	}

	/**
	 * Sets the dimensions (width and height) of a Scilab RadioButton
	 * @param radioButton the radioButton we want to set the dimensions of
	 * @param newSize the size we want to set to the radioButton
	 * @see org.scilab.modules.ihm.UIElement#setDims(org.scilab.modules.ihm.utils.Size)
	 */
	public static void setDims(RadioButton radioButton, Size newSize) {
		ScilabRadioButtonBridge.setDims(radioButton, newSize);
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a Scilab radioButton
	 * @param radioButton the radioButton we want to set the position of
	 * @param newPosition the position we want to set to the radioButton
	 * @see org.scilab.modules.ihm.UIElement#setPosition(org.scilab.modules.ihm.utils.Position)
	 */
	public static void setPosition(RadioButton radioButton, Position newPosition) {
		ScilabRadioButtonBridge.setPosition(radioButton, newPosition);
	}

	/**
	 * Sets the visibility status of a Scilab RadioButton
	 * @param radioButton the radioButton we want to set the visiblity status of
	 * @param newVisibleState the visibility status we want to set to the radioButton (true to set the radioButton visible, false else)
	 * @see org.scilab.modules.ihm.UIElement#setVisible(boolean)
	 */
	public static void setVisible(RadioButton radioButton, boolean newVisibleState) {
		ScilabRadioButtonBridge.setVisible(radioButton, newVisibleState);
	}
	
	/**
	 * Set the Background color of the RadioButton
	 * @param radioButton the RadioButton we want to set the background of
	 * @param color the Color
	 */
	public static void setBackground(RadioButton radioButton, Color color) {
		ScilabRadioButtonBridge.setBackground(radioButton, color);
	}

	/**
	 * Get the Background color of the RadioButton
	 * @param radioButton the RadioButton we want to get the background of
	 * @return the Color
	 */
	public static Color getBackground(RadioButton radioButton) {
		return ScilabRadioButtonBridge.getBackground(radioButton);
	}

	/**
	 * Set the Foreground color of the RadioButton
	 * @param radioButton the RadioButton we want to set the foreground of
	 * @param color the Color
	 */
	public static void setForeground(RadioButton radioButton, Color color) {
		ScilabRadioButtonBridge.setForeground(radioButton, color);
	}

	/**
	 * Get the Foreground color of the RadioButton
	 * @param radioButton the RadioButton we want to get the foreground of
	 * @return the Color
	 */
	public static Color getForeground(RadioButton radioButton) {
		return ScilabRadioButtonBridge.getForeground(radioButton);
	}

	/**
	 * Set the font of the RadioButton.
	 * @param radioButton the RadioButton we want to set the font of
	 * @param font the font
	 */
	public static void setFont(RadioButton radioButton, Font font) {
		ScilabRadioButtonBridge.setFont(radioButton, font);
	}
	
	/**
	 * Get the font of the RadioButton.
	 * @param radioButton the RadioButton we want to get the font of
	 * @return the font
	 */
	public static Font getFont(RadioButton radioButton) {
		return ScilabRadioButtonBridge.getFont(radioButton);
	}

	/**
	 * Set if the RadioButton is enabled or not
	 * @param radioButton the RadioButton we want to set the status of
	 * @param status true if the RadioButton is enabled
	 */
	public static void setEnabled(RadioButton radioButton, boolean status) {
		ScilabRadioButtonBridge.setEnabled(radioButton, status);
	}
	
	/**
	 * Gets the enable status of a RadioButton
	 * @param radioButton the RadioButton we want to get the status of
	 * @return the enable status of the RadioButton (true if the RadioButton is enabled, false if not)
	 */
	public static boolean isEnabled(RadioButton radioButton) {
		return ScilabRadioButtonBridge.isEnabled(radioButton);
	}
	
	/**
	 * Add a callback to the RadioButton
	 * @param radioButton the RadioButton we want to set the callback of
	 * @param callback the callback to set.
	 */
	public static void setCallback(RadioButton radioButton, CallBack callback) {
		ScilabRadioButtonBridge.setCallback(radioButton, callback);
	}

	/**
	 * Set the horizontal alignment for the RadioButton text
	 * @param radioButton the RadioButton we want to set the alignment of
	 * @param alignment the value for the alignment (See ScilabAlignment.java)
	 */
	public static void setHorizontalAlignment(RadioButton radioButton, String alignment) {
		ScilabRadioButtonBridge.setHorizontalAlignment(radioButton, alignment);
	}

	/**
	 * Set the vertical alignment for the RadioButton text
	 * @param radioButton the RadioButton we want to set the alignment of
	 * @param alignment the value for the alignment (See ScilabAlignment.java)
	 */
	public static void setVerticalAlignment(RadioButton radioButton, String alignment) {
		ScilabRadioButtonBridge.setVerticalAlignment(radioButton, alignment);
	}

	/**
	 * Set if the RadioButton is checked or not
	 * @param radioButton the RadioButton we want to set the status of
	 * @param status true to set the RadioButon checked
	 */
	public static void setChecked(RadioButton radioButton, boolean status) {
		ScilabRadioButtonBridge.setChecked(radioButton, status);
	}
	
	/**
	 * Get the status of the RadioButton
	 * @param radioButton the RadioButton we want to get the status of
	 * @return true if the RadioButton is checked
	 */
	public static boolean isChecked(RadioButton radioButton) {
		return ScilabRadioButtonBridge.isChecked(radioButton);
	}

	/**
	 * Set the Relief of the RadioButton
	 * @param radioButton the RadioButton which we want to set the Relief of
	 * @param reliefType the type of the relief to set (See ScilabRelief.java)
	 */
	public static void setRelief(RadioButton radioButton, String reliefType) {
		ScilabRadioButtonBridge.setRelief(radioButton, reliefType);
	}

	/**
	 * Delete the RadioButton
	 * @param radioButton the RadioButton to be destroyed
	 */
	public static void destroy(RadioButton radioButton) {
		ScilabRadioButtonBridge.destroy(radioButton);
	}

	/**
	 * Give the focus to the RadioButton
	 * @param radioButton the RadioButton
	 */
	public static void requestFocus(RadioButton radioButton) {
		ScilabRadioButtonBridge.requestFocus(radioButton);
	}

	/*****************/
	/* Slider Bridge */
	/*****************/

	/**
	 * Creates a new Slider
	 * @return the created Slider
	 */
	public static SimpleSlider createSlider() {
		return ScilabSliderBridge.createSlider();
	}
	
	/**
	 * Sets the Text of a Slider
	 * @param slider the Slider we want to set the Text of
	 * @param newText the Text to set for the Slider
	 */
	public static void setText(Slider slider, String newText) {
		ScilabSliderBridge.setText(slider, newText);
	}
	
	/**
	 * Gets the text of an Slider
	 * @param slider the Slider we want to get the text of
	 * @return the text of the Slider
	 */
	public static String getText(Slider slider) {
		return ScilabSliderBridge.getText(slider);
	}
	
	/**
	 * Draws a Scilab Slider
	 * @param slider the Slider to draw
	 * @see org.scilab.modules.ihm.UIElement#draw()
	 */
	public static void draw(Slider slider) {
		ScilabSliderBridge.draw(slider);
	}

	/**
	 * Gets the dimensions (width and height) of a Scilab Slider
	 * @param slider the Slider we want to get the dimensions of
	 * @return the size of the slider
	 * @see org.scilab.modules.ihm.UIElement#getDims()
	 */
	public static Size getDims(Slider slider) {
		return ScilabSliderBridge.getDims(slider);
	}

	/**
	 * Gets the position (X-coordinate and Y-coordinate) of a Scilab slider
	 * @param slider the slider we want to get the position of
	 * @return the position of the slider
	 * @see org.scilab.modules.ihm.UIElement#getPosition()
	 */
	public static Position getPosition(Slider slider) {
		return ScilabSliderBridge.getPosition(slider);
	}

	/**
	 * Gets the visibility status of a Scilab Slider
	 * @param slider the slider we want to get the visiblity status of
	 * @return the visibility status of the slider (true if the slider is visible, false if not)
	 * @see org.scilab.modules.ihm.UIElement#isVisible()
	 */
	public static boolean isVisible(Slider slider) {
		return ScilabSliderBridge.isVisible(slider);
	}

	/**
	 * Sets the dimensions (width and height) of a Scilab Slider
	 * @param slider the slider we want to set the dimensions of
	 * @param newSize the size we want to set to the slider
	 * @see org.scilab.modules.ihm.UIElement#setDims(org.scilab.modules.ihm.utils.Size)
	 */
	public static void setDims(Slider slider, Size newSize) {
		ScilabSliderBridge.setDims(slider, newSize);
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a Scilab slider
	 * @param slider the slider we want to set the position of
	 * @param newPosition the position we want to set to the slider
	 * @see org.scilab.modules.ihm.UIElement#setPosition(org.scilab.modules.ihm.utils.Position)
	 */
	public static void setPosition(Slider slider, Position newPosition) {
		ScilabSliderBridge.setPosition(slider, newPosition);
	}

	/**
	 * Sets the visibility status of a Scilab Slider
	 * @param slider the slider we want to set the visiblity status of
	 * @param newVisibleState the visibility status we want to set to the slider (true to set the slider visible, false else)
	 * @see org.scilab.modules.ihm.UIElement#setVisible(boolean)
	 */
	public static void setVisible(Slider slider, boolean newVisibleState) {
		ScilabSliderBridge.setVisible(slider, newVisibleState);
	}
	
	/**
	 * Set the Background color of the Slider
	 * @param slider the Slider we want to set the background of
	 * @param color the Color
	 */
	public static void setBackground(Slider slider, Color color) {
		ScilabSliderBridge.setBackground(slider, color);
	}

	/**
	 * Get the Background color of the Slider
	 * @param slider the Slider we want to get the background of
	 * @return the Color
	 */
	public static Color getBackground(Slider slider) {
		return ScilabSliderBridge.getBackground(slider);
	}

	/**
	 * Set the Foreground color of the Slider
	 * @param slider the Slider we want to set the foreground of
	 * @param color the Color
	 */
	public static void setForeground(Slider slider, Color color) {
		ScilabSliderBridge.setForeground(slider, color);
	}

	/**
	 * Get the Foreground color of the Slider
	 * @param slider the Slider we want to get the foreground of
	 * @return the Color
	 */
	public static Color getForeground(Slider slider) {
		return ScilabSliderBridge.getForeground(slider);
	}

	/**
	 * Set the font of the Slider.
	 * @param slider the Slider we want to set the font of
	 * @param font the font
	 */
	public static void setFont(Slider slider, Font font) {
		ScilabSliderBridge.setFont(slider, font);
	}
	
	/**
	 * Get the font of the Slider.
	 * @param slider the Slider we want to get the font of
	 * @return the font
	 */
	public static Font getFont(Slider slider) {
		return ScilabSliderBridge.getFont(slider);
	}

	/**
	 * Set if the Slider is enabled or not
	 * @param slider the Slider we want to set the status of
	 * @param status true if the Slider is enabled
	 */
	public static void setEnabled(Slider slider, boolean status) {
		ScilabSliderBridge.setEnabled(slider, status);
	}
	
	/**
	 * Gets the enable status of an Slider
	 * @param slider the Slider we want to get the status of
	 * @return the enable status of the Slider (true if the Slider is enabled, false if not)
	 */
	public static boolean isEnabled(Slider slider) {
		return ScilabSliderBridge.isEnabled(slider);
	}
	
	/**
	 * Add a callback to the Slider
	 * @param slider the Slider we want to set the callback of
	 * @param callback the CallBack to set.
	 */
	public static void setCallback(Slider slider, CallBack callback) {
		ScilabSliderBridge.setCallback(slider, callback);
	}

	/**
	 * Set the horizontal alignment for the Slider text
	 * @param slider the Slider we want to set the alignment of
	 * @param alignment the value for the alignment (See ScilabAlignment.java)
	 */
	public static void setHorizontalAlignment(Slider slider, String alignment) {
		ScilabSliderBridge.setHorizontalAlignment(slider, alignment);
	}

	/**
	 * Set the vertical alignment for the Slider text
	 * @param slider the Slider we want to set the alignment of
	 * @param alignment the value for the alignment (See ScilabAlignment.java)
	 */
	public static void setVerticalAlignment(Slider slider, String alignment) {
		ScilabSliderBridge.setVerticalAlignment(slider, alignment);
	}

	/**
	 * Set the minor tick spacing for a Slider
	 * @param slider the Slider we want to set the spacing of
	 * @param space the increment value
	 */
	public static void setMinorTickSpacing(Slider slider, int space) {
		ScilabSliderBridge.setMinorTickSpacing(slider, space);	
	}

	/**
	 * Set the major tick spacing for a Slider
	 * @param slider the Slider we want to set the spacing of
	 * @param space the increment value
	 */
	public static void setMajorTickSpacing(Slider slider, int space) {
		ScilabSliderBridge.setMajorTickSpacing(slider, space);	
	}

	/**
	 * Set the minimum value of a Slider
	 * @param slider the Slider we want to set the minimum value of
	 * @param value the minimum value
	 */
	public static void setMinimumValue(Slider slider, int value) {
		ScilabSliderBridge.setMinimumValue(slider, value);	
	}

	/**
	 * Set the maximum value of a Slider
	 * @param slider the Slider we want to set the maximum value of
	 * @param value the maximum value
	 */
	public static void setMaximumValue(Slider slider, int value) {
		ScilabSliderBridge.setMaximumValue(slider, value);	
	}

	/**
	 * Get the current value of the Slider
	 * @param slider the Slider we want to get the value of
	 * @return the current value of the Slider
	 */
	public static int getValue(Slider slider) {
		return ScilabSliderBridge.getValue(slider);	
	}
	
	/**
	 * Sets the current value of the Slider
	 * @param slider the Slider we want to set the value of
	 * @param value the new value
	 */
	public static void setUserValue(Slider slider, int value) {
		ScilabSliderBridge.setUserValue(slider, value);	
	}

	/**
	 * Set the Relief of the Slider
	 * @param slider the Slider which we want to set the Relief of
	 * @param reliefType the type of the relief to set (See ScilabRelief.java)
	 */
	public static void setRelief(Slider slider, String reliefType) {
		ScilabSliderBridge.setRelief(slider, reliefType);
	}

	/**
	 * Set the slider orientation to vertical
	 * @param slider the slider we want to set the orientation of
	 */
	public static void setVertical(Slider slider) {
		ScilabSliderBridge.setVertical(slider);
	}

	/**
	 * Set the slider orientation to horizontal
	 * @param slider the slider we want to set the orientation of
	 */
	public static void setHorizontal(Slider slider) {
		ScilabSliderBridge.setHorizontal(slider);
	}

	/**
	 * Delete the Slider
	 * @param slider the Slider to be destroyed
	 */
	public static void destroy(Slider slider) {
		ScilabSliderBridge.destroy(slider);
	}

	/**
	 * Give the focus to the Slider
	 * @param slider the Slider
	 */
	public static void requestFocus(Slider slider) {
		ScilabSliderBridge.requestFocus(slider);
	}

	/******************/
	/* ListBox Bridge */
	/******************/

	/**
	 * Creates a new ListBox
	 * @return the created ListBox
	 */
	public static SimpleListBox createListBox() {
		return ScilabListBoxBridge.createListBox();
	}
	
	/**
	 * Sets the Text of a ListBox
	 * @param listBox the ListBox we want to set the Text of
	 * @param newText the Text to set for the ListBox
	 */
	public static void setText(ListBox listBox, String newText) {
		ScilabListBoxBridge.setText(listBox, newText);
	}
	
	/**
	 * Gets the text of an ListBox
	 * @param listBox the ListBox we want to get the text of
	 * @return the text of the ListBox
	 */
	public static String getText(ListBox listBox) {
		return ScilabListBoxBridge.getText(listBox);
	}
	
	/**
	 * Draws a Scilab ListBox
	 * @param listBox the ListBox to draw
	 * @see org.scilab.modules.ihm.UIElement#draw()
	 */
	public static void draw(ListBox listBox) {
		ScilabListBoxBridge.draw(listBox);
	}

	/**
	 * Gets the dimensions (width and height) of a Scilab ListBox
	 * @param listBox the ListBox we want to get the dimensions of
	 * @return the size of the listBox
	 * @see org.scilab.modules.ihm.UIElement#getDims()
	 */
	public static Size getDims(ListBox listBox) {
		return ScilabListBoxBridge.getDims(listBox);
	}

	/**
	 * Gets the position (X-coordinate and Y-coordinate) of a Scilab listBox
	 * @param listBox the listBox we want to get the position of
	 * @return the position of the listBox
	 * @see org.scilab.modules.ihm.UIElement#getPosition()
	 */
	public static Position getPosition(ListBox listBox) {
		return ScilabListBoxBridge.getPosition(listBox);
	}

	/**
	 * Gets the visibility status of a Scilab ListBox
	 * @param listBox the listBox we want to get the visiblity status of
	 * @return the visibility status of the listBox (true if the listBox is visible, false if not)
	 * @see org.scilab.modules.ihm.UIElement#isVisible()
	 */
	public static boolean isVisible(ListBox listBox) {
		return ScilabListBoxBridge.isVisible(listBox);
	}

	/**
	 * Sets the dimensions (width and height) of a Scilab ListBox
	 * @param listBox the listBox we want to set the dimensions of
	 * @param newSize the size we want to set to the listBox
	 * @see org.scilab.modules.ihm.UIElement#setDims(org.scilab.modules.ihm.utils.Size)
	 */
	public static void setDims(ListBox listBox, Size newSize) {
		ScilabListBoxBridge.setDims(listBox, newSize);
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a Scilab listBox
	 * @param listBox the listBox we want to set the position of
	 * @param newPosition the position we want to set to the listBox
	 * @see org.scilab.modules.ihm.UIElement#setPosition(org.scilab.modules.ihm.utils.Position)
	 */
	public static void setPosition(ListBox listBox, Position newPosition) {
		ScilabListBoxBridge.setPosition(listBox, newPosition);
	}

	/**
	 * Sets the visibility status of a Scilab ListBox
	 * @param listBox the listBox we want to set the visiblity status of
	 * @param newVisibleState the visibility status we want to set to the listBox (true to set the listBox visible, false else)
	 * @see org.scilab.modules.ihm.UIElement#setVisible(boolean)
	 */
	public static void setVisible(ListBox listBox, boolean newVisibleState) {
		ScilabListBoxBridge.setVisible(listBox, newVisibleState);
	}
	
	/**
	 * Set the Background color of the ListBox
	 * @param listBox the ListBox we want to set the background of
	 * @param color the Color
	 */
	public static void setBackground(ListBox listBox, Color color) {
		ScilabListBoxBridge.setBackground(listBox, color);
	}

	/**
	 * Get the Background color of the ListBox
	 * @param listBox the ListBox we want to get the background of
	 * @return the Color
	 */
	public static Color getBackground(ListBox listBox) {
		return ScilabListBoxBridge.getBackground(listBox);
	}

	/**
	 * Set the Foreground color of the ListBox
	 * @param listBox the ListBox we want to set the foreground of
	 * @param color the Color
	 */
	public static void setForeground(ListBox listBox, Color color) {
		ScilabListBoxBridge.setForeground(listBox, color);
	}

	/**
	 * Get the Foreground color of the ListBox
	 * @param listBox the ListBox we want to get the foreground of
	 * @return the Color
	 */
	public static Color getForeground(ListBox listBox) {
		return ScilabListBoxBridge.getForeground(listBox);
	}

	/**
	 * Set the font of the ListBox.
	 * @param listBox the ListBox we want to set the font of
	 * @param font the font
	 */
	public static void setFont(ListBox listBox, Font font) {
		ScilabListBoxBridge.setFont(listBox, font);
	}
	
	/**
	 * Get the font of the ListBox.
	 * @param listBox the ListBox we want to get the font of
	 * @return the font
	 */
	public static Font getFont(ListBox listBox) {
		return ScilabListBoxBridge.getFont(listBox);
	}

	/**
	 * Set if the ListBox is enabled or not
	 * @param listBox the ListBox we want to set the status of
	 * @param status true if the ListBox is enabled
	 */
	public static void setEnabled(ListBox listBox, boolean status) {
		ScilabListBoxBridge.setEnabled(listBox, status);
	}
	
	/**
	 * Gets the enable status of a ListBox
	 * @param listBox the ListBox we want to get the status of
	 * @return the enable status of the ListBox (true if the ListBox is enabled, false if not)
	 */
	public static boolean isEnabled(ListBox listBox) {
		return ScilabListBoxBridge.isEnabled(listBox);
	}
	
	/**
	 * Add a callback to the ListBox
	 * @param listBox the ListBox we want to set the callback of
	 * @param callback the Callback to set.
	 */
	public static void setCallback(ListBox listBox, CallBack callback) {
		ScilabListBoxBridge.setCallback(listBox, callback);
	}


	/**
	 * Set the horizontal alignment for the ListBox text
	 * @param listBox the ListBox we want to set the alignment of
	 * @param alignment the value for the alignment (See ScilabAlignment.java)
	 */
	public static void setHorizontalAlignment(ListBox listBox, String alignment) {
		ScilabListBoxBridge.setHorizontalAlignment(listBox, alignment);
	}

	/**
	 * Set the vertical alignment for the ListBox text
	 * @param listBox the ListBox we want to set the alignment of
	 * @param alignment the value for the alignment (See ScilabAlignment.java)
	 */
	public static void setVerticalAlignment(ListBox listBox, String alignment) {
		ScilabListBoxBridge.setVerticalAlignment(listBox, alignment);
	}

	/**
	 * Set if more than one item can be selected in a ListBox
	 * @param listBox the ListBox we want to set the status of
	 * @param status true if multiple selection is enabled
	 */
	public static void setMultipleSelectionEnabled(ListBox listBox, boolean status) {
		ScilabListBoxBridge.setMultipleSelectionEnabled(listBox, status);
	}

	/**
	 * Set the selected indices of the ListBox
	 * @param listBox the ListBox we want to set the selected indices of
	 * @param indices the indices of the items to be selected
	 */
	public static void setSelectedIndices(ListBox listBox, int[] indices) {
		ScilabListBoxBridge.setSelectedIndices(listBox, indices);
	}
	
	/**
	 * Get the selected indices of the ListBox
	 * @param listBox the ListBox we want to get the selected indices of
	 * @return the indices of the items selected
	 */
	public static int[] getSelectedIndices(ListBox listBox) {
		return ScilabListBoxBridge.getSelectedIndices(listBox);
	}

	/**
	 * Get the number of items selected in the ListBox
	 * @param listBox the ListBox we want to get the selection size of
	 * @return the number of items selected
	 */
	public static int getSelectionSize(ListBox listBox) {
		return ScilabListBoxBridge.getSelectionSize(listBox);
	}

	/**
	 * Get the text of all the list items
	 * @param listBox the ListBox we want to get the items text of
	 * @return the text items
	 * @see org.scilab.modules.gui.listbox.ListBox#getAllItemsText()
	 */
	public static String[] getAllItemsText(ListBox listBox) {
		return ScilabListBoxBridge.getAllItemsText(listBox);
	}

	/**
	 * Get the number of items in the list
	 * @param listBox the ListBox we want to get the items number of
	 * @return the number of items
	 * @see org.scilab.modules.gui.listbox.ListBox#getNumberOfItems()
	 */
	public static int getNumberOfItems(ListBox listBox) {
		return ScilabListBoxBridge.getNumberOfItems(listBox);
	}

	/**
	 * Set the text of the list items
	 * @param listBox the ListBox we want to set the items text of
	 * @param text the text of the items
	 * @see org.scilab.modules.gui.listbox.ListBox#setText(java.lang.String[])
	 */
	public static void setText(ListBox listBox, String[] text) {
		ScilabListBoxBridge.setText(listBox, text);
	}

	/**
	 * Set the Relief of the ListBox
	 * @param listBox the ListBox which we want to set the Relief of
	 * @param reliefType the type of the relief to set (See ScilabRelief.java)
	 */
	public static void setRelief(ListBox listBox, String reliefType) {
		ScilabListBoxBridge.setRelief(listBox, reliefType);
	}

	/**
	 * Delete the ListBox
	 * @param listBox the ListBox to be destroyed
	 */
	public static void destroy(ListBox listBox) {
		ScilabListBoxBridge.destroy(listBox);
	}

	/**
	 * Give the focus to the ListBox
	 * @param listBox the ListBox
	 */
	public static void requestFocus(ListBox listBox) {
		ScilabListBoxBridge.requestFocus(listBox);
	}

	/********************/
	/* PopupMenu Bridge */
	/********************/

	/**
	 * Creates a new PopupMenu
	 * @return the created PopupMenu
	 */
	public static SimplePopupMenu createPopupMenu() {
		return ScilabPopupMenuBridge.createPopupMenu();
	}
	
	/**
	 * Sets the Text of a PopupMenu
	 * @param popupMenu the PopupMenu we want to set the Text of
	 * @param newText the Text to set for the PopupMenu
	 */
	public static void setText(PopupMenu popupMenu, String newText) {
		ScilabPopupMenuBridge.setText(popupMenu, newText);
	}
	
	/**
	 * Gets the text of an PopupMenu
	 * @param popupMenu the PopupMenu we want to get the text of
	 * @return the text of the PopupMenu
	 */
	public static String getText(PopupMenu popupMenu) {
		return ScilabPopupMenuBridge.getText(popupMenu);
	}
	
	/**
	 * Draws a Scilab PopupMenu
	 * @param popupMenu the PopupMenu to draw
	 * @see org.scilab.modules.ihm.UIElement#draw()
	 */
	public static void draw(PopupMenu popupMenu) {
		ScilabPopupMenuBridge.draw(popupMenu);
	}

	/**
	 * Gets the dimensions (width and height) of a Scilab PopupMenu
	 * @param popupMenu the PopupMenu we want to get the dimensions of
	 * @return the size of the popupMenu
	 * @see org.scilab.modules.ihm.UIElement#getDims()
	 */
	public static Size getDims(PopupMenu popupMenu) {
		return ScilabPopupMenuBridge.getDims(popupMenu);
	}

	/**
	 * Gets the position (X-coordinate and Y-coordinate) of a Scilab popupMenu
	 * @param popupMenu the popupMenu we want to get the position of
	 * @return the position of the popupMenu
	 * @see org.scilab.modules.ihm.UIElement#getPosition()
	 */
	public static Position getPosition(PopupMenu popupMenu) {
		return ScilabPopupMenuBridge.getPosition(popupMenu);
	}

	/**
	 * Gets the visibility status of a Scilab PopupMenu
	 * @param popupMenu the popupMenu we want to get the visiblity status of
	 * @return the visibility status of the popupMenu (true if the popupMenu is visible, false if not)
	 * @see org.scilab.modules.ihm.UIElement#isVisible()
	 */
	public static boolean isVisible(PopupMenu popupMenu) {
		return ScilabPopupMenuBridge.isVisible(popupMenu);
	}

	/**
	 * Sets the dimensions (width and height) of a Scilab PopupMenu
	 * @param popupMenu the popupMenu we want to set the dimensions of
	 * @param newSize the size we want to set to the popupMenu
	 * @see org.scilab.modules.ihm.UIElement#setDims(org.scilab.modules.ihm.utils.Size)
	 */
	public static void setDims(PopupMenu popupMenu, Size newSize) {
		ScilabPopupMenuBridge.setDims(popupMenu, newSize);
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a Scilab popupMenu
	 * @param popupMenu the popupMenu we want to set the position of
	 * @param newPosition the position we want to set to the popupMenu
	 * @see org.scilab.modules.ihm.UIElement#setPosition(org.scilab.modules.ihm.utils.Position)
	 */
	public static void setPosition(PopupMenu popupMenu, Position newPosition) {
		ScilabPopupMenuBridge.setPosition(popupMenu, newPosition);
	}

	/**
	 * Sets the visibility status of a Scilab PopupMenu
	 * @param popupMenu the popupMenu we want to set the visiblity status of
	 * @param newVisibleState the visibility status we want to set to the popupMenu (true to set the popupMenu visible, false else)
	 * @see org.scilab.modules.ihm.UIElement#setVisible(boolean)
	 */
	public static void setVisible(PopupMenu popupMenu, boolean newVisibleState) {
		ScilabPopupMenuBridge.setVisible(popupMenu, newVisibleState);
	}
	
	/**
	 * Set the Background color of the PopupMenu
	 * @param popupMenu the PopupMenu we want to set the background of
	 * @param color the Color
	 */
	public static void setBackground(PopupMenu popupMenu, Color color) {
		ScilabPopupMenuBridge.setBackground(popupMenu, color);
	}

	/**
	 * Get the Background color of the PopupMenu
	 * @param popupMenu the PopupMenu we want to get the background of
	 * @return the Color
	 */
	public static Color getBackground(PopupMenu popupMenu) {
		return ScilabPopupMenuBridge.getBackground(popupMenu);
	}

	/**
	 * Set the Foreground color of the PopupMenu
	 * @param popupMenu the PopupMenu we want to set the foreground of
	 * @param color the Color
	 */
	public static void setForeground(PopupMenu popupMenu, Color color) {
		ScilabPopupMenuBridge.setForeground(popupMenu, color);
	}

	/**
	 * Get the Foreground color of the PopupMenu
	 * @param popupMenu the PopupMenu we want to get the foreground of
	 * @return the Color
	 */
	public static Color getForeground(PopupMenu popupMenu) {
		return ScilabPopupMenuBridge.getForeground(popupMenu);
	}

	/**
	 * Set the font of the PopupMenu.
	 * @param popupMenu the PopupMenu we want to set the font of
	 * @param font the font
	 */
	public static void setFont(PopupMenu popupMenu, Font font) {
		ScilabPopupMenuBridge.setFont(popupMenu, font);
	}
	
	/**
	 * Get the font of the PopupMenu.
	 * @param popupMenu the PopupMenu we want to get the font of
	 * @return the font
	 */
	public static Font getFont(PopupMenu popupMenu) {
		return ScilabPopupMenuBridge.getFont(popupMenu);
	}

	/**
	 * Set if the PopupMenu is enabled or not
	 * @param popupMenu the PopupMenu we want to set the status of
	 * @param status true if the PopupMenu is enabled
	 */
	public static void setEnabled(PopupMenu popupMenu, boolean status) {
		ScilabPopupMenuBridge.setEnabled(popupMenu, status);
	}
	
	/**
	 * Gets the enable status of a PopupMenu
	 * @param popupMenu the PopupMenu we want to get the status of
	 * @return the enable status of the PopupMenu (true if the PopupMenu is enabled, false if not)
	 */
	public static boolean isEnabled(PopupMenu popupMenu) {
		return ScilabPopupMenuBridge.isEnabled(popupMenu);
	}
	
	/**
	 * Add a callback to the PopupMenu
	 * @param popupMenu the PopupMenu we want to set the callback of
	 * @param callback the CallBack to set.
	 */
	public static void setCallback(PopupMenu popupMenu, CallBack callback) {
		ScilabPopupMenuBridge.setCallback(popupMenu, callback);
	}

	/**
	 * Set the horizontal alignment for the PopupMenu text
	 * @param popupMenu the PopupMenu we want to set the alignment of
	 * @param alignment the value for the alignment (See ScilabAlignment.java)
	 */
	public static void setHorizontalAlignment(PopupMenu popupMenu, String alignment) {
		ScilabPopupMenuBridge.setHorizontalAlignment(popupMenu, alignment);
	}

	/**
	 * Set the vertical alignment for the PopupMenu text
	 * @param popupMenu the PopupMenu we want to set the alignment of
	 * @param alignment the value for the alignment (See ScilabAlignment.java)
	 */
	public static void setVerticalAlignment(PopupMenu popupMenu, String alignment) {
		ScilabPopupMenuBridge.setVerticalAlignment(popupMenu, alignment);
	}

	/**
	 * Set the selected index of the ListBox
	 * @param popupMenu the PopupMenu we want to set the selected index of
	 * @param index the index of the item to be selected
	 */
	public static void setUserSelectedIndex(PopupMenu popupMenu, int index) {
		ScilabPopupMenuBridge.setUserSelectedIndex(popupMenu, index);
	}
	
	/**
	 * Get the selected index of the ListBox
	 * @param popupMenu the PopupMenu we want to get the selected index of
	 * @return the index of the item selected
	 */
	public static int getUserSelectedIndex(PopupMenu popupMenu) {
		return ScilabPopupMenuBridge.getUserSelectedIndex(popupMenu);
	}
	
	/**
	 * Get the text of all the PopupMenu items
	 * @param popupMenu the PopupMenu we want to get the items of
	 * @return the text items
	 */
	public static String[] getAllItemsText(PopupMenu popupMenu) {
		return ScilabPopupMenuBridge.getAllItemsText(popupMenu);
	}

	/**
	 * Get the number of items in the PopupMenu
	 * @param popupMenu the PopupMenu we want to get the items number of
	 * @return the number of items
	 */
	public static int getNumberOfItems(PopupMenu popupMenu) {
		return ScilabPopupMenuBridge.getNumberOfItems(popupMenu);
	}

	/**
	 * Set the text of the PopupMenu items
	 * @param popupMenu the PopupMenu we want to set the text items of
	 * @param text the text of the items
	 */
	public static void setText(PopupMenu popupMenu, String[] text) {
		ScilabPopupMenuBridge.setText(popupMenu, text);
	}

	/**
	 * Set the Relief of the PopupMenu
	 * @param popupMenu the PopupMenu which we want to set the Relief of
	 * @param reliefType the type of the relief to set (See ScilabRelief.java)
	 */
	public static void setRelief(PopupMenu popupMenu, String reliefType) {
		ScilabPopupMenuBridge.setRelief(popupMenu, reliefType);
	}

	/**
	 * Delete the PopupMenu
	 * @param popupMenu the PopupMenu to be destroyed
	 */
	public static void destroy(PopupMenu popupMenu) {
		ScilabPopupMenuBridge.destroy(popupMenu);
	}

	/**
	 * Give the focus to the PopupMenu
	 * @param popupMenu the PopupMenu
	 */
	public static void requestFocus(PopupMenu popupMenu) {
		ScilabPopupMenuBridge.requestFocus(popupMenu);
	}

	/*********************/
	/* FlowLayout Bridge */
	/*********************/
	/**
	 * Creates a new FlowLayout
	 * @return the created FlowLayout
	 */
	public static FlowLayout createFlowLayout() {
		return ScilabFlowLayoutBridge.createFlowLayout();
	}
	
	/***********************/
	/* BorderLayout Bridge */
	/***********************/
	/**
	 * Creates a new BorderLayout
	 * @return the created BorderLayout
	 */
	public static BorderLayout createBorderLayout() {
		return ScilabBorderLayoutBridge.createBorderLayout();
	}
	
	/***********************/
	/* FileChooser Bridge */
	/***********************/

	/**
	 * Create a new FileChooser
	 * @return the created FileChooser
	 */
	public static SimpleFileChooser createFileChooser() {
		return ScilabFileChooserBridge.createFileChooser();
	}
	
	/**
	 * Create a new ExportFileChooser
	 * @param figureID exported figure id
	 * @return the created ExportFileChooser
	 */
	public static SimpleFileChooser createExportFileChooser(int figureID) {
		return ScilabFileChooserBridge.createExportFileChooser(figureID);
	}
	
	/**
	 * Set the title of the file chooser
	 * @param fileChooser the file chooser we want to set the title of
	 * @param title the title to set
	 */
	public static void setTitle(FileChooser fileChooser, String title) {
		ScilabFileChooserBridge.setTitle(fileChooser, title);
	}
	
	/**
	 * Set the initial directory used for file search
	 * @param fileChooser the file chooser we want to set the initial directory of
	 * @param path the default path
	 */
	public static void setInitialDirectory(FileChooser fileChooser, String path) {
		ScilabFileChooserBridge.setInitialDirectory(fileChooser, path);
	}

	/**
	 * Display this chooser and wait for user selection 
	 * @param fileChooser the file chooser we want to display
	 */
	public static void displayAndWait(FileChooser fileChooser) {
		ScilabFileChooserBridge.displayAndWait(fileChooser);
	}
	
	/**
	 * Get the number of files selected
	 * @param fileChooser the file chooser we want to get the selection size of
	 * @return the number of files selected
	 */
	public static int getSelectionSize(FileChooser fileChooser) {
		return ScilabFileChooserBridge.getSelectionSize(fileChooser);
	}
	
	/**
	 * Get the names of selected files
	 * @param fileChooser the file chooser we want to get the selection of 
	 * @return the names of selected files
	 */
	public static String[] getSelection(FileChooser fileChooser) {
		return ScilabFileChooserBridge.getSelection(fileChooser);
	}
	
	/**
	 * Get the names of selected files
	 * @param fileChooser the file chooser we want to get the selection of 
	 * @return the names of selected files
	 */
	public static String[] getSelectionFileNames(FileChooser fileChooser) {
		return ScilabFileChooserBridge.getSelectionFileNames(fileChooser);
	}
	
	/**
	 * Set the flag indicating that we want only select directories
	 * @param fileChooser the file chooser we want to set the flag of 
	 */
	public static void setDirectorySelectionOnly(FileChooser fileChooser) {
		ScilabFileChooserBridge.setDirectorySelectionOnly(fileChooser);
	}
	
	/**
	 * We customize the file chooser for the graphic export 
	 * by adding format selection
	 */
	public static void exportCustomFileChooser() {
		ScilabFileChooserBridge.createFileChooser();
	}	
	
	/**
	 * Enable the multiple selection
	 * @param fileChooser the file chooser we want to get the  muliple selection 
	 * @param multipleSelection boolean for the multiple selection 
	 */
	public static void setMultipleSelection(FileChooser fileChooser, boolean multipleSelection) {
		ScilabFileChooserBridge.setMultipleSelection(fileChooser, multipleSelection);
	}

	/**
	 * Get the path name of selected files
	 * @param fileChooser the file chooser we want to get the path of selectd files 
	 * @return the path of selected files
	 */
	public static String getSelectionPathName(FileChooser fileChooser) {
		return ScilabFileChooserBridge.getSelectionPathName(fileChooser);
	}
	
	/**
	 * Get the filter index
	 * @param fileChooser the file chooser we want to get the filter indexs
	 * @return the filter index
	 */
	public static int getFilterIndex(FileChooser fileChooser) {
		return ScilabFileChooserBridge.getFilterIndex(fileChooser);
	}

	/**
	 * Set the dialog type (save or open a file ?)
	 * @param fileChooser the file chooser we want to get the filter indexs
	 * @param dialogType the dialog type
	 */
	public static void setUiDialogType(FileChooser fileChooser, int dialogType) {
		ScilabFileChooserBridge.setUiDialogType(fileChooser, dialogType);
	}

	/*********************/
	/* MessageBox Bridge */
	/*********************/

	/**
	 * Create a new MessageBox
	 * @return the created MessageBox
	 */
	public static SimpleMessageBox createMessageBox() {
		return ScilabMessageBoxBridge.createMessageBox();
	}
	
	/**
	 * Set the title of the MessageBox
	 * @param messageBox the MessageBox we want to set the title of
	 * @param title the title to set
	 */
	public static void setTitle(MessageBox messageBox, String title) {
		ScilabMessageBoxBridge.setTitle(messageBox, title);
	}
	
	/**
	 * Set the message of the MessageBox
	 * @param messageBox the MessageBox we want to set the mask of
	 * @param message the message to set
	 */
	public static void setMessage(MessageBox messageBox, String message) {
		ScilabMessageBoxBridge.setMessage(messageBox, message);
	}

	/**
	 * Set the message of the MessageBox (multi-line)
	 * @param messageBox the MessageBox we want to set the mask of
	 * @param message the message to set
	 */
	public static void setMessage(MessageBox messageBox, String[] message) {
		ScilabMessageBoxBridge.setMessage(messageBox, message);
	}

	/**
	 * Display this MessageBox and wait for user selection 
	 * @param messageBox the MessageBox we want to display
	 */
	public static void displayAndWait(MessageBox messageBox) {
		ScilabMessageBoxBridge.displayAndWait(messageBox);
	}
	
	/**
	 * Get the index of the button clicked
	 * @param messageBox the MessageBox we want to get the answer of
	 * @return the index of the button clicked
	 */
	public static int getSelectedButton(MessageBox messageBox) {
		return ScilabMessageBoxBridge.getSelectedButton(messageBox);
	}
	
	/**
	 * Set the indices of the default selected buttons (x_choices)
	 * @param messageBox the MessageBox we want to set the defaults of
	 * @param indices the indices of the default selected buttons
	 */
	public static void setDefaultSelectedButtons(MessageBox messageBox, int[] indices) {
		ScilabMessageBoxBridge.setDefaultSelectedButtons(messageBox, indices);
	}
	
	/**
	 * Get the indices of the user selected buttons (x_choices)
	 * @param messageBox the MessageBox we want to set the defaults of
	 * @return the indices of the default selected buttons
	 */
	public static int[] getUserSelectedButtons(MessageBox messageBox) {
		return ScilabMessageBoxBridge.getUserSelectedButtons(messageBox);
	}
	
	/**
	 * Set the labels of the buttons in the MessageBox
	 * @param messageBox the MessageBox we want to set the buttons labels of
	 * @param labels the labels of the buttons
	 */
	public static void setButtonsLabels(MessageBox messageBox, String[] labels) {
		ScilabMessageBoxBridge.setButtonsLabels(messageBox, labels);
	}

	/**
	 * Set the initial values of the editable zone in the MessageBox
	 * @param messageBox the MessageBox we want to set the initial value of
	 * @param value the initial values
	 */
	public static void setInitialValue(MessageBox messageBox, String[] value) {
		ScilabMessageBoxBridge.setInitialValue(messageBox, value);
	}

	/**
	 * Get the value of the editable zone in the MessageBox
	 * @param messageBox the MessageBox we want to get the value of
	 * @return the value
	 */
	public static String[] getValue(MessageBox messageBox) {
		return ScilabMessageBoxBridge.getValue(messageBox);
	}

	/**
	 * Get the size of the value of the editable zone in the MessageBox
	 * @param messageBox the MessageBox we want to get the value size of
	 * @return the value size
	 */
	public static int getValueSize(MessageBox messageBox) {
		return ScilabMessageBoxBridge.getValueSize(messageBox);
	}

	/**
	 * Set the items of the listbox in the MessageBox
	 * @param messageBox the MessageBox we want to set the items of
	 * @param items the items to set
	 */
	public static void setListBoxItems(MessageBox messageBox, String[] items) {
		ScilabMessageBoxBridge.setListBoxItems(messageBox, items);
	}
	
	/**
	 * Get the index of the selected item in the listbox in the MessageBox
	 * @param messageBox the MessageBox we want to get the selected item of
	 * @return the index
	 */
	public static int getSelectedItem(MessageBox messageBox) {
		return ScilabMessageBoxBridge.getSelectedItem(messageBox);
	}

	/**
	 * Set the name of the lines labels in the editable zone in the MessageBox
	 * @param messageBox the MessageBox we want to set the labels
	 * @param labels the labels
	 */
	public static void setLineLabels(MessageBox messageBox, String[] labels) {
		ScilabMessageBoxBridge.setLineLabels(messageBox, labels);
	}
	
	/**
	 * Set the name of the columns labels in the editable zone in the MessageBox
	 * @param messageBox the MessageBox we want to set the labels
	 * @param labels the labels
	 */
	public static void setColumnLabels(MessageBox messageBox, String[] labels) {
		ScilabMessageBoxBridge.setColumnLabels(messageBox, labels);
	}
	
	/**
	 * Set the default values of a multi-value editable zone in the MessageBox
	 * @param messageBox the MessageBox we want to set the default values
	 * @param values the values
	 */
	public static void setDefaultInput(MessageBox messageBox, String[] values) {
		ScilabMessageBoxBridge.setDefaultInput(messageBox, values);
	}

	/**
	 * Set a MessageBox modal or not
	 * @param messageBox the MessageBox we want to set the modality of
	 * @param status true to set the MessageBox modal and false else
	 */
	public static void setModal(MessageBox messageBox, boolean status) {
		ScilabMessageBoxBridge.setModal(messageBox, status);
	}

	/**
	 * Set the MessageBox icon
	 * @param messageBox the MessageBox we want to set the icon of
	 * @param name the name of the icon
	 */
	public static void setIcon(MessageBox messageBox, String name) {
		ScilabMessageBoxBridge.setIcon(messageBox, name);
	}

	/******************/
	/* WaitBar Bridge */
	/******************/

	/**
	 * Create a new WaitBar
	 * @return the created WaitBar
	 */
	public static SimpleWaitBar createWaitBar() {
		return ScilabWaitBarBridge.createWaitBar();
	}

	/**
	 * Set the title of the WaitBar
	 * @param waitBar the WaitBar
	 * @param title the title to set
	 */
	public static void setTitle(WaitBar waitBar, String title) {
		ScilabWaitBarBridge.setTitle(waitBar, title);
	}
	
	/**
	 * Set the message of the WaitBar
	 * @param waitBar the WaitBar
	 * @param message the message to set
	 */
	public static void setMessage(WaitBar waitBar, String[] message) {
		ScilabWaitBarBridge.setMessage(waitBar, message);
	}
	
	/**
	 * Set the current value of the WaitBar
	 * @param waitBar the WaitBar
	 * @param value the value to set
	 */
	public static void setValue(WaitBar waitBar, int value) {
		ScilabWaitBarBridge.setValue(waitBar, value);
	};
	
	/**
	 * Close the WaitBar
	 * @param waitBar the WaitBar
	 */
	public static void close(WaitBar waitBar) {
		ScilabWaitBarBridge.close(waitBar);
	}
	
	/**
	 * Indicates if the total execution time is known
	 * @param waitBar the WaitBar
	 * @param status true if the total progress time in unknown
	 */
	public static void setIndeterminateMode(WaitBar waitBar, boolean status) {
		ScilabWaitBarBridge.setIndeterminateMode(waitBar, status);
	}

	/***********************/
	/* Help Browser Bridge */
	/***********************/

	/**
	 * Create a new Help Browser
	 * @param helps help chapters and directories
	 * @param language Scilab current language
	 * @return the created Help Browser
	 */
	public static SimpleHelpBrowser createHelpBrowser(String[] helps, String language) {
		return ScilabHelpBrowserBridge.createHelpBrowser(helps, language);
	}

	/**
	 * Display the Help Browser
	 * @param browser the Help Browser
	 */
	public static void display(HelpBrowser browser) {
		ScilabHelpBrowserBridge.display(browser);
	}
	
	/**
	 * Display the matching items for a specified keyword
	 * @param browser the Help Browser
	 * @param keyword the keyword
	 */
	public static void searchKeywork(HelpBrowser browser, String keyword) {
		ScilabHelpBrowserBridge.searchKeywork(browser, keyword);
	}
	
	/**
	 * Display the result of a full-text search for a specified word
	 * @param browser the Help Browser
	 * @param keyword the keyword
	 */
	public static void fullTextSearch(HelpBrowser browser, String keyword) {
		ScilabHelpBrowserBridge.fullTextSearch(browser, keyword);
	}

	/**
	 * Close the HelpBrowser
	 * @param browser the Help Browser
	 */
	public static void close(HelpBrowser browser) {
		ScilabHelpBrowserBridge.close(browser);
	}

	/***********************/
	/* FontChooser Bridge */
	/***********************/

	/**
	 * Create a new FontChooser
	 * @param font the default font
	 * @return the created FontChooser
	 */
	public static SimpleFontChooser createFontChooser(Font font) {
		return ScilabFontChooserBridge.createFontChooser(font);
	}
	
	/** 
	 * Retrieve the selected font, or null 
	 * @param fontChooser the FontChooser
	 * @return the selected font
	 */
	public static Font getSelectedFont(FontChooser fontChooser) {
		return ScilabFontChooserBridge.getSelectedFont(fontChooser);
	}

	/** 
	 * Set the default font name 
	 * @param fontChooser the FontChooser
	 * @param fontName the default font name
	 */
	public static void setDefaultFontName(FontChooser fontChooser, String fontName) {
		ScilabFontChooserBridge.setDefaultFontName(fontChooser, fontName);
	}

	/** 
	 * Set the default font size 
	 * @param fontChooser the FontChooser
	 * @param fontSize the default font size
	 */
	public static void setDefaultFontSize(FontChooser fontChooser, int fontSize) {
		ScilabFontChooserBridge.setDefaultFontSize(fontChooser, fontSize);
	}

	/** 
	 * Set the default font bold attribute 
	 * @param fontChooser the FontChooser
	 * @param bold the default font bold attribute
	 */
	public static void setDefaultBold(FontChooser fontChooser, boolean bold) {
		ScilabFontChooserBridge.setDefaultBold(fontChooser, bold);
	}

	/** 
	 * Set the default font italic attribute 
	 * @param fontChooser the FontChooser
	 * @param italic the default font italic attribute
	 */
	public static void setDefaultItalic(FontChooser fontChooser, boolean italic) {
		ScilabFontChooserBridge.setDefaultItalic(fontChooser, italic);
	}
	
	/**
	 * Display the font chooser and wait for a user input
	 * @param fontChooser the FontChooser
	 */
	public static void displayAndWait(FontChooser fontChooser) {
		ScilabFontChooserBridge.displayAndWait(fontChooser);
	}

	/***********************/
	/* ColorChooser Bridge */
	/***********************/

	/**
	 * Create a new ColorChooser
	 * @param color the default color
	 * @return the created ColorChooser
	 */
	public static SimpleColorChooser createColorChooser(Color color) {
		return ScilabColorChooserBridge.createColorChooser(color);
	}
	
	/** 
	 * Set the title of the ColorChooser 
	 * @param colorChooser the ColorChooser
	 * @param title the title to set
	 */
	public static void setTitle(ColorChooser colorChooser, String title) {
		ScilabColorChooserBridge.setTitle(colorChooser, title);
	}

	/** 
	 * Retrieve the selected color, or null 
	 * @param colorChooser the ColorChooser
	 * @return the selected color
	 */
	public static Color getSelectedColor(ColorChooser colorChooser) {
		return ScilabColorChooserBridge.getSelectedColor(colorChooser);
	}
	
	/** 
	 * Set the default color 
	 * @param colorChooser the ColorChooser
	 * @param color the default color
	 */
	public static void setDefaultColor(ColorChooser colorChooser, Color color) {
		ScilabColorChooserBridge.setDefaultColor(colorChooser, color);
	}
	
	/**
	 * Display the font chooser and wait for a user input
	 * @param colorChooser the ColorChooser
	 */
	public static void displayAndWait(ColorChooser colorChooser) {
		ScilabColorChooserBridge.displayAndWait(colorChooser);
	}
	
	
	/******************/
	/* Tree Bridge    */
	/******************/

	public static void  showTree(Tree tree) {
		ScilabTreeBridge.showTree(tree);
	}

	public static SimpleTree createTree(Tree scilabTree) {
	    return ScilabTreeBridge.createTree(scilabTree);
	}
}

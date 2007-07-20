
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.bridge;

import javax.media.opengl.GL;

import org.scilab.modules.gui.canvas.Canvas;
import org.scilab.modules.gui.canvas.ScilabCanvasBridge;
import org.scilab.modules.gui.canvas.SimpleCanvas;
import org.scilab.modules.gui.console.Console;
import org.scilab.modules.gui.console.ScilabConsoleBridge;
import org.scilab.modules.gui.console.SimpleConsole;
import org.scilab.modules.gui.editbox.EditBox;
import org.scilab.modules.gui.editbox.ScilabEditBoxBridge;
import org.scilab.modules.gui.editbox.SimpleEditBox;
import org.scilab.modules.gui.frame.Frame;
import org.scilab.modules.gui.frame.ScilabFrameBridge;
import org.scilab.modules.gui.frame.SimpleFrame;
import org.scilab.modules.gui.layout.BorderLayout;
import org.scilab.modules.gui.layout.FlowLayout;
import org.scilab.modules.gui.layout.ScilabBorderLayoutBridge;
import org.scilab.modules.gui.layout.ScilabFlowLayoutBridge;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menu.ScilabMenuBridge;
import org.scilab.modules.gui.menu.SimpleMenu;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.menubar.ScilabMenuBarBridge;
import org.scilab.modules.gui.menubar.SimpleMenuBar;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.menuitem.ScilabMenuItemBridge;
import org.scilab.modules.gui.menuitem.SimpleMenuItem;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.gui.pushbutton.ScilabPushButtonBridge;
import org.scilab.modules.gui.pushbutton.SimplePushButton;
import org.scilab.modules.gui.separator.ScilabSeparatorBridge;
import org.scilab.modules.gui.separator.Separator;
import org.scilab.modules.gui.tab.ScilabTabBridge;
import org.scilab.modules.gui.tab.SimpleTab;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.textbox.ScilabTextBoxBridge;
import org.scilab.modules.gui.textbox.SimpleTextBox;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ScilabToolBarBridge;
import org.scilab.modules.gui.toolbar.SimpleToolBar;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.gui.window.ScilabWindowBridge;
import org.scilab.modules.gui.window.SimpleWindow;
import org.scilab.modules.gui.window.Window;

/**
 * Main bridge for Scilab Java GUIs
 * @author Vincent Couvert
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
	 * Sets a MeunBar to a window
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

	
	/****************/
	/* Frame Bridge */
	/****************/

	/**
	 * Creates a new frame
	 * @return the created frame
	 */
	public static SimpleFrame createFrame() {
		return ScilabFrameBridge.createFrame();
	}
	/**
	 * Draws a frame on screen
	 * @param frame the frame to be drawn
	 */
	public static void draw(Frame frame) {
		ScilabFrameBridge.draw(frame);
	}

	/**
	 * Gets the dimensions (width and height) of a frame
	 * @param frame the frame we want to get the dimensions of
	 * @return the dimensions of the frame
	 */
	public static Size getDims(Frame frame) {
		return ScilabFrameBridge.getDims(frame);
	}

	/**
	 * Gets the position (X-coordinate and Y-coordinate) of a frame
	 * @param frame the frame we want to get the position of
	 * @return position of the frame
	 */
	public static Position getPosition(Frame frame) {
		return ScilabFrameBridge.getPosition(frame);
	}

	/**
	 * Gets the visibility status of a frame
	 * @param frame the frame we want to Gets the status of
	 * @return visibility status (true if the frame is visible, false if not)
	 */
	public static boolean isVisible(Frame frame) {
		return ScilabFrameBridge.isVisible(frame);
	}

	/**
	 * Sets the dimensions (width and height) of a frame
	 * @param frame the frame we want to set the dimensions of
	 * @param newFrameSize new dimensions of the frame
	 */
	public static void setDims(Frame frame, Size newFrameSize) {
		ScilabFrameBridge.setDims(frame, newFrameSize);
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a frame
	 * @param frame the frame we want to set the position of
	 * @param newFramePosition new position of the frame
	 */
	public static void setPosition(Frame frame, Position newFramePosition) {
		ScilabFrameBridge.setPosition(frame, newFramePosition);
	}

	/**
	 * Sets the visibility status of a frame
	 * @param frame the frame we want to set the visibility status of
	 * @param newVisibleState new visibility status for the window
	 * 			(true set the window visible, false to set the window invisible)
	 */
	public static void setVisible(Frame frame, boolean newVisibleState) {
		ScilabFrameBridge.setVisible(frame, newVisibleState);
	}

	/**
	 * Add a member (dockable element) to a frame and returns the index of this member
	 * @param frame the frame where we want to add the member
	 * @param member the member to add
 	 * @return index of member
 	 */
	public static int addMember(Frame frame, Frame member) {
		return ScilabFrameBridge.addMember(frame, member);
	}

	/**
	 * Add a member (dockable element) to a frame and returns the index of this member
	 * @param frame the frame where we want to add the member
	 * @param member the member to add
 	 * @return index of member
 	 */
	public static int addMember(Frame frame, Console member) {
		return ScilabFrameBridge.addMember(frame, member);
	}

	/**
	 * Add a member (dockable element) to a frame and returns the index of this member
	 * @param frame the frame where we want to add the member
	 * @param member the member to add
 	 * @return index of member
 	 */
	public static int addMember(Frame frame, Canvas member) {
		return ScilabFrameBridge.addMember(frame, member);
	}
	
	/**
	 * Add a member (dockable element) to a frame and returns the index of this member
	 * @param frame the frame where we want to add the member
	 * @param member the member to add
 	 * @return index of member
 	 */
	public static int addMember(Frame frame, PushButton member) {
		return ScilabFrameBridge.addMember(frame, member);
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
	 * Add a member (dockable element) to a tab and returns the index of this member
	 * @param tab the tab where we want to add the member
	 * @param member the member to add
 	 * @return index of member
 	 */
	public static int addMember(Tab tab, Console member) {
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
	 * @see org.scilab.modules.console.Console#display()
	 */
	public static void display(Console console, String dataToDisplay) {
		ScilabConsoleBridge.display(console, dataToDisplay);
	}

	/**
	 * Reads input data in the console
	 * @param console the console to read the data from
	 * @return the data entered by the user
	 * @see org.scilab.modules.console.Console#readLine()
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
	 * Reads one user input char
	 * @param console the console used to get the char
	 * @return the data entered by the user
	 * @see fr.scilab.console.Console#getCharWithoutOutput()
	 */
	public static int getCharWithoutOutput(Console console) {
		return ScilabConsoleBridge.getCharWithoutOutput(console);
	}

	/*****************/
	/* Canvas Bridge */
	/*****************/

	/**
	 * Creates a Scilab Canvas
	 * @param figureIndex index of the displayed figure
	 * @return the created canvas
	 */
	public static SimpleCanvas createCanvas(int figureIndex) {
		return ScilabCanvasBridge.createCanvas(figureIndex);
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
	 * @param canvas the canvas we want to repaint
	 */
	public static void repaint(Canvas canvas) {
		ScilabCanvasBridge.repaint(canvas);
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
	public static void setMnemonic(MenuItem menuItem, int mnemonic) {
		ScilabMenuItemBridge.setMnemonic(menuItem, mnemonic);
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
	 * Append a MenuItem to a Menu
	 * @param menu the Menu which we want to add the MenuItem to
	 * @param newMenuItem the MenuItem to add to the Menu
	 */
	public static void add(Menu menu, MenuItem newMenuItem) {
		ScilabMenuBridge.add(menu, newMenuItem);
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
	
	/*****************/
	/* Separator Bridge */
	/*****************/

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
	 * Sets the icon of a PushButton
	 * @param pushButton the PushButton we want to set the icon of
	 * @param filename the path to the icon image to set to the PushButton
	 */
	public static void setIcon(PushButton pushButton, String filename) {
		ScilabPushButtonBridge.setIcon(pushButton, filename);
	}
	
	/*****************/
	/* TextBox Bridge */
	/*****************/

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
	
	/*****************/
	/* EditBox Bridge */
	/*****************/

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
	
	/*****************/
	/* FlowLayout Bridge */
	/*****************/
	/**
	 * Creates a new FlowLayout
	 * @return the created FlowLayout
	 */
	public static FlowLayout createFlowLayout() {
		return ScilabFlowLayoutBridge.createFlowLayout();
	}
	
	/*****************/
	/* BorderLayout Bridge */
	/*****************/
	/**
	 * Creates a new BorderLayout
	 * @return the created BorderLayout
	 */
	public static BorderLayout createBorderLayout() {
		return ScilabBorderLayoutBridge.createBorderLayout();
	}
}

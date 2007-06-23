
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.bridge;

import org.scilab.modules.gui.console.Console;
import org.scilab.modules.gui.console.ScilabConsoleBridge;
import org.scilab.modules.gui.canvas.Canvas;
import org.scilab.modules.gui.canvas.ScilabCanvasBridge;
import org.scilab.modules.gui.frame.Frame;
import org.scilab.modules.gui.frame.ScilabFrameBridge;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.gui.pushbutton.ScilabPushButtonBridge;
import org.scilab.modules.gui.separator.ScilabSeparatorBridge;
import org.scilab.modules.gui.separator.Separator;
import org.scilab.modules.gui.tab.ScilabTabBridge;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.gui.window.ScilabWindowBridge;
import org.scilab.modules.gui.window.Window;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menubar.ScilabMenuBarBridge;
import org.scilab.modules.gui.menuitem.ScilabMenuItemBridge;
import org.scilab.modules.gui.menu.ScilabMenuBridge;

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
	public static Window createWindow() {
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
	public static void setMenuBar(Window window, MenuBar newMenuBar) {
		ScilabWindowBridge.setMenuBar(window, newMenuBar);
	}
	
	/****************/
	/* Frame Bridge */
	/****************/

	/**
	 * Creates a new frame
	 * @return the created frame
	 */
	public static Frame createFrame() {
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


	/**************/
	/* Tab Bridge */
	/**************/

	/**
	 * Creates a Scilab tab
	 * @param name the name of the tab (used to identify it)
	 * @return the created tab
	 */
	public static Tab createTab(String name) {
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
	public static Console createConsole() {
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

	/*****************/
	/* Canvas Bridge */
	/*****************/

	/**
	 * Creates a Scilab Canvas
	 * @return the created canvas
	 */
	public static Canvas createCanvas() {
		return ScilabCanvasBridge.createCanvas();
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
	
	/*****************/
	/* MenuBar Bridge */
	/*****************/

	/**
	 * Creates a new MenuBar
	 * @return the created MenuBar
	 */
	public static MenuBar createMenuBar() {
		return ScilabMenuBarBridge.createMenuBar();
	}
	
	/**
	 * Append a Menu to a MenuBar
	 * @param menuBar the MenuBar which we want to add the newMenu to
	 * @param newMenu the Menu to add to the MenuBar
	 * @return the added Menu
	 */
	public static Menu add(MenuBar menuBar, Menu newMenu) {
		return ScilabMenuBarBridge.add(menuBar, newMenu);
	}
	
	/*****************/
	/* MenuItem Bridge */
	/*****************/

	/**
	 * Creates a new MenuItem
	 * @return the created MenuItem
	 */
	public static MenuItem createMenuItem() {
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
	public static Menu createMenu() {
		return ScilabMenuBridge.createMenu();
	}
	
	/**
	 * Sets the dimensions (width and height) of a swing Scilab Menu
	 * @param menu the Menu which we want to set the newSize to
	 * @param newSize the dimensions to set to the Menu
	 */
	public static void setDims(Menu menu, Size newSize) {
		ScilabMenuBridge.setDims(menu, newSize);
	}
	
	/**
	 * Gets the dimensions (width and height) of a swing Scilab Menu
	 * @param menu the Menu which we want to get the Size of
	 * @return the dimensions of the Menu
	 */
	public static Size getDims(Menu menu) {
		return ScilabMenuBridge.getDims(menu);
	}
	
	/**
	 * Gets the position (X-coordinate and Y-coordinate) of a menu
	 * @param menu the menu we want to get the position of
	 * @return position of the menu
	 */
	public static Position getPosition(Menu menu) {
		return ScilabMenuBridge.getPosition(menu);
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a menu
	 * @param menu the menu we want to set the position of
	 * @param newPosition new position of the menu
	 */
	public static void setPosition(Menu menu, Position newPosition) {
		ScilabMenuBridge.setPosition(menu, newPosition);
	}
	
	/**
	 * Gets the visibility status of a menu
	 * @param menu the menu we want to get the status of
	 * @return visibility status (true if the menu is visible, false if not)
	 */
	public static boolean isVisible(Menu menu) {
		return ScilabMenuBridge.isVisible(menu);
	}

	/**
	 * Sets the visibility status for a menu
	 * @param menu the menu we want to set the visibility status of
	 * @param newVisibleState new visibility status for the menu
	 * 			(true set the menu visible, false to set the menu invisible)
	 */
	public static void setVisible(Menu menu, boolean newVisibleState) {
		ScilabMenuBridge.setVisible(menu, newVisibleState);
	}
	
	/**
	 * Append a MenuItem to a MenuBar
	 * @param menu the Menu which we want to add the MenuItem to
	 * @param newMenuItem the MenuItem to add to the Menu
	 * @return the added MenuItem
	 */
	public static MenuItem add(Menu menu, MenuItem newMenuItem) {
		return ScilabMenuBridge.add(menu, newMenuItem);
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
	public static PushButton createPushButton() {
		return ScilabPushButtonBridge.createPushButton();
	}
	
}

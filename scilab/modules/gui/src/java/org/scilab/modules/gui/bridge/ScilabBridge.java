/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent Couvert
 * Copyright (C) 2007 - INRIA - Bruno Jofret
 * Copyright (C) 2010 - DIGITEO - Vincent COUVERT
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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
import org.scilab.modules.gui.checkboxmenuitem.CheckBoxMenuItem;
import org.scilab.modules.gui.checkboxmenuitem.ScilabCheckBoxMenuItemBridge;
import org.scilab.modules.gui.checkboxmenuitem.SimpleCheckBoxMenuItem;
import org.scilab.modules.gui.colorchooser.ColorChooser;
import org.scilab.modules.gui.colorchooser.ScilabColorChooserBridge;
import org.scilab.modules.gui.colorchooser.SimpleColorChooser;
import org.scilab.modules.gui.console.Console;
import org.scilab.modules.gui.console.ScilabConsole;
import org.scilab.modules.gui.console.ScilabConsoleBridge;
import org.scilab.modules.gui.console.SimpleConsole;
import org.scilab.modules.gui.contextmenu.ContextMenu;
import org.scilab.modules.gui.contextmenu.ScilabContextMenuBridge;
import org.scilab.modules.gui.contextmenu.SimpleContextMenu;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.filechooser.FileChooser;
import org.scilab.modules.gui.filechooser.ScilabFileChooserBridge;
import org.scilab.modules.gui.filechooser.SimpleFileChooser;
import org.scilab.modules.gui.fontchooser.FontChooser;
import org.scilab.modules.gui.fontchooser.ScilabFontChooserBridge;
import org.scilab.modules.gui.fontchooser.SimpleFontChooser;
import org.scilab.modules.gui.helpbrowser.HelpBrowser;
import org.scilab.modules.gui.helpbrowser.ScilabHelpBrowserBridge;
import org.scilab.modules.gui.helpbrowser.SimpleHelpBrowser;
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
import org.scilab.modules.gui.tree.ScilabTreeBridge;
import org.scilab.modules.gui.tree.SimpleTree;
import org.scilab.modules.gui.tree.Tree;
import org.scilab.modules.gui.uidisplaytree.ScilabUiDisplayTreeBridge;
import org.scilab.modules.gui.uidisplaytree.SimpleUiDisplayTree;
import org.scilab.modules.gui.uidisplaytree.UiDisplayTree;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;
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

    /**
     * DeIconify Window
     * @param window window to update
     */
    public static void windowDeiconified(Window window) {
        ScilabWindowBridge.windowDeiconified(window);
    }

    /**
     * Iconify Window
     * @param window window to update
     */
    public static void windowIconified(Window window) {
        ScilabWindowBridge.windowIconified(window);
    }

    /**
     * Maximized Window
     * @param window window to update
     */
    public static void windowMaximized(Window window) {
        ScilabWindowBridge.windowMaximized(window);
    }

    /**
     * Window is in the "normal" state.
     * @param window window to update
     */
    public static void windowNormal(Window window) {
        ScilabWindowBridge.windowNormal(window);
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
     * Creates a Scilab tab
     * @param name the name of the tab
     * @param uuid an uuid to identify the tab
     * @return the created tab
     */
    public static SimpleTab createTab(String name, String uuid) {
        return ScilabTabBridge.createTab(name, uuid);
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

    public static int addMember(Tab tab, UiDisplayTree member) {
        return ScilabTabBridge.addMember(tab, member);
    }
    public static void removeMember(Tab tab, UiDisplayTree member) {
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
    public static void setCallback(Tab tab, CommonCallBack callback) {
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
     * @param onOrOff set whether the swap buffer mode is on or off.
     */
    public static void setAutoSwapBufferMode(Canvas canvas, boolean onOrOff) {
        ScilabCanvasBridge.setAutoSwapBufferMode(canvas, onOrOff);
    }

    /**
     * @param canvas the canvas from which we want to retrieve the swap buffer mode.
     * @return set whether the swap buffer mode is on or off.
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
        return createMenuItem(true);
    }

    /**
     * Creates a new MenuItem
     * @param autoCheckedMode if false, menu checking is managed by the user (and not automatically by Java)
     * @return the created MenuItem
     */
    public static SimpleMenuItem createMenuItem(boolean autoCheckedMode) {
        return ScilabMenuItemBridge.createMenuItem(autoCheckedMode);
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
    public static void setCallback(MenuItem menuItem, CommonCallBack callback) {
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
    public static CommonCallBack getCallback(MenuItem menuItem) {
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
        return createCheckBoxMenuItem(true);
    }

    /**
     * Creates a new CheckBoxMenuItem
     * @param autoCheckedMode if false, menu checking is managed by the user (and not automatically by Java)
     * @return the created CheckBoxMenuItem
     */
    public static SimpleCheckBoxMenuItem createCheckBoxMenuItem(boolean autoCheckedMode) {
        return ScilabCheckBoxMenuItemBridge.createCheckBoxMenuItem(autoCheckedMode);
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
    public static void setCallback(CheckBoxMenuItem checkBoxMenuItem, CommonCallBack callback) {
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
    public static CommonCallBack getCallback(CheckBoxMenuItem checkBoxMenuItem) {
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
     * @param menu the Menu
     * @return the CallBack
     */
    public static CommonCallBack getCallback(Menu menu) {
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
    public static void setCallback(Menu menu, CommonCallBack callback) {
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
    public static void setCallback(ContextMenu contextMenu, CommonCallBack callback) {
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
    public static void setCallback(TextBox textBox, CommonCallBack callback) {
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
    public static SimpleFileChooser createExportFileChooser(Integer figureUID) {
        return ScilabFileChooserBridge.createExportFileChooser(figureUID);
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

    /**
     * Set the component used to set the location of the MessageBox (default is Scilab Console)
     * @param messageBox the MessageBox we want to set the parent of
     * @param parent the tab to be set as parent
     */
    public static void setParentForLocation(MessageBox messageBox, SimpleTab parent) {
        ScilabMessageBoxBridge.setParentForLocation(messageBox, parent);
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

    /**
     * Display a given tree
     * @param tree a tree structure
     */
    public static void  showTree(Tree tree) {
        ScilabTreeBridge.showTree(tree);
    }

    /**
     * Create a Scilab Tree
     * @param scilabTree a tree structure
     * @return a graphical tree
     */
    public static SimpleTree createTree(Tree scilabTree) {
        return ScilabTreeBridge.createTree(scilabTree);
    }

    /**
     * Function help on the keyword
     * @param console scilab console
     */
    public static void helpOnTheKeyword(ScilabConsole console) {
        ScilabConsoleBridge.helpOnTheKeyword(console);

    }

    /**
     * Evaluate the selection with echo
     * @param console the console
     */
    public static void evaluateSelectionWithEcho(ScilabConsole console) {
        ScilabConsoleBridge.evaluateSelectionWithEcho(console);
    }

    /**
     * Evaluate the selection with no echo
     * @param console the console
     */
    public static void evaluateSelectionWithNoEcho(ScilabConsole console) {
        ScilabConsoleBridge.evaluateSelectionWithNoEcho(console);
    }

    /**********************/
    /* UiDisplayTree Bridge */
    /**********************/

    /**
     * Creates a new UiDisplayTree
     * @return the created UiDisplayTree
     */
    public static SimpleUiDisplayTree createUiDisplayTree() {
        return ScilabUiDisplayTreeBridge.createUiDisplayTree();
    }

    /**
     * Sets the Text of a UiDisplayTree
     * @param uiTree the UiDisplayTree we want to set the Text of
     * @param newText the Text to set for the UiDisplayTree
     */
    public static void setText(UiDisplayTree uiTree, String newText) {
        ScilabUiDisplayTreeBridge.setText(uiTree, newText);
    }

    /**
     * Gets the text of an UiDisplayTree
     * @param uiTree the UiDisplayTree we want to get the text of
     * @return the text of the UiDisplayTree
     */
    public static String getText(UiDisplayTree uiTree) {
        return ScilabUiDisplayTreeBridge.getText(uiTree);
    }

    /**
     * Draws a Scilab UiDisplayTree
     * @param uiTree the UiDisplayTree to draw
     * @see org.scilab.modules.ihm.UIElement#draw()
     */
    public static void draw(UiDisplayTree uiTree) {
        ScilabUiDisplayTreeBridge.draw(uiTree);
    }

    /**
     * Gets the dimensions (width and height) of a Scilab UiDisplayTree
     * @param uiTree the UiDisplayTree we want to get the dimensions of
     * @return the size of the uiTree
     * @see org.scilab.modules.ihm.UIElement#getDims()
     */
    public static Size getDims(UiDisplayTree uiTree) {
        return ScilabUiDisplayTreeBridge.getDims(uiTree);
    }

    /**
     * Gets the position (X-coordinate and Y-coordinate) of a Scilab uiTree
     * @param uiTree the uiTree we want to get the position of
     * @return the position of the uiTree
     * @see org.scilab.modules.ihm.UIElement#getPosition()
     */
    public static Position getPosition(UiDisplayTree uiTree) {
        return ScilabUiDisplayTreeBridge.getPosition(uiTree);
    }

    /**
     * Gets the visibility status of a Scilab UiDisplayTree
     * @param uiTree the uiTree we want to get the visiblity status of
     * @return the visibility status of the uiTree (true if the uiTree is visible, false if not)
     * @see org.scilab.modules.ihm.UIElement#isVisible()
     */
    public static boolean isVisible(UiDisplayTree uiTree) {
        return ScilabUiDisplayTreeBridge.isVisible(uiTree);
    }

    /**
     * Sets the dimensions (width and height) of a Scilab UiDisplayTree
     * @param uiTree the uiTree we want to set the dimensions of
     * @param newSize the size we want to set to the uiTree
     * @see org.scilab.modules.ihm.UIElement#setDims(org.scilab.modules.ihm.utils.Size)
     */
    public static void setDims(UiDisplayTree uiTree, Size newSize) {
        ScilabUiDisplayTreeBridge.setDims(uiTree, newSize);
    }

    /**
     * Sets the position (X-coordinate and Y-coordinate) of a Scilab uiTree
     * @param uiTree the uiTree we want to set the position of
     * @param newPosition the position we want to set to the uiTree
     * @see org.scilab.modules.ihm.UIElement#setPosition(org.scilab.modules.ihm.utils.Position)
     */
    public static void setPosition(UiDisplayTree uiTree, Position newPosition) {
        ScilabUiDisplayTreeBridge.setPosition(uiTree, newPosition);
    }

    /**
     * Sets the visibility status of a Scilab UiDisplayTree
     * @param uiTree the uiTree we want to set the visiblity status of
     * @param newVisibleState the visibility status we want to set to the uiTree (true to set the uiTree visible, false else)
     * @see org.scilab.modules.ihm.UIElement#setVisible(boolean)
     */
    public static void setVisible(UiDisplayTree uiTree, boolean newVisibleState) {
        ScilabUiDisplayTreeBridge.setVisible(uiTree, newVisibleState);
    }

    /**
     * Set the Background color of the UiDisplayTree
     * @param uiTree the UiDisplayTree we want to set the background of
     * @param color the Color
     */
    public static void setBackground(UiDisplayTree uiTree, Color color) {
        ScilabUiDisplayTreeBridge.setBackground(uiTree, color);
    }

    /**
     * Get the Background color of the UiDisplayTree
     * @param uiTree the UiDisplayTree we want to get the background of
     * @return the Color
     */
    public static Color getBackground(UiDisplayTree uiTree) {
        return ScilabUiDisplayTreeBridge.getBackground(uiTree);
    }

    /**
     * Set the Foreground color of the UiDisplayTree
     * @param uiTree the UiDisplayTree we want to set the foreground of
     * @param color the Color
     */
    public static void setForeground(UiDisplayTree uiTree, Color color) {
        ScilabUiDisplayTreeBridge.setForeground(uiTree, color);
    }

    /**
     * Get the Foreground color of the UiDisplayTree
     * @param uiTree the UiDisplayTree we want to get the foreground of
     * @return the Color
     */
    public static Color getForeground(UiDisplayTree uiTree) {
        return ScilabUiDisplayTreeBridge.getForeground(uiTree);
    }

    /**
     * Set the font of the UiDisplayTree.
     * @param uiTree the UiDisplayTree we want to set the font of
     * @param font the font
     */
    public static void setFont(UiDisplayTree uiTree, Font font) {
        ScilabUiDisplayTreeBridge.setFont(uiTree, font);
    }

    /**
     * Get the font of the UiDisplayTree.
     * @param uiTree the UiDisplayTree we want to get the font of
     * @return the font
     */
    public static Font getFont(UiDisplayTree uiTree) {
        return ScilabUiDisplayTreeBridge.getFont(uiTree);
    }

    /**
     * Set if the UiDisplayTree is enabled or not
     * @param uiTree the UiDisplayTree we want to set the status of
     * @param status true if the UiDisplayTree is enabled
     */
    public static void setEnabled(UiDisplayTree uiTree, boolean status) {
        ScilabUiDisplayTreeBridge.setEnabled(uiTree, status);
    }

    /**
     * Gets the enable status of a UiDisplayTree
     * @param uiTree the UiDisplayTree we want to get the status of
     * @return the enable status of the UiDisplayTree (true if the UiDisplayTree is enabled, false if not)
     */
    public static boolean isEnabled(UiDisplayTree uiTree) {
        return ScilabUiDisplayTreeBridge.isEnabled(uiTree);
    }

    /**
     * Add a callback to the UiDisplayTree
     * @param uiTree the UiDisplayTree we want to set the callback of
     * @param callback the Callback to set.
     */
    public static void setCallback(UiDisplayTree uiTree, CommonCallBack callback) {
        ScilabUiDisplayTreeBridge.setCallback(uiTree, callback);
    }

    /**
     * Set the horizontal alignment for the UiDisplayTree text
     * @param uiTree the UiDisplayTree we want to set the alignment of
     * @param alignment the value for the alignment (See ScilabAlignment.java)
     */
    public static void setHorizontalAlignment(UiDisplayTree uiTree, String alignment) {
        ScilabUiDisplayTreeBridge.setHorizontalAlignment(uiTree, alignment);
    }

    /**
     * Set the vertical alignment for the UiDisplayTree text
     * @param uiTree the UiDisplayTree we want to set the alignment of
     * @param alignment the value for the alignment (See ScilabAlignment.java)
     */
    public static void setVerticalAlignment(UiDisplayTree uiTree, String alignment) {
        ScilabUiDisplayTreeBridge.setVerticalAlignment(uiTree, alignment);
    }

    /**
     * Set the Relief of the UiDisplayTree
     * @param uiTree the UiDisplayTree which we want to set the Relief of
     * @param reliefType the type of the relief to set (See ScilabRelief.java)
     */
    public static void setRelief(UiDisplayTree uiTree, String reliefType) {
        ScilabUiDisplayTreeBridge.setRelief(uiTree, reliefType);
    }

    /**
     * Delete the UiDisplayTree
     * @param uiTree the UiDisplayTree to be destroyed
     */
    public static void destroy(UiDisplayTree uiTree) {
        ScilabUiDisplayTreeBridge.destroy(uiTree);
    }

    /**
     * Give the focus to the UiDisplayTree
     * @param uiTree the UiDisplayTree
     */
    public static void requestFocus(UiDisplayTree uiTree) {
        ScilabUiDisplayTreeBridge.requestFocus(uiTree);
    }

    /**
     * Sets the tree data to be added to viewport
     * @param text the array of strings containing tree data
     */
    public static void setData(UiDisplayTree uiTree, String[] text) {
        ScilabUiDisplayTreeBridge.setData(uiTree, text);
    }
}

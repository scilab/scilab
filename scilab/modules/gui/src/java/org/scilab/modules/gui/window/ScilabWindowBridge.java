/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent Couvert
 * Copyright (C) 2007 - INRIA - Bruno JOFRET
 * Copyright (C) 2007 - INRIA - Marouane BEN JELLOUL
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

package org.scilab.modules.gui.window;

import org.scilab.modules.gui.bridge.window.SwingScilabWindow;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

/**
 * Bridge for Scilab windows in GUIs
 * @author Vincent COUVERT
 * @author Bruno JOFRET
 * @author Marouane BEN JELLOUL
 */
public class ScilabWindowBridge {

    /**
     * Constructor
     */
    protected ScilabWindowBridge() {
        throw new UnsupportedOperationException(); /* Prevents calls from subclass */
    };

    /**
     * Draws a Scilab window
     * @param window the window to draw
     */
    public static void draw(Window window) {
        window.getAsSimpleWindow().draw();
    }

    /**
     * Deiconify the window and put it in front of other window
     * @param window the window to raise
     */
    public static void raise(Window window) {
        window.getAsSimpleWindow().raise();
    }

    /**
     * Creates a Scilab window
     * @return the created window
     */
    public static SimpleWindow createWindow() {
        throw new UnsupportedOperationException();
    }

    /**
     * Gets the dimensions (width and height) of a Scilab window
     * @param window the window we want to get the dimensions of
     * @return the dimensions of the window
     */
    public static Size getDims(Window window) {
        return window.getAsSimpleWindow().getDims();
    }

    /**
     * Sets the dimensions (width and height) of a Scilab window
     * @param window the window we want to set the dimensions of
     * @param newWindowSize the dimensions we want to set to the window
     */
    public static void setDims(Window window, Size newWindowSize) {
        window.getAsSimpleWindow().setDims(newWindowSize);
    }

    /**
     * Gets the position (X-coordinate and Y-coordinate) of a Scilab window
     * @param window the window we want to get the position of
     * @return the position of the window
     */
    public static Position getPosition(Window window) {
        return window.getAsSimpleWindow().getPosition();
    }

    /**
     * Sets the position (X-coordinate and Y-coordinate) of a Scilab window
     * @param window the window we want to set the position of
     * @param newWindowPosition the position to set to the window
     */
    public static void setPosition(Window window, Position newWindowPosition) {
        window.getAsSimpleWindow().setPosition(newWindowPosition);
    }

    /**
     * Gets the title of a Scilab windo
     * @param window the window we want to get the title of
     * @return the title of the window
     */
    public static String getTitle(Window window) {
        return window.getAsSimpleWindow().getTitle();
    }

    /**
     * Sets the title of a Scilab window
     * @param window the window we want to set the title of
     * @param newWindowTitle the title we want to set to the window
     */
    public static void setTitle(Window window, String newWindowTitle) {
        window.getAsSimpleWindow().setTitle(newWindowTitle);
    }

    /**
     * Gets the visibility status of a Scilab window
     * @param window the window we want to getthe visibility status of
     * @return the visibility status of the window (true if the window is visible, false if not)
     */
    public static boolean isVisible(Window window) {
        return window.getAsSimpleWindow().isVisible();
    }

    /**
     * Sets the visibility status of a Scilab window
     * @param window the window we want to set the visibility status of
     * @param newVisibleState the visibility status we want to set to the window (true to set the window visible, false else)
     */
    public static void setVisible(Window window, boolean newVisibleState) {
        window.getAsSimpleWindow().setVisible(newVisibleState);
    }

    /**
     * Add a tab to a Scilab window
     * @param window the window which we want to add the tab to
     * @param newTab the tab to add to the window
     */
    public static void addTab(Window window, Tab newTab) {
        //window.getAsSimpleWindow().addTab(newTab.getAsSimpleTab());
        //newTab.getAsSimpleTab().setParentWindowId(window.getAsSimpleWindow().getElementId());
    }

    /**
     * Remove a tab from a window
     * @param window the window which we want to remove the tab from
     * @param tab tab to remove
     */
    public static void removeTab(Window window, Tab tab) {
        //window.getAsSimpleWindow().removeTab(tab.getAsSimpleTab());
    }

    /**
     * Sets a MenuBar to a Scilab window
     * @param window the window which we want to add the MenuBar to
     * @param newMenuBar the MenuBar to add to the window
     */
    public static void addMenuBar(Window window, MenuBar newMenuBar) {
        window.getAsSimpleWindow().addMenuBar(newMenuBar);
    }

    /**
     * Sets a ToolBar to a Scilab window
     * @param window the window which we want to add the newToolBar to
     * @param newToolBar the ToolBar to add to the window
     */
    public static void addToolBar(Window window, ToolBar newToolBar) {
        window.getAsSimpleWindow().addToolBar(newToolBar);
    }

    /**
     * Sets a InfoBar to a Scilab window
     * @param window the window which we want to add the newInfoBar to
     * @param newInfoBar the InfoBar to add to the window
     */
    public static void addInfoBar(Window window, TextBox newInfoBar) {
        window.getAsSimpleWindow().addInfoBar(newInfoBar);
    }

    /**
     * @param window window from which we want the information
     * @return number of objects (tabs) docked in this window
     */
    public static int getNbDockedObjects(Window window) {
        return window.getAsSimpleWindow().getNbDockedObjects();
    }

    /**
     * Update the dimension of the window and its component.
     * Only useful when the window is not yet visible
     * @param window window to update
     */
    public static void updateDimensions(Window window) {
        window.getAsSimpleWindow().updateDimensions();
    }

    /**
    * Deiconified Window
    */
    public static void windowDeiconified(Window window) {
        window.getAsSimpleWindow().windowDeiconified();
    }

    /**
    * Iconified Window
    */
    public static void windowIconified(Window window) {
        window.getAsSimpleWindow().windowIconified();
    }

    /**
    * Maximized Window
    */
    public static void windowMaximized(Window window) {
        window.getAsSimpleWindow().windowMaximized();
    }

    /**
    * Window is in the "normal" state.
    */
    public static void windowNormal(Window window) {
        window.getAsSimpleWindow().windowNormal();
    }

}

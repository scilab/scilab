/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent Couvert
 * Copyright (C) 2009 - DIGITEO - Sylvestre Koumar
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

package org.scilab.modules.gui.tab;

import org.scilab.modules.gui.bridge.tab.SwingScilabDockablePanel;
import org.scilab.modules.gui.canvas.Canvas;
import org.scilab.modules.gui.console.Console;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.helpbrowser.HelpBrowser;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.uidisplaytree.UiDisplayTree;
import org.scilab.modules.gui.tree.Tree;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

/**
 * Bridge for Scilab tabs in GUIs
 * @author Vincent COUVERT
 */
public class ScilabTabBridge {

    /**
     * Constructor
     */
    protected ScilabTabBridge() {
        throw new UnsupportedOperationException(); /* Prevents calls from subclass */
    };

    /**
     * Creates a Scilab tab
     * @param name the name of the tab (used to identify it)
     * @return the created tab
     */
    public static SimpleTab createTab(String name) {
        return new SwingScilabDockablePanel(name);
    }

    /**
     * Creates a Scilab tab
     * @param name the name of the tab (used to identify it)
     * @return the created tab
     */
    public static SimpleTab createTab(String name, String uuid) {
        return new SwingScilabDockablePanel(name, uuid);
    }

    /**
     * Creates a Scilab tab able to display a figure handle
     * @param name the name of the tab (used to identify it)
     * @param figureId id of the displayed figure
     * @return the created tab
     */
    public static SimpleTab createTab(String name, int figureId) {
        return new SwingScilabDockablePanel(name, figureId);
    }

    /**
     * Draws a Scilab tab
     * @param tab the tab to draw
     */
    public static void draw(Tab tab) {
        tab.getAsSimpleTab().draw();
    }

    /**
     * Gets the dimensions (width and height) of a Scilab tab
     * @param tab the tab we want to get the dimensions of
     * @return the dimensions of the tab
     */
    public static Size getDims(Tab tab) {
        return tab.getAsSimpleTab().getDims();
    }

    /**
     * Sets the dimensions (width and height) of a Scilab tab
     * @param tab the tab we want to set the dimensions of
     * @param newTabSize the dimensions we want to set to the tab
     */
    public static void setDims(Tab tab, Size newTabSize) {
        tab.getAsSimpleTab().setDims(newTabSize);
    }

    /**
     * Gets the position (X-coordinate and Y-coordinate) of a Scilab tab
     * @param tab the tab we want to get the position of
     * @return the position of the tab
     */
    public static Position getPosition(Tab tab) {
        return tab.getAsSimpleTab().getPosition();
    }

    /**
     * Sets the position (X-coordinate and Y-coordinate) of a Scilab tab
     * @param tab the tab we want to set the position of
     * @param newTabPosition the position we want to set to the tab
     */
    public static void setPosition(Tab tab, Position newTabPosition) {
        tab.getAsSimpleTab().setPosition(newTabPosition);
    }

    /**
     * Gets the Name of a Scilab tab
     * @param tab the tab we want to get the title of
     * @return the title of the tab
     */
    public static String getName(Tab tab) {
        return tab.getAsSimpleTab().getName();
    }

    /**
     * Sets the name of a Scilab tab
     * @param tab the tab we want to set the name of
     * @param newTabName the name to set to the tab
     */
    public static void setName(Tab tab, String newTabName) {
        tab.getAsSimpleTab().setName(newTabName);
    }

    /**
     * Gets the visibility status of a Scilab tab
     * @param tab the tab we want to get the visibility status of
     * @return the visibility status of the tab (true if the tab is visible, false if not)
     */
    public static boolean isVisible(Tab tab) {
        return tab.getAsSimpleTab().isVisible();
    }

    /**
     * Sets the visibility status of a Scilab tab
     * @param tab the tab we want to set the visibility status of
     * @param newVisibleState the visibility status to set to the tab (true to set the tab visible, false else)
     */
    public static void setVisible(Tab tab, boolean newVisibleState) {
        tab.getAsSimpleTab().setVisible(newVisibleState);
    }

    /**
     * Add a member (dockable element) to a tab and returns the index of this member
     * @param tab the tab where we want to add the member
     * @param member the member to add
     * @return index of member
     */
    public static int addMember(Tab tab, Console member) {
        return tab.getAsSimpleTab().addMember(member);
    }

    /**
     * We want to be able to add directly a HelpBrowser in a Tab.
     * @param tab the tab where we want to add the member
     * @param member the member to add
     * @return the position of the HelpBrowser in the member list.
     */
    public static int addMember(Tab tab, HelpBrowser member) {
        return tab.getAsSimpleTab().addMember(member);
    }

    /**
     * Add a member (dockable element) to a tab and returns the index of this member
     * @param tab the tab where we want to add the member
     * @param member the member to add
     * @return index of member
     */
    public static int addMember(Tab tab, Canvas member) {
        return (tab.getAsSimpleTab().addMember(member));
    }

    public static int addMember(Tab tab, UiDisplayTree member) {
        return (tab.getAsSimpleTab().addMember(member));
    }
    public static void removeMember(Tab tab, UiDisplayTree member) {
        tab.getAsSimpleTab().removeMember(member);
    }

    /**
     * Sets a MenuBar to a Scilab tab
     * @param tab the tab which we want to add the MenuBar to
     * @param newMenuBar the MenuBar to add to the tab
     */
    public static void addMenuBar(Tab tab, MenuBar newMenuBar) {
        tab.addMenuBar(newMenuBar);
    }

    /**
     * We want to be able to remove directly a Canvas from a Tab.
     * @param tab the tab from which we want to remove the Canvas from
     * @param member canvas to remove
     */
    public static void removeMember(Tab tab, Canvas member) {
        tab.getAsSimpleTab().removeMember(member);
    }

    /**
     * Add a member (dockable element) to a tab and returns the index of this member
     * @param tab the tab which we want to add the Tree to
     * @param member the Tree to add
     * @return the position of the Tree in the member list.
     */
    public static int addMember(Tab tab, Tree member) {
        return (tab.getAsSimpleTab().addMember(member));
    }

    /**
     * Remove a Tree member from a tab
     * @param tab the tab which we want to remove the Tree from
     * @param member the Tree to add
     */
    public static void removeMember(Tab tab, Tree member) {
        tab.getAsSimpleTab().removeMember(member);
    }

    /**
     * Get the current status of the Tab in its parent
     * @param tab the tab which we want to add the MenuBar to
     * @return true is the tab is the tab currently "on top" in its parent
     */
    public static boolean isCurrentTab(Tab tab) {
        return (tab.getAsSimpleTab().isCurrentTab());
    }

    /**
     * Set the parent window id for this tab
     * @param tab the tab which we want to set the parent window id
     * @param id the id of the parent window
     */
    public static void setParentWindowId(Tab tab, int id) {
        //tab.getAsSimpleTab().setParentWindowId(id);
    }

    /**
     * Get the parent window id for this tab
     * @param tab the tab which we want to get the parent window id
     * @return the id of the parent window
     */
    public static int getParentWindowId(Tab tab) {
        return 0;//tab.getAsSimpleTab().getParentWindowId();
    }

    /**
     * Set the callback of the tab
     * @param tab the tab which we want to set the callback of
     * @param callback the CallBack to set
     */
    public static void setCallback(Tab tab, CommonCallBack callback) {
        tab.getAsSimpleTab().setCallback(callback);
    }

    /**
     * Set this tab as the current tab of its parent Window
     * @param tab the tab
     */
    public static void setCurrent(Tab tab) {
        tab.getAsSimpleTab().setCurrent();
    }

    /**
     * Set the background color of a tab.
     * @param tab tab to modify
     * @param red red channel of the color
     * @param green green channel
     * @param blue blue channel
     */
    public static void setBackground(Tab tab, double red, double green, double blue) {
        tab.getAsSimpleTab().setBackground(red, green, blue);
    }

    /**
     * Specify whether the canvas should fit the parent tab size
     * (and consequently the scrollpane size) or not
     * @param tab tab to modify
     * @param onOrOff true to enable autoresize mode
     */
    public static void setAutoResizeMode(Tab tab, boolean onOrOff) {
        tab.getAsSimpleTab().setAutoResizeMode(onOrOff);
    }

    /**
     * @param tab tab to modify
     * @return whether the resize mode is on or off
     */
    public static boolean getAutoResizeMode(Tab tab) {
        return tab.getAsSimpleTab().getAutoResizeMode();
    }

    /**
     * Get the part of the axes which is currently viewed
     * @param tab tab to modify
     * @return [x,y,w,h] array
     */
    public static int[] getViewingRegion(Tab tab) {
        return tab.getAsSimpleTab().getViewingRegion();
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
        tab.getAsSimpleTab().setViewingRegion(posX, posY, width, height);
    }


    /**
     * @param tab tab to modify
     * @return size of the axes in pixels
     */
    public static Size getAxesSize(Tab tab) {
        return tab.getAsSimpleTab().getAxesSize();
    }


    /**
     * @param tab tab to modify
     * @param newSize set a new axes size
     */
    public static void setAxesSize(Tab tab, Size newSize) {
        tab.getAsSimpleTab().setAxesSize(newSize);
    }

    /**
     * Set the event handler of the Axes
     * @param tab tab to modify
     * @param command the name of the Scilab function to call
     */
    public static void setEventHandler(Tab tab, String command) {
        tab.getAsSimpleTab().setEventHandler(command);
    }

    /**
     * Set the status of the event handler of the Axes
     * @param tab tab to modify
     * @param status is true to set the event handler active
     */
    public static void setEventHandlerEnabled(Tab tab, boolean status) {
        tab.getAsSimpleTab().setEventHandlerEnabled(status);
    }

}

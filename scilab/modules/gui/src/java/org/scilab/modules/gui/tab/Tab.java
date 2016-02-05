/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent Couvert
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

import org.scilab.modules.gui.console.Console;
import org.scilab.modules.gui.canvas.Canvas;
import org.scilab.modules.gui.container.Container;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.helpbrowser.HelpBrowser;
import org.scilab.modules.gui.tree.Tree;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.gui.window.Window;

/**
 * Interface for tabs in Scilab GUIs
 * @author Vincent COUVERT
 */
public interface Tab extends Container {

    /**
     * Gets a much more dummy Objects.
     * @return the component.
     */
    SimpleTab getAsSimpleTab();

    /**
     * Gets the Name of a tab
     * @return the Name of the tab
     */
    String getName();

    /**
     * Sets the Name of a tab
     * @param newTabName the Name we want to set to the tab
     */
    void setName(String newTabName);

    /**
     * We want to be able to add directly a Canvas in a Tab.
     * @param member the member to add
     * @return the position of the canvas in the member list.
     */
    int addMember(Canvas member);

    /**
     * We want to be able to add directly a Console in a Tab.
     * @param member the member to add
     * @return the position of the canvas in the member list.
     */
    int addMember(Console member);

    /**
     * We want to be able to add directly a HelpBrowser in a Tab.
     * @param member the member to add
     * @return the position of the HelpBrowser in the member list.
     */
    int addMember(HelpBrowser member);

    /**
     * We want to be able to add directly a Tree Overview in a Tab.
     * @param member the member to add
     * @return the position of the Tree Overview in the member list.
     */
    int addMember(Tree member);

    /**
     * We want to be able to remove directly a Canvas from a Tab.
     * @param member canvas to remove
     */
    void removeMember(Canvas member);

    /**
     * Destroy the tab.
     */
    void close();

    /**
     * Get the current status of the Tab in its parent
     * @return true is the tab is the tab currently "on top" in its parent
     */
    boolean isCurrentTab();

    /**
     * Set the parent window id for this tab
     * @param id the id of the parent window
     */
    void setParentWindowId(int id);

    /**
     * Get the parent window id for this tab
     * @return the id of the parent window
     */
    int getParentWindowId();

    /**
     * Set the callback of the tab
     * @param callback the CallBack to set
     */
    void setCallback(CommonCallBack callback);

    /**
     * Get the parent Window
     * @return parent window of the tab object
     */
    Window getParentWindow();

    /**
     * Set this tab as the current tab of its parent Window
     */
    void setCurrent();

    /**
     * Set the background color of the tab.
     * @param red red channel of the color
     * @param green green channel
     * @param blue blue channel
     */
    void setBackground(double red, double green, double blue);

    /**
     * Specify whether the canvas should fit the parent tab size
     * (and consequently the scrollpane size) or not
     * @param onOrOff true to enable autoresize mode
     */
    void setAutoResizeMode(boolean onOrOff);

    /**
     * @return whether the resize mode is on or off
     */
    boolean getAutoResizeMode();

    /**
     * Get the part of the axes which is currently viewed
     * @return [x,y,w,h] array
     */
    int[] getViewingRegion();

    /**
     * Specify a new viewport for the axes
     * For SwingScilabCanvas viewport can not be modified
     * since it match the parent tab size
     * @param posX X coordinate of upper left point of the viewport within the canvas
     * @param posY Y coordinate of upper left point of the viewport within the canvas
     * @param width width of the viewport
     * @param height height of the viewport
     */
    void setViewingRegion(int posX, int posY, int width, int height);


    /**
     * @return size of the axes in pixels
     */
    Size getAxesSize();


    /**
     * @param newSize set a new axes size
     */
    void setAxesSize(Size newSize);

    /**
     * Set the event handler of the Canvas
     * @param command the name of the Scilab function to call
     */
    void setEventHandler(String command);

    /**
     * Set the status of the event handler of the Canvas
     * @param status is true to set the event handler active
     */
    void setEventHandlerEnabled(boolean status);

}

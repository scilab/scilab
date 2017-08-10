/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent Couvert
 * Copyright (C) 2007 - INRIA - Bruno JOFRET
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

import org.scilab.modules.gui.bridge.ScilabBridge;
import org.scilab.modules.gui.canvas.Canvas;
import org.scilab.modules.gui.console.Console;
import org.scilab.modules.gui.container.ScilabContainer;
import org.scilab.modules.gui.dockable.Dockable;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.helpbrowser.HelpBrowser;
import org.scilab.modules.gui.menubar.MenuBar;
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
     ** Just let the Bridge do his job translating Scilab thinking to Java
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
     * Constructor
     * @param name - name of the Tab
     * @param uuid an uuid to identify the tab
     */
    protected ScilabTab(String name, String uuid) {
        component = ScilabBridge.createTab(name, uuid);
    }

    /**
     * Constructor for a graphical tab
     * @param name - name of the Tab
     * @param uuid an uuid which will be used to restore the tab
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
     * @param name the name of the tab
     * @param uuid an uuid to identify the tab
     * @return the created Scilab Tab
     */
    public static Tab createTab(String name, String uuid) {
        return new ScilabTab(name, uuid);
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
     * We want to be able to add directly a Tree Overview in a Tab.
     * @param member the Tree Overview to add
     * @return the position of the Tree Overview in the member list.
     */
    public int addMember(Tree member) {
        return ScilabBridge.addMember(this, member);
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
    public void setCallback(CommonCallBack callback) {
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

}


/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Bruno JOFRET
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.gui.tab;

import org.scilab.modules.gui.checkbox.CheckBox;
import org.scilab.modules.gui.console.Console;
import org.scilab.modules.gui.canvas.Canvas;
import org.scilab.modules.gui.editbox.EditBox;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.frame.Frame;
import org.scilab.modules.gui.helpbrowser.HelpBrowser;
import org.scilab.modules.gui.label.Label;
import org.scilab.modules.gui.listbox.ListBox;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.popupmenu.PopupMenu;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.gui.radiobutton.RadioButton;
import org.scilab.modules.gui.uitable.UiTable;
import org.scilab.modules.gui.uidisplaytree.UiDisplayTree;
import org.scilab.modules.gui.slider.Slider;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.tree.Tree;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

/**
 * Interface for tabs in Scilab GUIs
 * Describe all what the "widget" tab
 * must be able to do as a GUI Component
 * @author Bruno JOFRET
 */
public interface SimpleTab {

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
     * We want to be able to add directly a Frame in a Tab.
     * @param member the member to add
     * @return the position of the frame in the member list.
     */
    int addMember(Frame member);

    /**
     * Remove a Frame from a Tab.
     * @param member the Frame to remove
     */
    void removeMember(Frame member);

    /**
     * We want to be able to add directly a Console in a Tab.
     * @param member the member to add
     * @return the position of the console in the member list.
     */
    int addMember(Console member);

    /**
     * We want to be able to add directly a HelpBrowser in a Tab.
     * @param member the member to add
     * @return the position of the HelpBrowser in the member list.
     */
    int addMember(HelpBrowser member);

    /**
     * We want to be able to add directly a Console in a Tab.
     * @param member the member to add
     * @return the position of the console in the member list.
     */
    int addMember(Canvas member);

    /**
     * We want to be able to add directly a pushbutton in a Tab.
     * @param member the pushbutton to add
     * @return the position of the pushbutton in the member list.
     */
    int addMember(PushButton member);

    /**
     * Remove a PushButton from a Tab.
     * @param member the pushbutton to add
     */
    void removeMember(PushButton member);

    /**
     * We want to be able to add directly a editbox in a Tab.
     * @param member the editbox to add
     * @return the position of the editbox in the member list.
     */
    int addMember(EditBox member);

    /**
     * Remove an EditBox from a Tab.
     * @param member the EditBox to remove
     */
    void removeMember(EditBox member);

    /**
     * We want to be able to add directly a label in a Tab.
     * @param member the label to add
     * @return the position of the label in the member list.
     */
    int addMember(Label member);

    /**
     * Remove a Label from a Tab.
     * @param member the Label to remove
     */
    void removeMember(Label member);

    /**
     * We want to be able to add directly a checkbox in a Tab.
     * @param member the checkbox to add
     * @return the position of the checkbox in the member list.
     */
    int addMember(CheckBox member);

    /**
     * Remove a CheckBox from a Tab.
     * @param member the CheckBox to remove
     */
    void removeMember(CheckBox member);

    /**
     * We want to be able to add directly a RadioButton in a Tab.
     * @param member the RadioButton to add
     * @return the position of the RadioButton in the member list.
     */
    int addMember(RadioButton member);

    /**
     * Remove a RadioButton from a Tab.
     * @param member the RadioButton to remove
     */
    void removeMember(RadioButton member);

    int addMember(UiDisplayTree member);
    void removeMember(UiDisplayTree member);

    /**
     * We want to be able to add directly a UiTable in a Tab.
     * @param member the UiTable to add
     * @return the position of the UiTable in the member list.
     */
    int addMember(UiTable member);

    /**
     * Remove a UiTable from a Tab.
     * @param member the UiTable to remove
     */
    void removeMember(UiTable member);

    /**
     * We want to be able to add directly a Slider in a Tab.
     * @param member the Slider to add
     * @return the position of the Slider in the member list.
     */
    int addMember(Slider member);

    /**
     * Remove a Slider from a Tab.
     * @param member the Slider to remove
     */
    void removeMember(Slider member);

    /**
     * We want to be able to add directly a ListBox in a Tab.
     * @param member the ListBox to add
     * @return the position of the ListBox in the member list.
     */
    int addMember(ListBox member);

    /**
     * Remove a ListBox from a Tab.
     * @param member the ListBox to remove
     */
    void removeMember(ListBox member);

    /**
     * We want to be able to add directly a PopupMenu in a Tab.
     * @param member the PopupMenu to add
     * @return the position of the PopupMenu in the member list.
     */
    int addMember(PopupMenu member);

    /**
     * Remove a PopupMenu from a Tab.
     * @param member the PopupMenu to remove
     */
    void removeMember(PopupMenu member);

    /**
     * We want to be able to remove directly a Canvas from a Tab.
     * @param member canvas to remove
     */
    void removeMember(Canvas member);


    /**
     * We want to be able to add directly a PopupMenu in a Tab.
     * @param member the PopupMenu to add
     * @return the position of the PopupMenu in the member list.
     */
    int addMember(Tree member);


    /**
     * We want to be able to remove directly a Tree Overview from a Tab.
     * @param member Tree OverView to remove
     */
    void removeMember(Tree member);

    /**
     * Gets the size of an Tab (width and height)
     * @return the size of the Tab
     */
    Size getDims();

    /**
     * Sets the size of an Tab (width and height)
     * @param newSize the size we want to set to the Tab
     */
    void setDims(Size newSize);

    /**
     * Gets the position of an Tab (X-coordinate and Y-corrdinate)
     * @return the position of the Tab
     */
    Position getPosition();

    /**
     * Sets the position of an Tab (X-coordinate and Y-corrdinate)
     * @param newPosition the position we want to set to the Tab
     */
    void setPosition(Position newPosition);

    /**
     * Gets the visibility status of an Tab
     * @return the visibility status of the Tab (true if the Tab is visible, false if not)
     */
    boolean isVisible();

    /**
     * Sets the visibility status of an Tab
     * @param newVisibleState the visibility status we want to set for the Tab
     * 			(true if the Tab is visible, false if not)
     */
    void setVisible(boolean newVisibleState);

    /**
     * Draws an Tab
     */
    void draw();

    /**
     * Get the current status of the Tab in its parent
     * @return true is the tab is the tab currently "on top" in its parent
     */
    boolean isCurrentTab();

    /**
     * Set the parent window id for this tab
     * @param id the id of the parent window
     */
    void setParentWindowId(String id);

    /**
     * Get the parent window id for this tab
     * @return the id of the parent window
     */
    String getParentWindowId();

    /**
     * Get the MenuBar associated to this tab
     * @return MenuBar the MenuBar associated.
     */
    MenuBar getMenuBar();

    /**
     * Set the MenuBar associated to this tab
     * @param newMenuBar : the MenuBar to associate.
     */
    void setMenuBar(MenuBar newMenuBar);

    /**
     * Get the ToolBar associated to this tab
     * @return ToolBar the ToolBar associated.
     */
    ToolBar getToolBar();

    /**
     * Set the ToolBar associated to this tab
     * @param newToolBar : the ToolBar to associate.
     */
    void setToolBar(ToolBar newToolBar);

    /**
     * Get the InfoBar associated to this tab
     * @return infoBar the InfoBar associated.
     */
    TextBox getInfoBar();

    /**
     * Set the InfoBar associated to this tab
     * @param newInfoBar the InfoBar to associate.
     */
    void setInfoBar(TextBox newInfoBar);

    /**
     * Set the callback of the tab
     * @param callback the CallBack to set
     */
    void setCallback(CommonCallBack callback);

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

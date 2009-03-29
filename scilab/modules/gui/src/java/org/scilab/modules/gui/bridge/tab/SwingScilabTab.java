/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 20072008 - INRIA - Vincent Couvert
 * Copyright (C) 2007 - INRIA - Bruno JOFRET
 * Copyright (C) 2007 - INRIA - Marouane BEN JELLOUL
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.bridge.tab;

import java.awt.Color;
import java.awt.Component;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Point;
import java.lang.reflect.InvocationTargetException;
import java.util.Iterator;
import java.util.Set;

import javax.swing.Action;
import javax.swing.SwingUtilities;

import org.flexdock.docking.DockingConstants;
import org.flexdock.view.View;
import org.scilab.modules.gui.bridge.canvas.SwingScilabCanvasImpl;
import org.scilab.modules.gui.bridge.checkbox.SwingScilabCheckBox;
import org.scilab.modules.gui.bridge.console.SwingScilabConsole;
import org.scilab.modules.gui.bridge.editbox.SwingScilabEditBox;
import org.scilab.modules.gui.bridge.frame.SwingScilabFrame;
import org.scilab.modules.gui.bridge.helpbrowser.SwingScilabHelpBrowser;
import org.scilab.modules.gui.bridge.label.SwingScilabLabel;
import org.scilab.modules.gui.bridge.listbox.SwingScilabListBox;
import org.scilab.modules.gui.bridge.popupmenu.SwingScilabPopupMenu;
import org.scilab.modules.gui.bridge.pushbutton.SwingScilabPushButton;
import org.scilab.modules.gui.bridge.radiobutton.SwingScilabRadioButton;
import org.scilab.modules.gui.bridge.slider.SwingScilabSlider;
import org.scilab.modules.gui.bridge.window.SwingScilabWindow;
import org.scilab.modules.gui.canvas.Canvas;
import org.scilab.modules.gui.checkbox.CheckBox;
import org.scilab.modules.gui.console.Console;
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
import org.scilab.modules.gui.tab.SimpleTab;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.BarUpdater;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.SciUndockingAction;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.gui.utils.UIElementMapper;
import org.scilab.modules.gui.window.Window;

/**
 * Swing implementation for Scilab tabs in GUIs
 * This implementation uses FlexDock package
 * @author Bruno JOFRET
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 * @author Jean-Baptiste SILVY
 */
public class SwingScilabTab extends View implements SimpleTab {

    private static final long serialVersionUID = 1L;

    private static final int VIEWPORT_SIZE = 4;

    private int parentWindowId;

    private MenuBar menuBar;

    private ToolBar toolBar;

    private TextBox infoBar;

    /** Contains the canvas and widgets */
    private SwingScilabAxes contentPane;

    /** Scroll the axes */
    private ScilabScrollPane scrolling;

    /**
     * Constructor
     * @param name the name of the tab (used to identify it)
     */
    public SwingScilabTab(String name) {
	super(name, name, name);

	// This button is "overloaded" when we add a callback
	//this.addAction(DockingConstants.CLOSE_ACTION);
	// Removed because make JOGL crash when "Unpin"
	//this.addAction(DockingConstants.PIN_ACTION);
	this.addAction(DockingConstants.ACTIVE_WINDOW);

	// no need for an axes
	contentPane = null;
	scrolling = null;

	this.setVisible(true);

    }

    /**
     * Create a graphic tab used to display a figure with 3D graphics and/or UIcontrols
     * @param name name of the tab
     * @param figureId id of the displayed figure
     */
    public SwingScilabTab(String name, int figureId) {
	super(name, name, name);

	// This button is "overloaded" when we add a callback
	//this.addAction(DockingConstants.CLOSE_ACTION);
	// Removed because make JOGL crash when "Unpin"
	//this.addAction(DockingConstants.PIN_ACTION);
	this.addAction(DockingConstants.ACTIVE_WINDOW);

	// create the panel in which all the uiobjects will lie.
	contentPane = new SwingScilabAxes(figureId);

	// add it inside a JSCrollPane
	scrolling = new SwingScilabScrollPane(contentPane);

	// put in in the back of the tab
	setContentPane(scrolling.getAsContainer());


	this.setVisible(true);

    }

    /**
     * Repaint it
     */
    public void repaint() {
    	super.repaint();

    	/** Update toolbar / menubar / infobar / title */
    	Window parentWindow = (Window) UIElementMapper.getCorrespondingUIElement(parentWindowId);
    	if (parentWindow != null) {
    		Set<Dockable> dockables = ((SwingScilabWindow) parentWindow.getAsSimpleWindow()).getDockingPort().getDockables();
    	
    		if ((isShowing() && dockables.size() == 1) || isActive() || dockables.size() == 1) {
    			BarUpdater.updateBars(getParentWindowId(), getMenuBar(), getToolBar(), getInfoBar(), getName());
    		} else {
    			/** Try to find active tab */
    			Iterator<Dockable> it =  dockables.iterator();
    			while (it.hasNext()) {
    				SwingScilabTab dock = (SwingScilabTab) it.next();
    				if (((SwingScilabTab) dock).isActive()) {
    					BarUpdater.updateBars(getParentWindowId(), dock.getMenuBar(), dock.getToolBar(), dock.getInfoBar(), dock.getName());
    					return;
    				}
    			}
    		}
    	}
    }

    /**
     * Sets the Name of a swing Scilab tab
     * @param newTabName the Name of the tab
     * @see org.scilab.modules.gui.tab.Tab#setName()
     */
    public void setName(String newTabName) {
	setTitle(newTabName, true);
	if (isActive()) {
	    BarUpdater.updateBars(getParentWindowId(), getMenuBar(), getToolBar(), getInfoBar(), getName());
	}
    }

    /**
     * Gets the title of a swing Scilab tab
     * @return the title of the tab
     * @see org.scilab.modules.gui.tab.Tab#getTitle()
     */
    public String getName() {
	return this.getTitle();
    }

    /**
     * Paint immediately this component
     */
    public void paintImmediately() {
	// paint all
	paintImmediately(0, 0, getWidth(), getHeight());
	if (isActive()) {
	    BarUpdater.updateBars(getParentWindowId(), getMenuBar(), getToolBar(), getInfoBar(), getName());
	}
    }

    /**
     * Draws a swing Scilab tab
     * @see org.scilab.modules.gui.uielement.UIElement#draw()
     */
    public void draw() {
	if (SwingUtilities.isEventDispatchThread()) {
	    setVisible(true);
	    paintImmediately();
	} else {
	    try {
		SwingUtilities.invokeAndWait(new Runnable() {
		    public void run() {
			setVisible(true);
			paintImmediately();
		    }
		});
	    } catch (InterruptedException e) {
		e.printStackTrace();
	    } catch (InvocationTargetException e) {
		e.printStackTrace();
	    }
	}

    }

    /**
     * Gets the dimensions (width and height) of a swing Scilab tab
     * @return the dimensions of the tab
     * @see org.scilab.modules.gui.uielement.UIElement#getDims()
     */
    public Size getDims() {
	return new Size(this.getSize().width, this.getSize().height);
    }

    /**
     * Get the size for the axes
     * @return size of the axes in pixels
     */
    public Size getAxesSize() {
	return new Size(contentPane.getWidth(), contentPane.getHeight());
    }

    /**
     * @param newSize new size to set for the axes
     */
    public void setAxesSize(Size newSize) {
	contentPane.setSize(new Dimension(newSize.getWidth(), newSize.getHeight()));
    }

    /**
     * Gets the position (X-coordinate and Y-coordinate) of a swing Scilab tab
     * @return the position of the tab
     * @see org.scilab.modules.gui.uielement.UIElement#getPosition()
     */
    public Position getPosition() {
	return new Position(this.getX(), this.getY());
    }

    /**
     * Sets the dimensions (width and height) of a swing Scilab tab
     * @param newSize the dimensions we want to set to the tab
     * @see org.scilab.modules.gui.uielement.UIElement#setDims(org.scilab.modules.gui.utils.Size)
     */
    public void setDims(Size newSize) {
	this.setSize(newSize.getWidth(), newSize.getHeight());
    }

    /**
     * Sets the position (X-coordinate and Y-coordinate) of a swing Scilab tab
     * @param newPosition the position we want to set to the tab
     * @see org.scilab.modules.gui.uielement.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
     */
    public void setPosition(Position newPosition) {
	this.setLocation(newPosition.getX(), newPosition.getY());
    }

    /**
     * Add a member (dockable element) to container and returns its index
     * @param member the member to add
     * @return index of member in ArrayList
     */
    public int addMember(Canvas member) {
    	int result;

    	if (SwingScilabCanvasImpl.isGLCanvasEnabled()) {
    		int[] currentView = getViewingRegion();
    		final SwingScilabTab thisF = this;
    		try {
    			SwingUtilities.invokeAndWait(new Runnable() {
    				public void run() {
    					scrolling = new AwtScilabScrollPane(contentPane, thisF);
    					setContentPane(scrolling.getAsContainer());
    					revalidate();

    				}
    			});
    		} catch (InterruptedException e) {
    			e.printStackTrace();
    		} catch (InvocationTargetException e) {
    			e.getCause().printStackTrace();
    		}
    		// set the canvas after doing every thing
    		result = contentPane.addMember(member);
    		// set the same viewport as before
    		setViewingRegion(currentView[0], currentView[1], currentView[2], currentView[2 + 1]);
    	} else {
    		result = contentPane.addMember(member);
    	}
    	return result;
    }

    /**
     * We want to be able to remove directly a Canvas from a Tab.
     * @param member canvas to remove
     */
    public void removeMember(Canvas member) {
    	contentPane.removeMember(member);
    	if (SwingScilabCanvasImpl.isGLCanvasEnabled()) {
    		try {
    			SwingUtilities.invokeAndWait(new Runnable() {
    				public void run() {
    					scrolling = new SwingScilabScrollPane(contentPane);
    					setContentPane(scrolling.getAsContainer());
    					revalidate();
    				}
    			});
    		} catch (InterruptedException e) {
    			e.printStackTrace();
    		} catch (InvocationTargetException e) {
    			e.getCause().printStackTrace();
    		}
    	}
    }

    /**
     * Add a member (dockable element) to container and returns its index
     * @param member the member to add
     * @return index of member in ArrayList
     */
    public int addMember(Console member) {
	return this.addMember((SwingScilabConsole) member.getAsSimpleConsole());
    }

    /**
     * Add a member (dockable element) to container and returns its index
     * @param member the member to add
     * @return index of member in ArrayList
     */
    private int addMember(SwingScilabConsole member) {
	// replace the current content pane
	this.setContentPane(member);
	return this.getComponentZOrder(member);
    }

    /**
     * Add a member (dockable element) to container and returns its index
     * @param member the member to add
     * @return index of member in ArrayList
     */
    public int addMember(HelpBrowser member) {
	return this.addMember((SwingScilabHelpBrowser) member.getAsSimpleHelpBrowser());
    }

    /**
     * Add a member (dockable element) to container and returns its index
     * @param member the member to add
     * @return index of member in ArrayList
     */
    private int addMember(SwingScilabHelpBrowser member) {
	// replace the current content pane
	this.setContentPane(member);
	return this.getComponentZOrder(member);
    }

    /**
     * Add a member (dockable element) to container and returns its index
     * @param member the member to add
     * @return index of member in ArrayList
     */
    public int addMember(Frame member) {
	return this.addMember((SwingScilabFrame) member.getAsSimpleFrame());
    }

    /**
     * Add a member (dockable element) to container and returns its index
     * @param member the member to add
     * @return index of member in ArrayList
     */
    private int addMember(SwingScilabFrame member) {
	return contentPane.addFrame(member);
    }

    /**
     * Remove a Frame from its container
     * @param member the Frame to remove
     */
    public void removeMember(Frame member) {
	this.removeMember((SwingScilabFrame) member.getAsSimpleFrame());
    }

    /**
     * Remove a Frame from its container
     * @param member the Frame to remove
     */
    private void removeMember(SwingScilabFrame member) {
	contentPane.removeFrame(member);
    }

    /**
     * Add a member (dockable element) to container and returns its index
     * @param member the member to add
     * @return index of member in ArrayList
     */
    public int addMember(PushButton member) {
	return this.addMember((SwingScilabPushButton) member.getAsSimplePushButton());
    }

    /**
     * Add a member (dockable element) to container and returns its index
     * @param member the member to add
     * @return index of member in ArrayList
     */
    private int addMember(SwingScilabPushButton member) {
	int res = contentPane.addWidget(member);
	repaint();
	return res;
    }

    /**
     * Remove a PushButton from its container
     * @param member the PushButton to remove
     */
    public void removeMember(PushButton member) {
	this.removeMember((SwingScilabPushButton) member.getAsSimplePushButton());
    }

    /**
     * Remove a PushButton from its container
     * @param member the PushButton to remove
     */
    private void removeMember(SwingScilabPushButton member) {
	contentPane.remove(member);
    }

    /**
     * Add a member (dockable element) to container and returns its index
     * @param member the member to add
     * @return index of member in ArrayList
     */
    public int addMember(EditBox member) {
	return this.addMember((SwingScilabEditBox) member.getAsSimpleEditBox());
    }

    /**
     * Add a member (dockable element) to container and returns its index
     * @param member the member to add
     * @return index of member in ArrayList
     */
    private int addMember(SwingScilabEditBox member) {
	return contentPane.addWidget(member);
    }

    /**
     * Remove an EditBox from its container
     * @param member the EditBox to remove
     */
    public void removeMember(EditBox member) {
	this.removeMember((SwingScilabEditBox) member.getAsSimpleEditBox());
    }

    /**
     * Remove an EditBox from its container
     * @param member the EditBox to remove
     */
    private void removeMember(SwingScilabEditBox member) {
	contentPane.removeWidget(member);
    }

    /**
     * Add a member (dockable element) to container and returns its index
     * @param member the member to add
     * @return index of member in ArrayList
     */
    public int addMember(Label member) {
	return this.addMember((SwingScilabLabel) member.getAsSimpleLabel());
    }

    /**
     * Add a member (dockable element) to container and returns its index
     * @param member the member to add
     * @return index of member in ArrayList
     */
    private int addMember(SwingScilabLabel member) {
	return contentPane.addWidget(member);
    }

    /**
     * Remove a Label from its container
     * @param member the Label to remove
     */
    public void removeMember(Label member) {
	this.removeMember((SwingScilabLabel) member.getAsSimpleLabel());
    }

    /**
     * Remove a Label from its container
     * @param member the Label to remove
     */
    private void removeMember(SwingScilabLabel member) {
	contentPane.removeWidget(member);
    }

    /**
     * Add a member (dockable element) to container and returns its index
     * @param member the member to add
     * @return index of member in ArrayList
     */
    public int addMember(CheckBox member) {
	return this.addMember((SwingScilabCheckBox) member.getAsSimpleCheckBox());
    }

    /**
     * Add a member (dockable element) to container and returns its index
     * @param member the member to add
     * @return index of member in ArrayList
     */
    private int addMember(SwingScilabCheckBox member) {
	return contentPane.addWidget(member);
    }

    /**
     * Remove a CheckBox from its container
     * @param member the CheckBox to remove
     */
    public void removeMember(CheckBox member) {
	this.removeMember((SwingScilabCheckBox) member.getAsSimpleCheckBox());
    }

    /**
     * Remove a CheckBox from its container
     * @param member the CheckBox to remove
     */
    private void removeMember(SwingScilabCheckBox member) {
	contentPane.removeWidget(member);
    }

    /**
     * Add a member (dockable element) to container and returns its index
     * @param member the member to add
     * @return index of member in ArrayList
     */
    public int addMember(RadioButton member) {
	return this.addMember((SwingScilabRadioButton) member.getAsSimpleRadioButton());
    }

    /**
     * Add a member (dockable element) to container and returns its index
     * @param member the member to add
     * @return index of member in ArrayList
     */
    private int addMember(SwingScilabRadioButton member) {
	return contentPane.addWidget(member);
    }

    /**
     * Remove a RadioButton from its container
     * @param member the RadioButton to remove
     */
    public void removeMember(RadioButton member) {
	this.removeMember((SwingScilabRadioButton) member.getAsSimpleRadioButton());
    }

    /**
     * Remove a RadioButton from its container
     * @param member the RadioButton to remove
     */
    private void removeMember(SwingScilabRadioButton member) {
	contentPane.removeWidget(member);
    }

    /**
     * Add a member (dockable element) to container and returns its index
     * @param member the member to add
     * @return index of member in ArrayList
     */
    public int addMember(Slider member) {
	return this.addMember((SwingScilabSlider) member.getAsSimpleSlider());
    }

    /**
     * Add a member (dockable element) to container and returns its index
     * @param member the member to add
     * @return index of member in ArrayList
     */
    private int addMember(SwingScilabSlider member) {
	return contentPane.addWidget(member);
    }

    /**
     * Remove a Slider from its container
     * @param member the Slider to remove
     */
    public void removeMember(Slider member) {
	this.removeMember((SwingScilabSlider) member.getAsSimpleSlider());
    }

    /**
     * Remove a Slider from its container
     * @param member the Slider to remove
     */
    private void removeMember(SwingScilabSlider member) {
	contentPane.removeWidget(member);
    }

    /**
     * Add a member (dockable element) to container and returns its index
     * @param member the member to add
     * @return index of member in ArrayList
     */
    public int addMember(ListBox member) {
	return this.addMember((SwingScilabListBox) member.getAsSimpleListBox());
    }

    /**
     * Add a member (dockable element) to container and returns its index
     * @param member the member to add
     * @return index of member in ArrayList
     */
    private int addMember(SwingScilabListBox member) {
	return contentPane.addWidget(member);
    }

    /**
     * Remove a ListBox from its container
     * @param member the ListBox to remove
     */
    public void removeMember(ListBox member) {
	this.removeMember((SwingScilabListBox) member.getAsSimpleListBox());
    }

    /**
     * Remove a ListBox from its container
     * @param member the ListBox to remove
     */
    private void removeMember(SwingScilabListBox member) {
	contentPane.removeWidget(member);
    }

    /**
     * Add a member (dockable element) to container and returns its index
     * @param member the member to add
     * @return index of member in ArrayList
     */
    public int addMember(PopupMenu member) {
	return this.addMember((SwingScilabPopupMenu) member.getAsSimplePopupMenu());
    }

    /**
     * Add a member (dockable element) to container and returns its index
     * @param member the member to add
     * @return index of member in ArrayList
     */
    private int addMember(SwingScilabPopupMenu member) {
	return contentPane.addWidget(member);
    }

    /**
     * Remove a PopupMenu from its container
     * @param member the PopupMenu to remove
     */
    public void removeMember(PopupMenu member) {
	this.removeMember((SwingScilabPopupMenu) member.getAsSimplePopupMenu());
    }

    /**
     * Remove a PopupMenu from its container
     * @param member the PopupMenu to remove
     */
    private void removeMember(SwingScilabPopupMenu member) {
	contentPane.removeWidget(member);
    }

    /**
     * Add a member (dockable element) to container and returns its index
     * @param member the member to add
     * @return index of member in ArrayList
     */
    public int addMember(Dockable member) {
	// TODO Auto-generated method stub
	return 0;
    }

    /**
     * Get the current status of the Tab in its parent
     * @return true is the tab is the tab currently "on top" in its parent
     */
    public boolean isCurrentTab() {
	// TODO should not always return TRUE
	return true;
    }

    /**
     * Get the parent window id for this tab
     * @return the id of the parent window
     */
    public int getParentWindowId() {
	return this.parentWindowId;
    }

    /**
     * Set the parent window id for this tab
     * @param id the id of the parent window
     */
    public void setParentWindowId(int id) {
	this.parentWindowId = id;
    }

    /**
     * Setter for MenuBar
     * @param newMenuBar : the MenuBar to set.
     * @see org.scilab.modules.gui.tab.SimpleTab#setMenuBar(org.scilab.modules.gui.menubar.MenuBar)
     */
    public void setMenuBar(MenuBar newMenuBar) {
	this.menuBar = newMenuBar;
    }


    /**
     * Getter for MenuBar
     * @return MenuBar : the MenuBar associated to the Tab.
     * @see org.scilab.modules.gui.tab.SimpleTab#getMenuBar()
     */
    public MenuBar getMenuBar() {
	return this.menuBar;
    }

    /**
     * Setter for ToolBar
     * @param newToolBar : the ToolBar to set.
     * @see org.scilab.modules.gui.tab.SimpleTab#setToolBar(org.scilab.modules.gui.toolbar.ToolBar)
     */
    public void setToolBar(ToolBar newToolBar) {
	this.toolBar = newToolBar;
    }

    /**
     * Getter for ToolBar
     * @return ToolBar : the ToolBar associated to the Tab.
     * @see org.scilab.modules.gui.tab.SimpleTab#getToolBar()
     */
    public ToolBar getToolBar() {
	return this.toolBar;
    }

    /**
     * Setter for InfoBar
     * @param newInfoBar the InfoBar to set.
     */
    public void setInfoBar(TextBox newInfoBar) {
	this.infoBar = newInfoBar;
    }

    /**
     * Getter for InfoBar
     * @return the InfoBar associated to the Tab.
     */
    public TextBox getInfoBar() {
	return this.infoBar;
    }

    /**
     * Set the callback of the tab
     * @param callback the callback to set.
     */
    public void setCallback(CallBack callback) {
	callback.putValue(Action.NAME, DockingConstants.CLOSE_ACTION);
	this.addAction(callback);

	/* Undock button */
	SciUndockingAction undockAction = new SciUndockingAction(this);
	undockAction.putValue(Action.NAME, "undock");
	this.addAction(undockAction);
    }

    /**
     * Set this tab as the current tab of its parent Window
     */
    public void setCurrent() {
	super.setActive(true);
    }

    /**
     * Set the background color of the tab.
     * @param red red channel of the color
     * @param green green channel
     * @param blue blue channel
     */
    public void setBackground(double red, double green, double blue) {
	Color newColor = new Color((float) red, (float) green, (float) blue);
	contentPane.setBackground(red, green, blue);
	scrolling.setBackground(red, green, blue);
	setBackground(newColor);
    }

    /**
     * Get the part of the axes which is currently viewed
     * @return [x,y,w,h] array
     */
    public int[] getViewingRegion() {
	return scrolling.getViewingRegion();
    }

    /**
     * Specify a new viewport for the axes
     * For SwingScilabCanvas viewport can not be modified
     * since it match the parent tab size
     * @param posX X coordinate of upper left point of the viewport within the axes
     * @param posY Y coordinate of upper left point of the viewport within the axes
     * @param width width of the viewport
     * @param height height of the viewport
     */
    public void setViewingRegion(int posX, int posY, int width, int height) {
	// Check that the canvas can be resized
	if (!scrolling.getAutoResizeMode()) {
	    // don't set viewport size here it should always fit parent tab size
	    // It seems that we must check the viewport size and positions
	    // to get coherent values, otherwise the setViewPosition hangs...
	    // there are three checks that must be performed for the two dimensions
	    // - be sure that viewport position is greater than 0.
	    // - if the viewport is larger than the canvas, then it can't be moved
	    // - if the viewport is smaller than the canvas, then it should remains
	    //   inside the canvas

	    int canvasWidth = contentPane.getWidth();
	    int canvasHeight = contentPane.getHeight();
	    int[] curViewedRegion = getViewingRegion();
	    int viewportPosX = curViewedRegion[0];
	    int viewPortPosY = curViewedRegion[1];
	    int viewportWidth = curViewedRegion[2];
	    int viewportHeight = curViewedRegion[VIEWPORT_SIZE - 1];

	    // use previous values as default ones
	    int realPosX = 0;
	    int realPosY = 0;


	    if (viewportWidth <= canvasWidth) {
		// viewport smaller than the canvas
		// check that the viewport stays in the canvas
		// the left most position is canvasWidth - viewporwidth
		realPosX = Math.min(posX, canvasWidth - viewportWidth);
	    } else {
		// viewport larger than the canvas
		// get previous position (should be 0)
		realPosX = viewportPosX;
	    }
	    // last check, greater than 0
	    realPosX = Math.max(0, realPosX);

	    if (viewportHeight <= canvasHeight) {
		realPosY = Math.min(posY, canvasHeight - viewportHeight);
	    } else {
		realPosY = viewPortPosY;
	    }
	    realPosY = Math.max(0, realPosY);

	    // must be called on the Swing thread otherwise some JOGL corruption may appear
	    final Point realPos = new Point(realPosX, realPosY);
	    try {
		SwingUtilities.invokeAndWait(new Runnable() {
		    public void run() {
			scrolling.setViewPosition(realPos.x, realPos.y);
		    }
		});
	    } catch (InterruptedException e) {
		e.printStackTrace();
	    } catch (InvocationTargetException e) {
		e.getCause().printStackTrace();
	    }


	}
    }

    /**
     * Set the event handler of the Canvas
     * @param funName the name of the Scilab function to call
     */
    public void setEventHandler(String funName) {
	contentPane.setEventHandler(funName);
    }


    /**
     * Set the status of the event handler of the Canvas
     * @param status is true to set the event handler active
     */
    public void setEventHandlerEnabled(boolean status) {
	contentPane.setEventHandlerEnabled(status);
    }

    /**
     * Specify whether the canvas should fit the parent tab size
     * (and consequently the scrollpane size) or not
     * @param onOrOff true to enable autoresize mode
     */
    public void setAutoResizeMode(boolean onOrOff) {
	scrolling.setAutoResizeMode(onOrOff);
    }

    /**
     * @return whether the resize mode is on or off
     */
    public boolean getAutoResizeMode() {
	return scrolling.getAutoResizeMode();
    }

    /**
     * Get the displacement in pixel that should be used for rotating axes
     * @param displacement out parameter, [x,y] array of displacement in pixels
     * @return true if the displacement recording continue, false otherwise
     */
    public boolean getRotationDisplacement(int[] displacement) {
	return contentPane.getRotationDisplacement(displacement);
    }
	
	/**
	 * Close the tab and disable it.
	 */
	public void close() {
		this.getContentPane().removeAll();
		this.setMenuBar(null);
		this.setToolBar(null);
		this.setInfoBar(null);
		this.setTitlebar(null);
		this.removeAll();
		setActive(false);
		
		scrolling = null;
		contentPane = null;
		
		// without this children canvas are not released.
		Container dummyContainer = new Container();
		this.setContentPane(dummyContainer);
	}

    /**
     * Asynchronous stop of rotation tracking.
     */
    public void stopRotationRecording() {
	contentPane.stopRotationRecording();
    }

    /**
     * Redefine paint children to be sure that AWT components are well painted.
     */
    public void paintChildren(Graphics g) {
	Component[] children = getComponents();
	for (int i = 0; i < children.length; i++) {
	    // AWT children don't draw themselves automatically
	    // so force their draw
	    if (!children[i].isLightweight()) {
		children[i].paint(g);
	    }
	}
	super.paintChildren(g);
    }

}

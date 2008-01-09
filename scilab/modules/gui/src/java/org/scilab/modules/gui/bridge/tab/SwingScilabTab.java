/* Copyright INRIA 2007 */

package org.scilab.modules.gui.bridge.tab;

import org.flexdock.docking.DockingConstants;
import org.flexdock.view.View;
import org.scilab.modules.gui.bridge.canvas.SwingScilabCanvas;
import org.scilab.modules.gui.bridge.console.SwingScilabConsole;
import org.scilab.modules.gui.bridge.editbox.SwingScilabEditBox;
import org.scilab.modules.gui.bridge.frame.SwingScilabFrame;
import org.scilab.modules.gui.bridge.label.SwingScilabLabel;
import org.scilab.modules.gui.bridge.pushbutton.SwingScilabPushButton;
import org.scilab.modules.gui.canvas.Canvas;
import org.scilab.modules.gui.console.Console;
import org.scilab.modules.gui.dockable.Dockable;
import org.scilab.modules.gui.editbox.EditBox;
import org.scilab.modules.gui.frame.Frame;
import org.scilab.modules.gui.label.Label;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.gui.tab.SimpleTab;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.BarUpdater;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

/**
 * Swing implementation for Scilab tabs in GUIs
 * This implementation uses FlexDock package
 * @author Bruno JOFRET
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public class SwingScilabTab extends View implements SimpleTab {
	
	private static final long serialVersionUID = 1L;
	
	private int parentWindowId;
	
	private MenuBar menuBar;
	
	private ToolBar toolBar;

	/**
	 * Constructor
	 * @param name the name of the tab (used to identify it)
	 */
	public SwingScilabTab(String name) {
		super(name, name, name);
		this.addAction(DockingConstants.CLOSE_ACTION);
		// FIXME : Need improvement to be available at a release Status...
		this.addAction(DockingConstants.PIN_ACTION);
		this.addAction(DockingConstants.ACTIVE_WINDOW);
		this.setLayout(null);
	}

    /**
     * Repaint it
     */
	public void repaint() {
		super.repaint();
		BarUpdater.updateBars(getParentWindowId(), getMenuBar(), getToolBar());
	}
	
	/**
	 * Sets the Name of a swing Scilab tab
	 * @param newTabName the Name of the tab
	 * @see org.scilab.modules.gui.tab.Tab#setName()
	 */
	public void setName(String newTabName) {
		setTitle(newTabName, true);
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
	 * Draws a swing Scilab tab
	 * @see org.scilab.modules.gui.uielement.UIElement#draw()
	 */
	public void draw() {
		this.setVisible(true);
		this.doLayout();
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
		// TODO : Check wether we want a Canvas in a Tab or not.
		return this.addMember((SwingScilabCanvas) member.getAsSimpleCanvas());
	}
	
	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	private int addMember(SwingScilabCanvas member) {
		// TODO : Check wether we want a Canvas in a Tab or not.
		this.setContentPane(member);
		return this.getComponentZOrder(member);
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
		this.setContentPane(member);
		return this.getComponentZOrder(member);
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
		this.add(member);
		this.revalidate(); // If do not revalidate then the component do not appear
		return this.getComponentZOrder(member);
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
		this.add(member);
		this.revalidate(); // If do not revalidate then the component do not appear
		return this.getComponentZOrder(member);
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
		this.add(member);
		this.revalidate(); // If do not revalidate then the component do not appear
		return this.getComponentZOrder(member);
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
	
	
}


/* Copyright INRIA 2007 */

package org.scilab.modules.gui.bridge.tab;

import org.flexdock.docking.DockingConstants;
import org.flexdock.view.View;

import org.scilab.modules.gui.bridge.canvas.SwingScilabCanvas;
import org.scilab.modules.gui.bridge.console.SwingScilabConsole;
import org.scilab.modules.gui.bridge.frame.SwingScilabFrame;
import org.scilab.modules.gui.bridge.pushbutton.SwingScilabPushButton;
import org.scilab.modules.gui.console.Console;
import org.scilab.modules.gui.canvas.Canvas;
import org.scilab.modules.gui.dockable.Dockable;
import org.scilab.modules.gui.frame.Frame;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.gui.tab.SimpleTab;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

/**
 * Swing implementation for Scilab tabs in GUIs
 * This implementation uses FlexDock package
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public class SwingScilabTab extends View implements SimpleTab {
	
	private static final long serialVersionUID = 1L;
	
	private int parentWindowId;
	
	private int elementId;

	/**
	 * Constructor
	 * @param name the name of the tab (used to identify it)
	 */
	public SwingScilabTab(String name) {
		super(name, name, name);
		this.addAction(DockingConstants.CLOSE_ACTION);
		this.addAction(DockingConstants.PIN_ACTION);
		this.setLayout(null);
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

//	 TODO : Check wether we want a Canvas in a Tab or not.
	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	public int addMember(Canvas member) {
		return this.addMember((SwingScilabCanvas) member.getAsSimpleCanvas());
	}
	
//	 TODO : Check wether we want a Canvas in a Tab or not.
	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	private int addMember(SwingScilabCanvas member) {
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
	 * Get the element id for this tab
	 * @return id the id of the corresponding tab object
	 */
	public int getElementId() {
		return this.elementId;
	}

	/**
	 * Set the element id for this tab
	 * @param id the id of the corresponding tab object
	 */
	public void setElementId(int id) {
		this.elementId = id;
	}
}


/* Copyright INRIA 2007 */

package org.scilab.modules.gui.bridge;

import org.flexdock.docking.DockingConstants;
import org.flexdock.view.View;

import org.scilab.modules.gui.console.Console;
import org.scilab.modules.gui.canvas.Canvas;
import org.scilab.modules.gui.dockable.Dockable;
import org.scilab.modules.gui.frame.Frame;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

/**
 * Swing implementation for Scilab tabs in GUIs
 * This implementation uses FlexDock package
 * @author Vincent COUVERT
 */
public class SwingScilabTab extends View implements Tab {

	/**
	 * Constructor
	 * @param name the name of the tab (used to identify it)
	 */
	public SwingScilabTab(String name) {
		super(name, name, name);
		this.addAction(DockingConstants.CLOSE_ACTION);
		this.addAction(DockingConstants.PIN_ACTION);
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
	 * @see org.scilab.modules.gui.UIElement#draw()
	 */
	public void draw() {
		this.setVisible(true);
		this.doLayout();
	}

	/**
	 * Gets the dimensions (width and height) of a swing Scilab tab
	 * @return the dimensions of the frame
	 * @see org.scilab.modules.gui.UIElement#getDims()
	 */
	public Size getDims() {
		return new Size(this.getSize().width, this.getSize().height);
	}

	/**
	 * Gets the position (X-coordinate and Y-coordinate) of a swing Scilab tab
	 * @return the position of the tab
	 * @see org.scilab.modules.gui.UIElement#getPosition()
	 */
	public Position getPosition() {
		return new Position(this.getX(), this.getY());
	}

	/**
	 * Sets the dimensions (width and height) of a swing Scilab tab
	 * @param newSize the dimensions we want to set to the tab
	 * @see org.scilab.modules.gui.UIElement#setDims(org.scilab.modules.gui.utils.Size)
	 */
	public void setDims(Size newSize) {
		this.setSize(newSize.getWidth(), newSize.getHeight());
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a swing Scilab tab
	 * @param newPosition the position we want to set to the tab
	 * @see org.scilab.modules.gui.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
	 */
	public void setPosition(Position newPosition) {
		this.setLocation(newPosition.getX(), newPosition.getY());
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
	private int addMember(SwingScilabConsole member) {
		this.setContentPane(member);
		return this.getComponentZOrder(member);
	}

	// TODO : Check wether we want a Canvas in a Tab or not.

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
	public int addMember(Frame member) {
		return this.addMember((SwingScilabFrame) member);
	}

	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	public int addMember(Console member) {
		return this.addMember((SwingScilabConsole) member);
	}

	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	public int addMember(Canvas member) {
		return this.addMember((SwingScilabCanvas) member);
	}

	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	public int addMember(Dockable member) {
		// TODO : Must manage this with Exceptions.
		System.out.println("[SwingScilabTab.addMember(Dockable)] : Must not be there !!!");
		return -1;
	}
}

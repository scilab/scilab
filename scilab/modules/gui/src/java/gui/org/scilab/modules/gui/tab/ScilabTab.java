
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.tab;

import org.scilab.modules.console.Console;
import org.scilab.modules.gui.ScilabBridge;
import org.scilab.modules.gui.canvas.Canvas;
import org.scilab.modules.gui.container.ScilabContainer;
import org.scilab.modules.gui.dockable.Dockable;
import org.scilab.modules.gui.frame.Frame;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

/**
 * Class for tabs is Scilab GUIs
 * @author Vincent COUVERT
 */
public class ScilabTab extends ScilabContainer implements Tab {

	/**
	 * Constructor
	 */
	protected ScilabTab() {
        throw new UnsupportedOperationException(); /* Prevents calls from subclass */
    }

	/**
	 * Creates a Scilab Tab
	 * @param name the name of the tab (used to identify it)
	 * @return the created Scilab Tab
	 */
	public static Tab createTab(String name) {
		return ScilabBridge.createTab(name);
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
	public int addMember(Frame member) {
		return ScilabBridge.addMember(this, member);
	}

	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	public int addMember(Console member) {
		return ScilabBridge.addMember(this, member);
	}

	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	public int addMember(Canvas member) {
		return ScilabBridge.addMember(this, member);
	}

	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 * @see org.scilab.modules.gui.container.Container#addMember(org.scilab.modules.gui.dockable.Dockable)
	 */
	public int addMember(Dockable member) {
		return addMember(member);
	}

}

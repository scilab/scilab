
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.frame;

import org.scilab.modules.gui.console.Console;
import org.scilab.modules.gui.bridge.ScilabBridge;
import org.scilab.modules.gui.canvas.Canvas;
import org.scilab.modules.gui.container.ScilabContainer;
import org.scilab.modules.gui.dockable.Dockable;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

/**
 * Class for frames in Scilab GUIs
 * @author Vincent COUVERT
 */
public class ScilabFrame extends ScilabContainer implements Frame {

	/**
	 * Constructor
	 */
	protected ScilabFrame() {
        throw new UnsupportedOperationException(); /* Prevents calls from subclass */
    }

	/**
	 * Creates a Scilab Frame
	 * @return the created Scilab FRame
	 */
	public static Frame createFrame() {
		return ScilabBridge.createFrame();
	}

	/**
	 * Draws a frame
	 * @see org.scilab.modules.gui.UIElement#draw()
	 */
	public void draw() {
		ScilabBridge.draw(this);
	}

	/**
	 * Gets the dimensions (width and height) of a Scilab frame
	 * @return the size of the frame
	 * @see org.scilab.modules.gui.UIElement#getDims()
	 */
	public Size getDims() {
		return ScilabBridge.getDims(this);
	}

	/**
	 * Gets the position (X-coordinate and Y-coordinates) of a Scilab frame
	 * @return the position of the frame
	 * @see org.scilab.modules.gui.UIElement#getPosition()
	 */
	public Position getPosition() {
		return ScilabBridge.getPosition(this);
	}

	/**
	 * Gets the visibility status of a frame
	 * @return the visibility status of the frame (true if the frame is visible, false if not)
	 * @see org.scilab.modules.gui.UIElement#isVisible()
	 */
	public boolean isVisible() {
		return ScilabBridge.isVisible(this);
	}

	/**
	 * Sets the dimensions (width and height) of a Scilab frame
	 * @param newFrameSize the size we want to set to the frame
	 * @see org.scilab.modules.gui.UIElement#setDims(org.scilab.modules.gui.utils.Size)
	 */
	public void setDims(Size newFrameSize) {
		ScilabBridge.setDims(this, newFrameSize);
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a Scilab frame
	 * @param newFramePosition the position we want to set to the frame
	 * @see org.scilab.modules.gui.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
	 */
	public void setPosition(Position newFramePosition) {
		ScilabBridge.setPosition(this, newFramePosition);
	}

	/**
	 * Sets the visibility status of a Scilab frame
	 * @param newVisibleState the visibility status we want to set to the frame
	 * @see org.scilab.modules.gui.UIElement#setVisible(boolean)
	 */
	public void setVisible(boolean newVisibleState) {
		ScilabBridge.setVisible(this, newVisibleState);
	}

	/**
	 * Add a member (dockable element) to a Scilab frame and returns its index
	 * @param member the member to add
	 * @return index of member in frame components
	 */
	public int addMember(Frame member) {
		return ScilabBridge.addMember(this, member);
	}

	/**
	 * Add a member (dockable element) to a Scilab frame and returns its index
	 * @param member the member to add
	 * @return index of member in frame components
	 */
	public int addMember(Console member) {
		//return ScilabBridge.addMember(this, member);
		return 0;
	}

	/**
	 * Add a member (dockable element) to a Scilab frame and returns its index
	 * @param member the member to add
	 * @return index of member in frame components
	 */
	public int addMember(Canvas member) {
		return ScilabBridge.addMember(this, member);
	}

	/**
	 * Add a member (dockable element) to a Scilab frame and returns its index
	 * @param member the member to add
	 * @return index of member in frame components
	 * @see org.scilab.modules.gui.container.Container#addMember(org.scilab.modules.gui.dockable.Dockable)
	 */
	public int addMember(Dockable member) {
		return addMember(member);
	}
}

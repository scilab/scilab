
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.bridge.frame;

import javax.swing.JPanel;

import org.scilab.modules.gui.bridge.canvas.SwingScilabCanvas;
import org.scilab.modules.gui.bridge.pushbutton.SwingScilabPushButton;
import org.scilab.modules.gui.canvas.Canvas;
import org.scilab.modules.gui.dockable.Dockable;
import org.scilab.modules.gui.frame.Frame;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

/**
 * Swing implementation for Scilab frames in GUI
 * @author Vincent COUVERT
 */
public class SwingScilabFrame extends JPanel implements Frame {

	/**
	 * Constructor
	 */
	public SwingScilabFrame() {
		super();
	}

	/**
	 * Draws a Swing Scilab frame
	 * @see org.scilab.modules.gui.UIElement#draw()
	 */
	public void draw() {
		this.setVisible(true);
		this.doLayout();
	}

	/**
	 * Gets the dimensions (width and height) of a swing Scilab frame
	 * @return the dimension of the frame
	 * @see org.scilab.modules.gui.UIElement#getDims()
	 */
	public Size getDims() {
		return new Size(this.getSize().width, this.getSize().height);
	}

	/**
	 * Gets the position (X-coordinate and Y-coordinate) of a swing Scilab frame
	 * @return the position of the frame
	 * @see org.scilab.modules.gui.UIElement#getPosition()
	 */
	public Position getPosition() {
		return new Position(this.getX(), this.getY());
	}

	/**
	 * Sets the dimensions (width and height) of a swing Scilab frame
	 * @param newSize the size we want to set to the frame
	 * @see org.scilab.modules.gui.UIElement#setDims(org.scilab.modules.gui.utils.Size)
	 */
	public void setDims(Size newSize) {
		this.setSize(newSize.getWidth(), newSize.getHeight());
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a swing Scilab frame
	 * @param newPosition the position we want to set to the frame
	 * @see org.scilab.modules.gui.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
	 */
	public void setPosition(Position newPosition) {
		this.setLocation(newPosition.getX(), newPosition.getY());
	}

	// TODO : Check wether we want a Console in a Frame or not.

//	/**
//	 * Add a member (dockable element) to container and returns its index
//	 * @param member the member to add
//	 * @return index of member in ArrayList
//	 */
//	private int addMember(SwingScilabConsole member) {
//		this.add(member);
//		return this.getComponentZOrder(member);
//	}

	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	private int addMember(SwingScilabFrame member) {
		this.add(member);
		return this.getComponentZOrder(member);
	}

	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	private int addMember(SwingScilabCanvas member) {
		this.add(member);
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
		System.out.println("[SwingScilabFrame.addMember(Dockable)] : Must not be there !!!");
		return -1;
	}
	
	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	public int addMember(PushButton member) {
		return this.addMember((SwingScilabPushButton) member);
	}
	
	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	private int addMember(SwingScilabPushButton member) {
		this.add(member);
		return this.getComponentZOrder(member);
	}
}


/* Copyright INRIA 2007 */

package org.scilab.modules.gui.bridge.frame;

import java.awt.Color;

import javax.swing.BoxLayout;
import javax.swing.JPanel;
import javax.swing.border.LineBorder;

import org.scilab.modules.gui.bridge.canvas.SwingScilabCanvas;
import org.scilab.modules.gui.bridge.pushbutton.SwingScilabPushButton;
import org.scilab.modules.gui.bridge.tab.SwingScilabTab;
import org.scilab.modules.gui.canvas.Canvas;
import org.scilab.modules.gui.dockable.Dockable;
import org.scilab.modules.gui.frame.Frame;
import org.scilab.modules.gui.layout.LayoutManager;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.gui.tab.Tab;
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
	public int addMember(Frame member) {
		return this.addMember((SwingScilabFrame) member);
	}

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
	 * Add a Frame in a Frame with a BorderLayout.
	 * @param member the member to add
	 * @param borderLayout the BorderLayout to use
	 * @return the position of the Frame in the member list.
	 */
	public int addMember(Frame member, String borderLayout) {
		return this.addMember((SwingScilabFrame) member, borderLayout);
	}
	
	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @param borderLayout the BorderLayout to use
	 * @return index of member in ArrayList
	 */
	private int addMember(SwingScilabFrame member, String borderLayout) {
		this.add(member, borderLayout);
		return this.getComponentZOrder(member);
	}
	
	/**
	 * Add a Frame in a Frame with a layout.
	 * @param member the member to add
	 * @param layoutPosition the Layout position to use
	 * @return the position of the Frame in the member list.
	 */
	public int addMember(Frame member, int layoutPosition) {
		return this.addMember((SwingScilabFrame) member, layoutPosition);
	}
	
	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @param layoutPosition the Layout position to use
	 * @return index of member in ArrayList
	 */
	private int addMember(SwingScilabFrame member, int layoutPosition) {
		this.add(member, layoutPosition);
		return this.getComponentZOrder(member);
	}
	
	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	public int addMember(Canvas member) {
		//System.out.println("Test: start: public int addMember(Canvas member)");
		return this.addMember((SwingScilabCanvas) member);
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
	public int addMember(PushButton member) {
		//System.out.println("Test: start: public int addMember(PushButton member)");
		return this.addMember((SwingScilabPushButton) member);
	}
	
	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	private int addMember(SwingScilabPushButton member) {
		//System.out.println("Test: start: public int addMember(SwingScilabPushButton member)");
		this.add(member);
		return this.getComponentZOrder(member);
	}

	/**
	 * Add a PushButton in a Frame with a BorderLayout.
	 * @param member the PushButton to add
	 * @param borderLayout the BorderLayout to use
	 * @return the position of the PushButton in the member list.
	 */
	public int addMember(PushButton member, String borderLayout) {
		return this.addMember((SwingScilabPushButton) member, borderLayout);
	}
	
	/**
	 * Add a PushButton (dockable element) to Frame and returns its index
	 * @param member the PushButton to add
	 * @param layoutPosition the layout Position to use
	 * @return index of member in ArrayList
	 */
	private int addMember(SwingScilabPushButton member, int layoutPosition) {
		this.add(member, layoutPosition);
		return this.getComponentZOrder(member);
	}
	
	/**
	 * Add a PushButton in a Frame with a layout.
	 * @param member the PushButton to add
	 * @param layoutPosition the layout Position to use
	 * @return the position of the PushButton in the member list.
	 */
	public int addMember(PushButton member, int layoutPosition) {
		return this.addMember((SwingScilabPushButton) member, layoutPosition);
	}
	
	/**
	 * Add a PushButton (dockable element) to Frame and returns its index
	 * @param member the PushButton to add
	 * @param borderLayout the BorderLayout to use
	 * @return index of member in ArrayList
	 */
	private int addMember(SwingScilabPushButton member, String borderLayout) {
		this.add(member, borderLayout);
		return this.getComponentZOrder(member);
	}
	
	// TODO : check if possible
	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
//	public int addMember(Tab member) {
//		//System.out.println("Test: start: public int addMember(Tab member)");
//		return this.addMember((SwingScilabTab) member);
//	}
	
	// TODO : check if possible
	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
//	private int addMember(SwingScilabTab member) {
//		//System.out.println("Test: start: public int addMember(SwingScilabTab member)");
//		this.add(member);
//		return this.getComponentZOrder(member);
//	}
	
	
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
	 * To set the Background color of the element.
	 * @param color the Color
	 */
//	public void setBackground(Color color) {
//		super.setBackground(color);
//	}
	
	/**
	 * To set the Border color and size of the element.
	 * @param lineBorder the LineBorder
	 */
	public void setBorder(LineBorder lineBorder) {
		super.setBorder(lineBorder);
	}
	
	/**
	 * To set the Layout of the element.
	 * @param layout the layout
	 */
	public void setLayout(LayoutManager layout) {
		super.setLayout((java.awt.LayoutManager) layout);
	}
}

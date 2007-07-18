
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.bridge.frame;

import javax.swing.JPanel;
import javax.swing.border.LineBorder;

import org.scilab.modules.gui.bridge.canvas.SwingScilabCanvas;
import org.scilab.modules.gui.bridge.checkbox.SwingScilabCheckBox;
import org.scilab.modules.gui.bridge.console.SwingScilabConsole;
import org.scilab.modules.gui.bridge.editbox.SwingScilabEditBox;
import org.scilab.modules.gui.bridge.label.SwingScilabLabel;
import org.scilab.modules.gui.bridge.listbox.SwingScilabListBox;
import org.scilab.modules.gui.bridge.pushbutton.SwingScilabPushButton;
import org.scilab.modules.gui.bridge.radiobutton.SwingScilabRadioButton;
import org.scilab.modules.gui.bridge.slider.SwingScilabSlider;
import org.scilab.modules.gui.bridge.tab.SwingScilabTab;
import org.scilab.modules.gui.bridge.textbox.SwingScilabTextBox;
import org.scilab.modules.gui.canvas.Canvas;
import org.scilab.modules.gui.checkbox.CheckBox;
import org.scilab.modules.gui.console.Console;
import org.scilab.modules.gui.container.Container;
import org.scilab.modules.gui.editbox.EditBox;
import org.scilab.modules.gui.frame.Frame;
import org.scilab.modules.gui.frame.SimpleFrame;
import org.scilab.modules.gui.label.Label;
import org.scilab.modules.gui.layout.LayoutManager;
import org.scilab.modules.gui.listbox.ListBox;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.gui.radiobutton.RadioButton;
import org.scilab.modules.gui.slider.Slider;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

/**
 * Swing implementation for Scilab frames in GUI
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public class SwingScilabFrame extends JPanel implements SimpleFrame {

	/**
	 * Constructor
	 */
	public SwingScilabFrame() {
		super();
		// the Default layout is null so we have to set a Position and a Size of every Dockable we add to it
		this.setLayout((LayoutManager) null);
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
	private int addMember(SwingScilabConsole member) {
		this.add(member);
		return this.getComponentZOrder(member);
	}

	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	public int addMember(EditBox member) {
		return this.addMember((SwingScilabEditBox) member);
	}
	
	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	private int addMember(SwingScilabEditBox member) {
		this.add(member);
		return this.getComponentZOrder(member);
	}
	
	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	public int addMember(Label member) {
		return this.addMember((SwingScilabLabel) member);
	}

	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	private int addMember(SwingScilabLabel member) {
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
	public int addMember(CheckBox member) {
		return this.addMember((SwingScilabCheckBox) member);
	}

	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	private int addMember(SwingScilabCheckBox member) {
		this.add(member);
		return this.getComponentZOrder(member);
	}
	
	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	public int addMember(ListBox member) {
		return this.addMember((SwingScilabListBox) member);
	}

	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	private int addMember(SwingScilabListBox member) {
		this.add(member.getScrollPane());
		return this.getComponentZOrder(member.getScrollPane());
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
	
	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	public int addMember(RadioButton member) {
		return this.addMember((SwingScilabRadioButton) member);
	}

	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	private int addMember(SwingScilabRadioButton member) {
		this.add(member);
		return this.getComponentZOrder(member);
	}
	
	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	public int addMember(Slider member) {
		return this.addMember((SwingScilabSlider) member);
	}

	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	private int addMember(SwingScilabSlider member) {
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
	
//	 TODO : Check if it should be possible to add a Tab to a frame and how it should behave
	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	public int addMember(Tab member) {
		return this.addMember((SwingScilabTab) member);
	}
//	 TODO : Check if it should be possible to add a Tab to a frame and how it should behave
	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	private int addMember(SwingScilabTab member) {
		this.add(member);
		return this.getComponentZOrder(member);
	}
	
	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	public int addMember(TextBox member) {
		return this.addMember((SwingScilabTextBox) member);
	}

	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	private int addMember(SwingScilabTextBox member) {
		this.add(member);
		return this.getComponentZOrder(member);
	}
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
	
	/**
	 * Add this as member (dockable element) to the Scilab container and returns its index
	 * @param container the container in which we add this
	 * @return index of this in container components
	 * @see org.scilab.modules.gui.container.Container#addMember(org.scilab.modules.gui.dockable.Dockable)
	 * @see org.scilab.modules.gui.dockable.Dockable#addAsMemberTo(org.scilab.modules.gui.container.Container)
	 */
	public int addAsMemberTo(Container container) {
		// delegate to the container but also adding info on how to handle me (Frame)
		// Interface Container must describe methode: int addMember(Frame member);
		return container.addMember((Frame) this);
	}
}

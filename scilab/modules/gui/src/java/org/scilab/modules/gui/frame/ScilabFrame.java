
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.frame;

import org.scilab.modules.gui.bridge.ScilabBridge;
import org.scilab.modules.gui.canvas.Canvas;
import org.scilab.modules.gui.checkbox.CheckBox;
import org.scilab.modules.gui.console.Console;
import org.scilab.modules.gui.container.Container;
import org.scilab.modules.gui.container.ScilabContainer;
import org.scilab.modules.gui.dockable.Dockable;
import org.scilab.modules.gui.editbox.EditBox;
import org.scilab.modules.gui.label.Label;
import org.scilab.modules.gui.listbox.ListBox;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.gui.radiobutton.RadioButton;
import org.scilab.modules.gui.slider.Slider;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

/**
 * Class for frames in Scilab GUIs
 * @author Vincent COUVERT
 */
public class ScilabFrame extends ScilabContainer implements Frame {

	private SimpleFrame component;
	
	/**
	 * Constructor
	 */
	protected ScilabFrame() {
		component = ScilabBridge.createFrame();
    }

	/**
	 * Creates a Scilab Frame
	 * @return the created Scilab FRame
	 */
	public static Frame createFrame() {
		return new ScilabFrame();
	}

	/**
	 * Gets this Bridge component object
	 * @return this Bridge component object
	 */
	public SimpleFrame getAsSimpleFrame() {
		return component;
	}
	
	/**
	 * Sets a MenuBar to an element
	 * @param newMenuBar the MenuBar to set to the element
	 */
	public void addMenuBar(MenuBar newMenuBar) {
		// TODO Auto-generated method stub
		throw new UnsupportedOperationException();
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
	 * Add this as member (dockable element) to the Scilab Container and returns its index
	 * @param container the Container in which we add this
	 * @return index of this in container components
	 */
	public int addAsMemberTo(Container container) {
//		 TODO code this
		//return ScilabBridge.addAsMemberTo(this, container);
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
	 */
	public int addMember(CheckBox member) {
//		 TODO code this
		//return ScilabBridge.addMember(this, member);
		return 0;
	}
	
	/**
	 * Add a member (dockable element) to a Scilab frame and returns its index
	 * @param member the member to add
	 * @return index of member in frame components
	 */
	public int addMember(Console member) {
		// TODO code this
		//return ScilabBridge.addMember(this, member);
		return 0;
	}

	/**
	 * Add a member (dockable element) to a Scilab frame and returns its index
	 * @param member the member to add
	 * @return index of member in frame components
	 */
	public int addMember(Dockable member) {
//		 TODO code this
		//return ScilabBridge.addMember(this, member);
		return 0;
	}

	/**
	 * Add a member (dockable element) to a Scilab frame and returns its index
	 * @param member the member to add
	 * @return index of member in frame components
	 */
	public int addMember(EditBox member) {
//		 TODO code this
		//return ScilabBridge.addMember(this, member);
		return 0;
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
	public int addMember(Label member) {
//		 TODO code this
		//return ScilabBridge.addMember(this, member);
		return 0;
	}
	
	/**
	 * Add a member (dockable element) to a Scilab frame and returns its index
	 * @param member the member to add
	 * @return index of member in frame components
	 */
	public int addMember(ListBox member) {
//		 TODO code this
		//return ScilabBridge.addMember(this, member);
		return 0;
	}

	/**
	 * Add a member (dockable element) to a Scilab frame and returns its index
	 * @param member the member to add
	 * @return index of member in frame components
	 */
	public int addMember(PushButton member) {
		// TODO Auto-generated method stub
		return 0;
	}
	
	/**
	 * Add a member (dockable element) to a Scilab frame and returns its index
	 * @param member the member to add
	 * @return index of member in frame components
	 */
	public int addMember(RadioButton member) {
		// TODO Auto-generated method stub
		return 0;
	}
	
	/**
	 * Add a member (dockable element) to a Scilab frame and returns its index
	 * @param member the member to add
	 * @return index of member in frame components
	 */
	public int addMember(Slider member) {
//		 TODO code this
		//return ScilabBridge.addMember(this, member);
		return 0;
	}

	/**
	 * Add a member (dockable element) to a Scilab frame and returns its index
	 * @param member the member to add
	 * @return index of member in frame components
	 */
	public int addMember(Tab member) {
//		 TODO code this
		//return ScilabBridge.addMember(this, member);
		return 0;
	}
	
	/**
	 * Add a member (dockable element) to a Scilab frame and returns its index
	 * @param member the member to add
	 * @return index of member in frame components
	 */
	public int addMember(TextBox member) {
//		 TODO code this
		//return ScilabBridge.addMember(this, member);
		return 0;
	}

	/**
	 * WE ABSOLUTELY WANT TO AVOID
	 * USE OF ANY GUI IMPLEMENTATION OBJECTS !!!!
	 * 
	 * THE AIM IS NOT TO MIRROR THE IMPLEMENTATION !!!!
	 */

}

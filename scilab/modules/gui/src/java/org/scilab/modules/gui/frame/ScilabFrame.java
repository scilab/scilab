
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.frame;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;

import javax.swing.border.LineBorder;

import org.scilab.modules.gui.console.Console;
import org.scilab.modules.gui.bridge.ScilabBridge;
import org.scilab.modules.gui.canvas.Canvas;
import org.scilab.modules.gui.container.ScilabContainer;
import org.scilab.modules.gui.dockable.Dockable;
import org.scilab.modules.gui.layout.LayoutManager;
import org.scilab.modules.gui.pushbutton.PushButton;
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
		// TODO code this
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
	 */
	public int addMember(PushButton member) {
		// TODO Auto-generated method stub
		return 0;
	}

	/**
	 * Add a member (dockable element) to a Scilab frame and returns its index
	 * @param member the member to add
	 * @param borderLayout the BorderLayout to use
	 * @return index of member in frame components
	 */
	public int addMember(PushButton member, String borderLayout) {
		// TODO Auto-generated method stub
		return 0;
	}

	/**
	 * Add a member (dockable element) to a Scilab frame and returns its index
	 * @param member the member to add
	 * @param flowLayoutPosition the flowLayout Position to use
	 * @return index of member in frame components
	 */
	public int addMember(PushButton member, int flowLayoutPosition) {
		// TODO Auto-generated method stub
		return 0;
	}

	/**
	 * Add a member (dockable element) to a Scilab frame and returns its index
	 * @param member the member to add
	 * @param borderLayout the BorderLayout to use
	 * @return index of member in frame components
	 */
	public int addMember(Frame member, String borderLayout) {
		// TODO Auto-generated method stub
		return 0;
	}

	/**
	 * Add a member (dockable element) to a Scilab frame and returns its index
	 * @param member the member to add
	 * @param flowLayoutPosition the flowLayout Position to use
	 * @return index of member in frame components
	 */
	public int addMember(Frame member, int flowLayoutPosition) {
		// TODO Auto-generated method stub
		return 0;
	}

	/**
	 * To get the Background color of the element.
	 * @return color the Color
	 */
	public Color getBackground() {
		// TODO Auto-generated method stub
		return null;
	}

	/**
	 * To get the Font of the element.
	 * @return font the Font
	 */
	public Font getFont() {
		// TODO Auto-generated method stub
		return null;
	}

	/**
	 * To get the Foreground color of the element.
	 * @return color the Color
	 */
	public Color getForeground() {
		// TODO Auto-generated method stub
		return null;
	}
	
	/**
	 * If the maximum size has been set to a non-null value just returns it.
	 * If the UI delegate's getMaximumSize  method returns a non-null value then return that;
	 * otherwise defer to the component's layout manager.
	 * @return the maximum size
	 */
	public Dimension getMaximumSize() {
		// TODO Auto-generated method stub
		return null;
	}
	
	/**
	 * If the minimum size has been set to a non-null value just returns it.
	 * If the UI delegate's getMaximumSize  method returns a non-null value then return that;
	 * otherwise defer to the component's layout manager.
	 * @return the minimum size
	 */
	public Dimension getMinimumSize() {
		// TODO Auto-generated method stub
		return null;
	}
	
	/**
	 * If the preferredSize has been set to a non-null value just returns it. If the UI delegate's getPreferredSize
	 *  method returns a non null value then return that; otherwise defer to the component's layout manager.
	 * @return the preferredSize
	 */
	public Dimension getPreferredSize() {
		// TODO Auto-generated method stub
		return null;
	}
	
	/**
	 * To get the Dimension of the element.
	 * @return the Dimension
	 */
	public Dimension getSize() {
		// TODO Auto-generated method stub
		return null;
	}
	
	/**
	 * To get the x coordinate  of the element.
	 * @return the x coordinate
	 */
	public int getX() {
		// TODO Auto-generated method stub
		return 0;
	}

	/**
	 * To get the y coordinate  of the element.
	 * @return the y coordinate
	 */
	public int getY() {
		// TODO Auto-generated method stub
		return 0;
	}

	/**
	 * To set the Background color of the element.
	 * @param color the Color
	 */
	public void setBackground(Color color) {
		// TODO Auto-generated method stub
		
	}

	/**
	 * To set the Border color and size of the element.
	 * @param lineBorder the LineBorder
	 */
	public void setBorder(LineBorder lineBorder) {
		// TODO Auto-generated method stub
		
	}

	/**
	 * To set the Font of the element.
	 * @param font the Font
	 */
	public void setFont(Font font) {
		// TODO Auto-generated method stub
		
	}

	/**
	 * To set the Foreground color of the element.
	 * @param color the Color
	 */
	public void setForeground(Color color) {
		// TODO Auto-generated method stub
		
	}
	
	/**
	 * To set the Layout of the element.
	 * @param layout the layout
	 */
	public void setLayout(LayoutManager layout) {
		// TODO Auto-generated method stub
		
	}
	
	/**
	 * Moves this component to a new location. The top-left corner of the new location is specified by the x and y 
	 * parameters in the coordinate space of this component's parent.
	 * @param x - the x-coordinate of the new location's top-left corner in the parent's coordinate space
	 * @param y - the y-coordinate of the new location's top-left corner in the parent's coordinate space
	 */
	public void setLocation(int x, int y) {
		// TODO Auto-generated method stub
		
	}

	/**
	 * Sets the maximum size of the element.
	 * @param dimension the Dimension
	 */
	public void setMaximumSize(Dimension dimension) {
		// TODO Auto-generated method stub
		
	}
	
	/**
	 * Sets the minimum size of the element.
	 * @param dimension the Dimension
	 */
	public void setMinimumSize(Dimension dimension) {
		// TODO Auto-generated method stub
		
	}
	
	/**
	 * Sets the preferred size of the element.
	 * @param dimension the Dimension
	 */
	public void setPreferredSize(Dimension dimension) {
		// TODO Auto-generated method stub
		
	}

	/**
	 * To set the Dimension of the element.
	 * @param dimension the Dimension
	 */
	public void setSize(Dimension dimension) {
		// TODO Auto-generated method stub
		
	}

}


/* Copyright INRIA 2007 */

package org.scilab.modules.gui.bridge.checkbox;

import javax.swing.JCheckBox;
import javax.swing.border.LineBorder;

import org.scilab.modules.gui.checkbox.CheckBox;
import org.scilab.modules.gui.container.Container;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

/**
 * Swing implementation for Scilab CheckBox in GUIs
 * @author Marouane BEN JELLOUL
 */
public class SwingScilabCheckBox extends JCheckBox implements CheckBox {

	/**
	 * Constructor
	 */
	public SwingScilabCheckBox() {
		super();
	}
	
	/**
	 * To set the Border color and size of the element.
	 * @param lineBorder the LineBorder
	 */
	public void setBorder(LineBorder lineBorder) {
		super.setBorder(lineBorder);
	}

	/**
	 * Draws a swing Scilab CheckBox
	 * @see org.scilab.modules.gui.UIElement#draw()
	 */
	public void draw() {
		this.setVisible(true);
		this.doLayout();
	}

	/**
	 * Gets the dimensions (width and height) of a swing Scilab CheckBox
	 * @return the dimensions of the CheckBox
	 * @see org.scilab.modules.gui.uielement.UIElement#getDims()
	 */
	public Size getDims() {
		return new Size(this.getSize().width, this.getSize().height);
	}

	/**
	 * Gets the position (X-coordinate and Y-coordinate) of a swing Scilab CheckBox
	 * @return the position of the CheckBox
	 * @see org.scilab.modules.gui.uielement.UIElement#getPosition()
	 */
	public Position getPosition() {
		return new Position(this.getX(), this.getY());
	}

	/**
	 * Sets the dimensions (width and height) of a swing Scilab CheckBox
	 * @param newSize the dimensions we want to set to the CheckBox
	 * @see org.scilab.modules.gui.uielement.UIElement#setDims(org.scilab.modules.gui.utils.Size)
	 */
	public void setDims(Size newSize) {
		this.setSize(newSize.getWidth(), newSize.getHeight());		
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a swing Scilab CheckBox
	 * @param newPosition the position we want to set to the CheckBox
	 * @see org.scilab.modules.gui.uielement.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
	 */
	public void setPosition(Position newPosition) {
		this.setLocation(newPosition.getX(), newPosition.getY());
	}

	/**
	 * Add this as member (dockable element) to the Scilab container and returns its index
	 * @param container the container in which we add this
	 * @return index of this in container components
	 * @see org.scilab.modules.gui.container.Container#addMember(org.scilab.modules.gui.dockable.Dockable)
	 * @see org.scilab.modules.gui.dockable.Dockable#addAsMemberTo(org.scilab.modules.gui.container.Container)
	 */
	public int addAsMemberTo(Container container) {
		// delegate to the container but also adding info on how to handle me (CheckBox)
		// Interface Container must describe methode: int addMember(CheckBox member);
		return container.addMember((CheckBox) this);
	}
}

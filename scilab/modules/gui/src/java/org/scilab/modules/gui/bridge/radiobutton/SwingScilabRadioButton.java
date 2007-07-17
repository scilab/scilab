
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.bridge.radiobutton;

import javax.swing.JRadioButton;
import javax.swing.border.LineBorder;

import org.scilab.modules.gui.container.Container;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.gui.radiobutton.RadioButton;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

/**
 * Swing implementation for Scilab RadioButton in GUIs
 * @author Marouane BEN JELLOUL
 */
public class SwingScilabRadioButton extends JRadioButton implements RadioButton {

	/**
	 * Constructor
	 */
	public SwingScilabRadioButton() {
		super();
	}
	
	/**
	 * 
	 */
	public void draw() {
		this.setVisible(true);
		this.doLayout();
	}
	
	/**
	 * Gets the dimensions (width and height) of a swing Scilab RadioButton
	 * @return the dimensions of the RadioButton
	 * @see org.scilab.modules.gui.uielement.UIElement#getDims()
	 */
	public Size getDims() {
		return new Size(super.getSize().width, super.getSize().height);
	}

	/**
	 * Gets the position (X-coordinate and Y-coordinate) of a swing Scilab RadioButton
	 * @return the position of the RadioButton
	 * @see org.scilab.modules.gui.uielement.UIElement#getPosition()
	 */
	public Position getPosition() {
		return new Position(super.getX(), super.getY());
	}
	
	/**
	 * Sets the dimensions (width and height) of a swing Scilab RadioButton
	 * @param newSize the dimensions to set to the RadioButton
	 * @see org.scilab.modules.gui.uielement.UIElement#setDims(org.scilab.modules.gui.utils.Size)
	 */
	public void setDims(Size newSize) {
		super.setSize(newSize.getWidth(), newSize.getHeight());
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a swing Scilab RadioButton
	 * @param newPosition the position to set to the RadioButton
	 * @see org.scilab.modules.gui.uielement.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
	 */
	public void setPosition(Position newPosition) {
		this.setLocation(newPosition.getX(), newPosition.getY());
	}
	
	/**
	 * To set the Border color and size of the element.
	 * @param lineBorder the LineBorder
	 */
	public void setBorder(LineBorder lineBorder) {
		super.setBorder(lineBorder);
	}
	
	/**
	 * Add this as member (dockable element) to the Scilab container and returns its index
	 * @param container the container in which we add this
	 * @return index of this in container components
	 * @see org.scilab.modules.gui.container.Container#addMember(org.scilab.modules.gui.dockable.Dockable)
	 * @see org.scilab.modules.gui.dockable.Dockable#addAsMemberTo(org.scilab.modules.gui.container.Container)
	 */
	public int addAsMemberTo(Container container) {
		// delegate to the container but also adding info on how to handle me (RadioButton)
		// Interface Container must describe methode: int addMember(RadioButton member);
		return container.addMember((RadioButton) this);
	}
}

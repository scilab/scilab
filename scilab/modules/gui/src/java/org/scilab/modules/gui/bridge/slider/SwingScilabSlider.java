
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.bridge.slider;

import javax.swing.JSlider;
import javax.swing.border.LineBorder;

import org.scilab.modules.gui.container.Container;
import org.scilab.modules.gui.slider.Slider;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

/**
 * Swing implementation for Scilab Slider in GUIs
 * @author Marouane BEN JELLOUL
 */
public class SwingScilabSlider extends JSlider implements Slider {

	/**
	 * Constructor
	 */
	public SwingScilabSlider() {
		super();
		// Scilab slider always snap to ticks
		setSnapToTicks(true);
	}
	
	/**
	 * 
	 */
	public void draw() {
		this.setVisible(true);
		this.doLayout();
	}

	/**
	 * Add this as member (dockable element) to the Scilab container and returns its index
	 * @param container the container in which we add this
	 * @return index of this in container components
	 * @see org.scilab.modules.gui.container.Container#addMember(org.scilab.modules.gui.dockable.Dockable)
	 * @see org.scilab.modules.gui.dockable.Dockable#addAsMemberTo(org.scilab.modules.gui.container.Container)
	 */
	public int addAsMemberTo(Container container) {
		// delegate to the container but also adding info on how to handle me (Slider)
		// Interface Container must describe methode: int addMember(Slider member);
		return container.addMember((Slider) this);
	}

	/**
	 * Gets the dimensions (width and height) of a swing Scilab Slider
	 * @return the dimensions of the Slider
	 * @see org.scilab.modules.gui.uielement.UIElement#getDims()
	 */
	public Size getDims() {
		return new Size(super.getSize().width, super.getSize().height);
	}

	/**
	 * Gets the position (X-coordinate and Y-coordinate) of a swing Scilab Slider
	 * @return the position of the Slider
	 * @see org.scilab.modules.gui.uielement.UIElement#getPosition()
	 */
	public Position getPosition() {
		return new Position(super.getX(), super.getY());
	}
	
	/**
	 * Sets the dimensions (width and height) of a swing Scilab Slider
	 * @param newSize the dimensions to set to the Slider
	 * @see org.scilab.modules.gui.uielement.UIElement#setDims(org.scilab.modules.gui.utils.Size)
	 */
	public void setDims(Size newSize) {
		super.setSize(newSize.getWidth(), newSize.getHeight());
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a swing Scilab Slider
	 * @param newPosition the position to set to the Slider
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
}

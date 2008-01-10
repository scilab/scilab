
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.bridge.slider;

import javax.swing.JSlider;

import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.slider.SimpleSlider;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

/**
 * Swing implementation for Scilab Slider in GUIs
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public class SwingScilabSlider extends JSlider implements SimpleSlider {

	private static final long serialVersionUID = -4262320156090829309L;

	/**
	 * Constructor
	 */
	public SwingScilabSlider() {
		super();
		// Scilab slider always snap to ticks
		setSnapToTicks(true);
	}

	/**
	 * Draws a swing Scilab Slider
	 * @see org.scilab.modules.gui.UIElement#draw()
	 */
	public void draw() {
		this.setVisible(true);
		this.doLayout();
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
	 * Add a callback to the Slider
	 * @param command the Scilab command to execute when the Slider is validated
	 * @param commandType the type of the command that will be executed.
	 */
	public void setCallback(String command, int commandType) {
		System.out.println("setCallback(String command, int commandType) is not yet implemented for SwingScilabSlider");
		//addActionListener(ScilabCallBack.create(command, commandType));
	}

	/**
	 * Setter for MenuBar
	 * @param menuBarToAdd the MenuBar associated to the Tab.
	 */
	public void addMenuBar(MenuBar menuBarToAdd) {
		/* Unimplemented for CheckBoxes */
		throw new UnsupportedOperationException();
	}

	/**
	 * Setter for ToolBar
	 * @param toolBarToAdd the ToolBar associated to the Tab.
	 */
	public void addToolBar(ToolBar toolBarToAdd) {
		/* Unimplemented for CheckBoxes */
		throw new UnsupportedOperationException();
	}

	/**
	 * Getter for MenuBar
	 * @return MenuBar: the MenuBar associated to the Tab.
	 */
	public MenuBar getMenuBar() {
		/* Unimplemented for CheckBoxes */
		throw new UnsupportedOperationException();
	}

	/**
	 * Getter for ToolBar
	 * @return ToolBar: the ToolBar associated to the Tab.
	 */
	public ToolBar getToolBar() {
		/* Unimplemented for CheckBoxes */
		throw new UnsupportedOperationException();
	}

	/**
	 * Get the text of the Slider
	 * @return the text
	 * @see org.scilab.modules.gui.widget.Widget#getText()
	 */
	public String getText() {
		return this.getName();
	}

	/**
	 * Set the text of the Slider
	 * @param text the text to set to the Slider
	 * @see org.scilab.modules.gui.widget.Widget#setText(java.lang.String)
	 */
	public void setText(String text) {
		this.setName(text);
	}
}

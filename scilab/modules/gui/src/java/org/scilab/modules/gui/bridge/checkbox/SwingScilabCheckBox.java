
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.bridge.checkbox;

import javax.swing.JCheckBox;

import org.scilab.modules.gui.checkbox.SimpleCheckBox;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.ScilabAlignment;
import org.scilab.modules.gui.utils.ScilabRelief;
import org.scilab.modules.gui.utils.Size;

/**
 * Swing implementation for Scilab CheckBox in GUIs
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public class SwingScilabCheckBox extends JCheckBox implements SimpleCheckBox {

	private static final long serialVersionUID = 3435428345694647542L;

	/**
	 * Constructor
	 */
	public SwingScilabCheckBox() {
		super();
		/* Avoid the L&F to erase user background settings */
		setContentAreaFilled(false);
		setOpaque(true);
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
	 * Add a callback to the CheckBox
	 * @param command the Scilab command to execute when the CheckBox is validated
	 * @param commandType the type of the command that will be executed.
	 */
	public void setCallback(String command, int commandType) {
		System.out.println("setCallback(String command, int commandType) is not yet implemented for SwingScilabCheckBox");
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
	 * Set the horizontal alignment for the CheckBox text
	 * @param alignment the value for the alignment (See ScilabAlignment.java)
	 */
	public void setHorizontalAlignment(String alignment) {
		setHorizontalAlignment(ScilabAlignment.toSwingAlignment(alignment));
	}

	/**
	 * Set the vertical alignment for the CheckBox text
	 * @param alignment the value for the alignment (See ScilabAlignment.java)
	 */
	public void setVerticalAlignment(String alignment) {
		setVerticalAlignment(ScilabAlignment.toSwingAlignment(alignment));
	}

	/**
	 * Set if the CheckBox is checked or not
	 * @param status true to set the CheckBox checked
	 */
	public void setChecked(boolean status) {
		setSelected(status);
	}
	
	/**
	 * Get the status of the CheckBox
	 * @return true if the CheckBox is checked
	 */
	public boolean isChecked() {
		return isSelected();
	}
	
	/**
	 * Set the Relief of the CheckBox
	 * @param reliefType the type of the relief to set (See ScilabRelief.java)
	 */
	public void setRelief(String reliefType) {
		setBorder(ScilabRelief.getBorderFromRelief(reliefType));
	}

	/**
	 * Destroy the CheckBox
	 */
	public void destroy() {
		getParent().remove(this);
		this.setVisible(false);
	}
}

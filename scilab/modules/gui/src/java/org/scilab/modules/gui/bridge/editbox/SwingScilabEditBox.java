
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.bridge.editbox;

import javax.swing.JTextField;

import org.scilab.modules.gui.editbox.SimpleEditBox;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.ScilabAlignment;
import org.scilab.modules.gui.utils.ScilabRelief;
import org.scilab.modules.gui.utils.Size;

/**
 * Swing implementation for Scilab EditBox in GUIs
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public class SwingScilabEditBox extends JTextField implements SimpleEditBox {
	
	private static final long serialVersionUID = 1L;

	/**
	 * Constructor
	 */
	public SwingScilabEditBox() {
		super();
	}

	/**
	 * Draws a swing Scilab EditBox
	 * @see org.scilab.modules.gui.uielement.UIElement#draw()
	 */
	public void draw() {
		this.setVisible(true);
		this.doLayout();
	}

	/**
	 * Gets the dimensions (width and height) of a swing Scilab EditBox
	 * @return the dimensions of the EditBox
	 * @see org.scilab.modules.gui.uielement.UIElement#getDims()
	 */
	public Size getDims() {
		return new Size(this.getSize().width, this.getSize().height);
	}

	/**
	 * Gets the position (X-coordinate and Y-coordinate) of a swing Scilab EditBox
	 * @return the position of the EditBox
	 * @see org.scilab.modules.gui.uielement.UIElement#getPosition()
	 */
	public Position getPosition() {
		return new Position(this.getX(), this.getY());
	}

	/**
	 * Sets the dimensions (width and height) of a swing Scilab EditBox
	 * @param newSize the dimensions we want to set to the EditBox
	 * @see org.scilab.modules.gui.uielement.UIElement#setDims(org.scilab.modules.gui.utils.Size)
	 */
	public void setDims(Size newSize) {
		this.setSize(newSize.getWidth(), newSize.getHeight());
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a swing Scilab EditBox
	 * @param newPosition the position we want to set to the EditBox
	 * @see org.scilab.modules.gui.uielement.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
	 */
	public void setPosition(Position newPosition) {
		this.setLocation(newPosition.getX(), newPosition.getY());
	}
	
	/**
	 * Add a callback to the EditBox
	 * @param command the Scilab command to execute when the EditBox is validated
	 * @param commandType the type of the command that will be executed.
	 */
	public void setCallback(String command, int commandType) {
		System.out.println("setCallback(String command, int commandType) is not yet implemented for SwingScilabEditBox");
		//addActionListener(ScilabCallBack.create(command, commandType));
	}

	/**
	 * Setter for MenuBar
	 * @param menuBarToAdd the MenuBar associated to the Tab.
	 */
	public void addMenuBar(MenuBar menuBarToAdd) {
		/* Unimplemented for EditBoxes */
		throw new UnsupportedOperationException();
	}

	/**
	 * Setter for ToolBar
	 * @param toolBarToAdd the ToolBar associated to the Tab.
	 */
	public void addToolBar(ToolBar toolBarToAdd) {
		/* Unimplemented for EditBoxes */
		throw new UnsupportedOperationException();
	}

	/**
	 * Getter for MenuBar
	 * @return MenuBar: the MenuBar associated to the Tab.
	 */
	public MenuBar getMenuBar() {
		/* Unimplemented for EditBoxes */
		throw new UnsupportedOperationException();
	}

	/**
	 * Getter for ToolBar
	 * @return ToolBar: the ToolBar associated to the Tab.
	 */
	public ToolBar getToolBar() {
		/* Unimplemented for EditBoxes */
		throw new UnsupportedOperationException();
	}

	/**
	 * Set the horizontal alignment for the EditBox text
	 * @param alignment the value for the alignment (See ScilabAlignment.java)
	 */
	public void setHorizontalAlignment(String alignment) {
		setHorizontalAlignment(ScilabAlignment.toSwingAlignment(alignment));
	}

	/**
	 * Set the vertical alignment for the EditBox text
	 * @param alignment the value for the alignment (See ScilabAlignment.java)
	 */
	public void setVerticalAlignment(String alignment) {
		throw new UnsupportedOperationException();
	}
	
	/**
	 * Set the Relief of the EditBox
	 * @param reliefType the type of the relief to set (See ScilabRelief.java)
	 */
	public void setRelief(String reliefType) {
		setBorder(ScilabRelief.getBorderFromRelief(reliefType));
	}

	/**
	 * Destroy the EditBox
	 */
	public void destroy() {
		getParent().remove(this);
	}
}

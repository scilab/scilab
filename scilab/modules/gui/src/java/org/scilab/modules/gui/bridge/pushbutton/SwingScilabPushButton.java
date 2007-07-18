
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.bridge.pushbutton;

import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.border.LineBorder;

import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.menubar.SimpleMenuBar;
import org.scilab.modules.gui.pushbutton.SimplePushButton;
import org.scilab.modules.gui.toolbar.SimpleToolBar;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

/**
* Swing implementation for Scilab PushButtons in GUIs
* @author Marouane BEN JELLOUL
*/
public class SwingScilabPushButton extends JButton implements SimplePushButton {
	
	/**
	 * Constructor
	 */
	public SwingScilabPushButton() {
		super();
	}

	/**
	 * Sets a MenuBar to an element
	 * @param newMenuBar the MenuBar to set to the element
	 */
	public void addMenuBar(SimpleMenuBar newMenuBar) {
		// TODO Auto-generated method stub
	}

	/**
	 * Sets a ToolBar to an element
	 * @param newToolBar the ToolBar to set to the element
	 */
	public void addToolBar(SimpleToolBar newToolBar) {
		// TODO Auto-generated method stub	
	}
	
	/**
	 * Draws a swing Scilab PushButton
	 * @see org.scilab.modules.gui.uielement.UIElement#draw()
	 */
	public void draw() {
		this.setVisible(true);
		this.doLayout();
	}

	/**
	 * Gets the dimensions (width and height) of a swing Scilab PushButton
	 * @return the dimensions of the PushButton
	 * @see org.scilab.modules.gui.uielement.UIElement#getDims()
	 */
	public Size getDims() {
		return new Size(super.getSize().width, super.getSize().height);
	}

	/**
	 * Gets the position (X-coordinate and Y-coordinate) of a swing Scilab PushButton
	 * @return the position of the PushButton
	 * @see org.scilab.modules.gui.uielement.UIElement#getPosition()
	 */
	public Position getPosition() {
		return new Position(super.getX(), super.getY());
	}
	
	/**
	 * Sets the dimensions (width and height) of a swing Scilab PushButton
	 * @param newSize the dimensions to set to the PushButton
	 * @see org.scilab.modules.gui.uielement.UIElement#setDims(org.scilab.modules.gui.utils.Size)
	 */
	public void setDims(Size newSize) {
		super.setSize(newSize.getWidth(), newSize.getHeight());
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a swing Scilab PushButton
	 * @param newPosition the position to set to the PushButton
	 * @see org.scilab.modules.gui.uielement.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
	 */
	public void setPosition(Position newPosition) {
		super.setLocation(newPosition.getX(), newPosition.getY());
	}
	
	/**
	 * To set the Border color and size of the element.
	 * @param lineBorder the LineBorder
	 */
	public void setBorder(LineBorder lineBorder) {
		super.setBorder(lineBorder);
	}
	
	/**
	 * Sets the icon of a PushButton
	 * @param filename the path to the icon image to set to the PushButton
	 */
	public void setIcon(String filename) {
		super.setIcon(new ImageIcon(filename));
	}

}


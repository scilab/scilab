
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.bridge.pushbutton;

import java.awt.Dimension;

import javax.swing.ImageIcon;
import javax.swing.JButton;

import org.scilab.modules.gui.events.callback.CallBack;
import org.scilab.modules.gui.events.callback.ScilabCallBack;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.pushbutton.SimplePushButton;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.ScilabAlignment;
import org.scilab.modules.gui.utils.ScilabRelief;
import org.scilab.modules.gui.utils.Size;

/**
 * Swing implementation for Scilab PushButtons in GUIs
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public class SwingScilabPushButton extends JButton implements SimplePushButton {
	
	private static final long serialVersionUID = 2277539556048935959L;
	
	private CallBack callback;

	/**
	 * Constructor
	 */
	public SwingScilabPushButton() {
		super();
		this.setFocusable(false);
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
		setSize(newSize.getWidth(), newSize.getHeight());
		setPreferredSize(new Dimension(newSize.getWidth(), newSize.getHeight()));
		//setBounds(getPosition().getX(), getPosition().getY(), newSize.getWidth(), newSize.getHeight());
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a swing Scilab PushButton
	 * @param newPosition the position to set to the PushButton
	 * @see org.scilab.modules.gui.uielement.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
	 */
	public void setPosition(Position newPosition) {
		setLocation(newPosition.getX(), newPosition.getY());
		setBounds(newPosition.getX(), newPosition.getY(), getDims().getWidth(),	getDims().getHeight());
	}
	
	/**
	 * Sets the icon of a PushButton
	 * @param filename the path to the icon image to set to the PushButton
	 */
	public void setIcon(String filename) {
		super.setIcon(new ImageIcon(filename));
	}
	
	/**
	 * Add a callback to the pushbutton
	 * @param command the Scilab command to execute when the pushbutton is activated
	 * @param commandType the type of the command that will be executed.
	 */
	public void setCallback(String command, int commandType) {
		callback = ScilabCallBack.create(command, commandType);
		addActionListener(callback);
	}

	/**
	 * Set if the pushbutton is enabled or not
	 * @param status true if the pushbutton is enabled
	 */
	public void setEnabled(boolean status) {
		super.setEnabled(status);
		/* (Des)Activate the callback */ 
		if (callback != null) {
			if (status) {
				addActionListener(callback);
			} else {
				removeActionListener(callback);
			}
		}
	}

	/**
	 * Setter for MenuBar
	 * @param menuBarToAdd the MenuBar associated to the Tab.
	 */
	public void addMenuBar(MenuBar menuBarToAdd) {
		/* Unimplemented for pushbuttons */
		throw new UnsupportedOperationException();
	}

	/**
	 * Setter for ToolBar
	 * @param toolBarToAdd the ToolBar associated to the Tab.
	 */
	public void addToolBar(ToolBar toolBarToAdd) {
		/* Unimplemented for pushbuttons */
		throw new UnsupportedOperationException();
	}

	/**
	 * Getter for MenuBar
	 * @return MenuBar: the MenuBar associated to the Tab.
	 */
	public MenuBar getMenuBar() {
		/* Unimplemented for pushbuttons */
		throw new UnsupportedOperationException();
	}

	/**
	 * Getter for ToolBar
	 * @return ToolBar: the ToolBar associated to the Tab.
	 */
	public ToolBar getToolBar() {
		/* Unimplemented for pushbuttons */
		throw new UnsupportedOperationException();
	}
	
	/**
	 * Set the horizontal alignment for the PushButton text
	 * @param alignment the value for the alignment (See ScilabAlignment.java)
	 */
	public void setHorizontalAlignment(String alignment) {
		setHorizontalAlignment(ScilabAlignment.toSwingAlignment(alignment));
	}

	/**
	 * Set the vertical alignment for the PushButton text
	 * @param alignment the value for the alignment (See ScilabAlignment.java)
	 */
	public void setVerticalAlignment(String alignment) {
		setVerticalAlignment(ScilabAlignment.toSwingAlignment(alignment));
	}

	/**
	 * Set the Relief of the PushButton
	 * @param reliefType the type of the relief to set (See ScilabRelief.java)
	 */
	public void setRelief(String reliefType) {
		setBorder(ScilabRelief.getBorderFromRelief(reliefType));
	}
}


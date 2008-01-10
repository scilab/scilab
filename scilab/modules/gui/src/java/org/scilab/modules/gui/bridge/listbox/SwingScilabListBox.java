
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.bridge.listbox;

import java.awt.Color;
import java.awt.Font;

import javax.swing.JList;
import javax.swing.JScrollPane;

import org.scilab.modules.gui.listbox.SimpleListBox;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

/**
 * Swing implementation for Scilab ListBox in GUIs
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public class SwingScilabListBox implements SimpleListBox {
	
	/**
	 * the Scroll Pane that contain the JList
	 */
	private JScrollPane scrollPane;
	
	/**
	 * the JList we use
	 */
	private JList list;
	
	/**
	 * Constructor
	 */
	public SwingScilabListBox() {
		list = new JList();
		scrollPane = new JScrollPane(list);
	}
	
	/**
	 * Get the scrollpane
	 * @return the scrollpane
	 */
	public JScrollPane getScrollPane() {
		return scrollPane;
	}

	/**
	 * To get the Background color of the element.
	 * @return color the Color
	 */
	public Color getBackground() {
		return list.getBackground();
	}
	
	/**
	 * To get the Font of the element.
	 * @return font the Font
	 */
	public Font getFont() {
		return list.getFont();
	}
	
	/**
	 * To get the Foreground color of the element.
	 * @return color the Color
	 */
	public Color getForeground() {
		return list.getForeground();
	}
	
	/**
	 * To set the Background color of the element.
	 * @param color the Color
	 */
	public void setBackground(Color color) {
		list.setBackground(color);
	}

	/**
	 * To set the Font of the element.
	 * @param font the Font
	 */
	public void setFont(Font font) {
		list.setFont(font);
	}
	
	/**
	 * To set the Foreground color of the element.
	 * @param color the Color
	 */
	public void setForeground(Color color) {
		list.setForeground(color);
	}
	
	/**
	 * Draws a swing Scilab tab
	 * @see org.scilab.modules.gui.uielement.UIElement#draw()
	 */
	public void draw() {
		this.setVisible(true);
	}

	/**
	 * Gets the dimensions (width and height) of a swing Scilab tab
	 * @return the dimensions of the tab
	 * @see org.scilab.modules.gui.uielement.UIElement#getDims()
	 */
	public Size getDims() {
		return new Size(scrollPane.getWidth(), scrollPane.getHeight());
	}

	/**
	 * Gets the position (X-coordinate and Y-coordinate) of a swing Scilab tab
	 * @return the position of the tab
	 * @see org.scilab.modules.gui.uielement.UIElement#getPosition()
	 */
	public Position getPosition() {
		return new Position(scrollPane.getX(), scrollPane.getY());
	}

	/**
	 * Sets the dimensions (width and height) of a swing Scilab tab
	 * @param newSize the dimensions we want to set to the tab
	 * @see org.scilab.modules.gui.uielement.UIElement#setDims(org.scilab.modules.gui.utils.Size)
	 */
	public void setDims(Size newSize) {
		scrollPane.setSize(newSize.getWidth(), newSize.getHeight());
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a swing Scilab tab
	 * @param newPosition the position we want to set to the tab
	 * @see org.scilab.modules.gui.uielement.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
	 */
	public void setPosition(Position newPosition) {
		scrollPane.setLocation(newPosition.getX(), newPosition.getY());
	}

	/**
	 * Gets the visibility status of an UIElement
	 * @return the visibility status of the UIElement (true if the UIElement is visible, false if not)
	 */
	public boolean isVisible() {
		return list.isVisible() & scrollPane.isVisible();
	}
	
	/**
	 * Sets the visibility status of an UIElement
	 * @param newVisibleState the visibility status we want to set for the UIElement
	 * 			(true if the UIElement is visible, false if not)
	 */
	public void setVisible(boolean newVisibleState) {
		scrollPane.setVisible(newVisibleState);
		list.setVisible(newVisibleState);
	}
	/**
	 * Add a callback to the CheckBox
	 * @param command the Scilab command to execute when the CheckBox is validated
	 * @param commandType the type of the command that will be executed.
	 */
	public void setCallback(String command, int commandType) {
		System.out.println("setCallback(String command, int commandType) is not yet implemented for SwingScilabListBox");
		//addActionListener(ScilabCallBack.create(command, commandType));
	}

	/**
	 * Setter for MenuBar
	 * @param menuBarToAdd the MenuBar associated to the Tab.
	 */
	public void addMenuBar(MenuBar menuBarToAdd) {
		/* Unimplemented for ListBoxes */
		throw new UnsupportedOperationException();
	}

	/**
	 * Setter for ToolBar
	 * @param toolBarToAdd the ToolBar associated to the Tab.
	 */
	public void addToolBar(ToolBar toolBarToAdd) {
		/* Unimplemented for ListBoxes */
		throw new UnsupportedOperationException();
	}

	/**
	 * Getter for MenuBar
	 * @return MenuBar: the MenuBar associated to the Tab.
	 */
	public MenuBar getMenuBar() {
		/* Unimplemented for ListBoxes */
		throw new UnsupportedOperationException();
	}

	/**
	 * Getter for ToolBar
	 * @return ToolBar: the ToolBar associated to the Tab.
	 */
	public ToolBar getToolBar() {
		/* Unimplemented for ListBoxes */
		throw new UnsupportedOperationException();
	}

	/**
	 * Get the text if the list items
	 * @return the items
	 * @see org.scilab.modules.gui.widget.Widget#getText()
	 */
	public String getText() {
		// TODO Auto-generated method stub
		return null;
	}

	/**
	 * Enable or disable the list for selection
	 * @param status true to set the list enabled
	 * @see org.scilab.modules.gui.widget.Widget#setEnabled(boolean)
	 */
	public void setEnabled(boolean status) {
		list.setEnabled(status);
		scrollPane.setEnabled(status);
	}

	/**
	 * Set the text of the list items
	 * @param text the text of the items
	 * @see org.scilab.modules.gui.widget.Widget#setText(java.lang.String)
	 */
	public void setText(String text) {
		// TODO Auto-generated method stub
	}
}

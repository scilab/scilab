
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.bridge.listbox;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;

import javax.swing.JList;
import javax.swing.JScrollPane;
import javax.swing.border.LineBorder;

import org.scilab.modules.gui.container.Container;
import org.scilab.modules.gui.listbox.ListBox;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

/**
 * Swing implementation for Scilab ListBox in GUIs
 * @author Marouane BEN JELLOUL
 */
public class SwingScilabListBox {
//	 FIXME : Must have some Interface here...
//	implements SimpleListBox {
	
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
	 * Set the selection mode. The accepted selection modes are:
	 * - ListBox.SINGLE_SELECTION (=0)
	 * - ListBox.MULTIPLE_SELECTION (=2)
	 * @param mode - the selection mode we want
	 * @throws java.lang.IllegalArgumentException - if the selection mode isn't one of those allowed
	 * @see org.scilab.modules.gui.listbox.ListBox#setSelectionMode(int)
	 */
	public void setSelectionMode(int mode) throws IllegalArgumentException {
		switch (mode) {
			case 0:
				// ListBox.SINGLE_SELECTION is the same as ListSelectionModel.SINGLE_SELECTION of Swing
				list.setSelectionMode(mode);
				break;
			case 2:
				// ListBox.MULTIPLE_SELECTION is the same as ListSelectionModel.MULTIPLE_INTERVAL_SELECTION of Swing
				list.setSelectionMode(mode);
				break;
			default:
				throw new IllegalArgumentException("the mode " + mode + " is not allowed");
		}
	}
	
	/**
	 * To set the Border color and size of the element.
	 * @param lineBorder the LineBorder
	 */
	public void setBorder(LineBorder lineBorder) {
		scrollPane.setBorder(lineBorder);
	}
	
	/**
	 * get the scroll pane that contain the listBox.
	 * @return the scroll pane that contain the listBox
	 */
	public JScrollPane getScrollPane() {
		return scrollPane;
	}
	
	/**
	 * To set the content of the ListBox.
	 * @param listData the content of the ListBox
	 */
	public void setListData(String[] listData) {
		list.setListData(listData);
	}
	
	/**
	 * To set the Dimension of the element.
	 * @param dimension the Dimension
	 */
	public void setSize(Dimension dimension) {
		scrollPane.setSize(dimension);
	}
	
	/**
	 * To get the Dimension of the element.
	 * @return the Dimension
	 */
	public Dimension getSize() {
		return scrollPane.getSize();
	}
	
	/**
	 * Moves this component to a new location. The top-left corner of the new location is specified by the x and y 
	 * parameters in the coordinate space of this component's parent.
	 * @param x - the x-coordinate of the new location's top-left corner in the parent's coordinate space
	 * @param y - the y-coordinate of the new location's top-left corner in the parent's coordinate space
	 */
	public void setLocation(int x, int y) {
		scrollPane.setLocation(x, y);
	}
	
	/**
	 * To get the x coordinate  of the element.
	 * @return the x coordinate
	 */
	public int getX() {
		return scrollPane.getX();
	}
	
	/**
	 * To get the y coordinate  of the element.
	 * @return the y coordinate
	 */
	public int getY() {
		return scrollPane.getY();
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
		return new Size(this.getSize().width, this.getSize().height);
	}

	/**
	 * Gets the position (X-coordinate and Y-coordinate) of a swing Scilab tab
	 * @return the position of the tab
	 * @see org.scilab.modules.gui.uielement.UIElement#getPosition()
	 */
	public Position getPosition() {
		return new Position(this.getX(), this.getY());
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
		this.setLocation(newPosition.getX(), newPosition.getY());
	}

	/**
	 * Gets the visibility status of an UIElement
	 * @return the visibility status of the UIElement (true if the UIElement is visible, false if not)
	 */
	public boolean isVisible() {
		// TODO check if this is ok
		return list.isVisible();
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
	 * Add this as member (dockable element) to the Scilab container and returns its index
	 * @param container the container in which we add this
	 * @return index of this in container components
	 * @see org.scilab.modules.gui.container.Container#addMember(org.scilab.modules.gui.dockable.Dockable)
	 * @see org.scilab.modules.gui.dockable.Dockable#addAsMemberTo(org.scilab.modules.gui.container.Container)
	 */
	public int addAsMemberTo(Container container) {
		// delegate to the container but also adding info on how to handle me (ListBox)
		// Interface Container must describe methode: int addMember(ListBox member);
		return container.addMember((ListBox) this);
	}
}

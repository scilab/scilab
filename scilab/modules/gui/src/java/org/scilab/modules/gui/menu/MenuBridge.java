
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.menu;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;

import org.scilab.modules.gui.menuitem.MenuItemBridge;

/**
 * Interface for MenuBridge the associated object to Scilab GUIs Menu
 * @author Marouane BEN JELLOUL
 */
public interface MenuBridge {
	/**
	 * Append a MenuItemBridge to the MenuBridge
	 * @param newMenuItem the MenuItemBridge to add to the MenuBridge
	 * @return the added MenuItemBridge
	 */
	MenuItemBridge add(MenuItemBridge newMenuItem);
	
	/**
	 * Sets the text of the Menu
	 * @param newText the text we want to set for the Menu
	 */
	void setText(String newText);
	
	/**
	 * set a mnemonic to the Menu
	 * @param mnemonic the new mnemonic of the Menu
	 */
	void setMnemonic(int mnemonic);
	
	/**
	 * Add a Separator to a Menu
	 */
	void addSeparator();
	
	/**
	 * To set the Background color of the element.
	 * @param color the Color
	 */
	void setBackground(Color color);
	
	/**
	 * To get the Background color of the element.
	 * @return color the Color
	 */
	Color getBackground();
	
	/**
	 * To set the Foreground color of the element.
	 * @param color the Color
	 */
	void setForeground(Color color);
	
	/**
	 * To get the Foreground color of the element.
	 * @return color the Color
	 */
	Color getForeground();
	
	/**
	 * To set the Font of the element.
	 * @param font the Font
	 */
	void setFont(Font font);
	
	/**
	 * To get the Font of the element.
	 * @return font the Font
	 */
	Font getFont();
	
	/**
	 * To set the Dimension of the element.
	 * @param dimension the Dimension
	 */
	void setSize(Dimension dimension);
	
	/**
	 * To get the Dimension of the element.
	 * @return the Dimension
	 */
	Dimension getSize();
	
	/**
	 * Moves this component to a new location. The top-left corner of the new location is specified by the x and y 
	 * parameters in the coordinate space of this component's parent.
	 * @param x - the x-coordinate of the new location's top-left corner in the parent's coordinate space
	 * @param y - the y-coordinate of the new location's top-left corner in the parent's coordinate space
	 */
	void setLocation(int x, int y);
	
	/**
	 * To get the x coordinate  of the element.
	 * @return the x coordinate
	 */
	int getX();
	
	/**
	 * To get the y coordinate  of the element.
	 * @return the y coordinate
	 */
	int getY();
}

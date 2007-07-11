
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.menu;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;

import javax.swing.border.LineBorder;

import org.scilab.modules.gui.bridge.ScilabBridge;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.menuitem.ScilabMenuItem;

/**
 * Class for Scilab Menus in GUIs
 * @author Marouane BEN JELLOUL
 */
public class ScilabMenu extends ScilabMenuItem implements Menu {

	/**
	 * Constructor
	 */
	protected ScilabMenu() {
        throw new UnsupportedOperationException(); /* Prevents calls from subclass */
	}
	
	/**
	 * Creates a Scilab Menu object
	 * @return the created Menu
	 */
	public static Menu createMenu() {
		return ScilabBridge.createMenu();
	}
	
	/**
	 * Append a MenuItem to a Scilab Menu
	 * @param newMenuItem the MenuItem to add to the Menu
	 * @return the added MenuItem
	 * @see org.scilab.modules.gui.menu.Menu#add(org.scilab.modules.gui.MenuItem)
	 */
	public MenuItem add(MenuItem newMenuItem) {
		return ScilabBridge.add(this, newMenuItem);
	}
	
	/**
	 * Sets the text of a Scilab Menu
	 * @param newText the Text to set to the Menu
	 * @see org.scilab.modules.gui.menu.Menu#setText(java.lang.String)
	 */
	public void setText(String newText) {
//		TODO QUESTION is this code needed or this methode can be empty ?
//		TODO uncomment if needed and code the needed methodes
		// ScilabBridge.setText(this, newText);
	}
	
	/**
	 * set a mnemonic to a Menu
	 * @param mnemonic the mnemonic to add to the Menu
	 * @see org.scilab.modules.gui.menu.Menu#setMnemonic(org.scilab.modules.gui.widget.int)
	 */
	public void setMnemonic(int mnemonic) {
		ScilabBridge.setMnemonic(this, mnemonic);
	}
	
	/**
	 * Add a Separator to the Menu
	 * @see org.scilab.modules.gui.menu.Menu#addSeparator(org.scilab.modules.gui..)
	 */
	public void addSeparator() {
		ScilabBridge.addSeparator(this);
	}

	/**
	 * To get the Background color of the element.
	 * @return color the Color
	 */
	public Color getBackground() {
		// TODO Auto-generated method stub
		return null;
	}

	/**
	 * To get the Font of the element.
	 * @return font the Font
	 */
	public Font getFont() {
		// TODO Auto-generated method stub
		return null;
	}

	/**
	 * To get the Foreground color of the element.
	 * @return color the Color
	 */
	public Color getForeground() {
		// TODO Auto-generated method stub
		return null;
	}
		
	/**
	 * To get the Dimension of the element.
	 * @return the Dimension
	 */
	public Dimension getSize() {
		// TODO Auto-generated method stub
		return null;
	}
	
	/**
	 * To get the x coordinate  of the element.
	 * @return the x coordinate
	 */
	public int getX() {
		// TODO Auto-generated method stub
		return 0;
	}

	/**
	 * To get the y coordinate  of the element.
	 * @return the y coordinate
	 */
	public int getY() {
		// TODO Auto-generated method stub
		return 0;
	}
	
	/**
	 * To set the Background color of the element.
	 * @param color the Color
	 */
	public void setBackground(Color color) {
		// TODO Auto-generated method stub
		
	}

	/**
	 * To set the Border color and size of the element.
	 * @param lineBorder the LineBorder
	 */
	public void setBorder(LineBorder lineBorder) {
		// TODO Auto-generated method stub
		
	}

	/**
	 * To set the Font of the element.
	 * @param font the Font
	 */
	public void setFont(Font font) {
		// TODO Auto-generated method stub
		
	}

	/**
	 * To set the Foreground color of the element.
	 * @param color the Color
	 */
	public void setForeground(Color color) {
		// TODO Auto-generated method stub
		
	}
	
	/**
	 * Moves this component to a new location. The top-left corner of the new location is specified by the x and y 
	 * parameters in the coordinate space of this component's parent.
	 * @param x - the x-coordinate of the new location's top-left corner in the parent's coordinate space
	 * @param y - the y-coordinate of the new location's top-left corner in the parent's coordinate space
	 */
	public void setLocation(int x, int y) {
		// TODO Auto-generated method stub
		
	}

	/**
	 * To set the Dimension of the element.
	 * @param dimension the Dimension
	 */
	public void setSize(Dimension dimension) {
		// TODO Auto-generated method stub
		
	}
}

/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Marouane BEN JELLOUL
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.window;

import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

/**
 * Interface for SimpleWindow the associated object to Scilab GUIs Window
 * @author Marouane BEN JELLOUL
 */
public interface SimpleWindow {
	
	/**
	 * Gets the title of a window
	 * @return the title of the window
	 */
	String getTitle();

	/**
	 * Sets the title of a window
	 * @param newWindowTitle the title we want to set for the window
	 */
	void setTitle(String newWindowTitle);

	/**
	 * Add a tab to a window
	 * @param newTab the tab to add to the window
	 */
	void addTab(Tab newTab);
	
	/**
	 * Remove a tab from a window
	 * @param tab tab to remove
	 */
	void removeTab(Tab tab);
	
	/**
	 * Sets a InfoBar to a window
	 * @param newInfoBar the Scilab InfoBar to set to the Scilab window
	 */
	void addInfoBar(TextBox newInfoBar);
	
	/**
	 * Gets the size of an Window (width and height)
	 * @return the size of the Window
	 */
	Size getDims();

	/**
	 * Sets the size of an Window (width and height)
	 * @param newSize the size we want to set to the Window
	 */
	void setDims(Size newSize);

	/**
	 * Gets the position of an Window (X-coordinate and Y-corrdinate)
	 * @return the position of the Window
	 */
	Position getPosition();

	/**
	 * Sets the position of an Window (X-coordinate and Y-corrdinate)
	 * @param newPosition the position we want to set to the Window
	 */
	void setPosition(Position newPosition);

	/**
	 * Gets the visibility status of an Window
	 * @return the visibility status of the Window (true if the Window is visible, false if not)
	 */
	boolean isVisible();

	/**
	 * Sets the visibility status of an Window
	 * @param newVisibleState the visibility status we want to set for the Window
	 * 			(true if the Window is visible, false if not)
	 */
	void setVisible(boolean newVisibleState);

	/**
	 * Draws an Window
	 */
	void draw();
	
	/**
	 * Deiconify the window and put it in front of other window
	 */
	void raise();
	
	/**
	 * Sets a MenuBar to a window
	 * @param newMenuBar the MenuBar to add to the window
	 */
	void addMenuBar(MenuBar newMenuBar);

	/**
	 * Sets a ToolBar to a window
	 * @param newToolBar the ToolBar to set to the window
	 */
	void addToolBar(ToolBar newToolBar);
	
	/**
	 * @return number of objects (tabs) docked in this window
	 */
	int getNbDockedObjects();
	
	/**
	 * Set the element id for this window
	 * @param id the id of the corresponding window object
	 */
	void setElementId(int id);
	
	/**
	 * Get the element id for this window
	 * @return id the id of the corresponding window object
	 */
	int getElementId();
	
	/**
	 * Close this window
	 */
	void close();
	
	/**
	 * Update the dimension of the window and its component.
	 * Only useful when the window is not yet visible
	 */
	void updateDimensions();
	
	/**
	* Deiconified Window
	*/
	void windowDeiconified();
	
	/**
	* Iconified Window
	*/
	void windowIconified();
	
	/**
	* Maximized Window
	*/
	void windowMaximized();

	/**
	* Window is in the "normal" state.
	*/
	void windowNormal();

}

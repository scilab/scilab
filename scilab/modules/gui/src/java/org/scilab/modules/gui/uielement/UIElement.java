
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.uielement;

//import java.awt.Color;
//import javax.swing.border.LineBorder;

import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

/**
 * Interface for Scilab GUIs element
 * In the following, an UIElement is a Scilab GUI element
 * @author Vincent COUVERT
 * @author Bruno JOFRET
 */
public interface UIElement {

	/**
	 * Associate the Tool bar to the UIElement
	 * @return the Tool bar.
	 */
	public abstract void addToolBar(ToolBar toolBarToAdd);

	/**
	 * Associate the Menu bar to the UIElement
	 * @return the Tool bar.
	 */
	public abstract void addMenuBar(MenuBar toolMenuToAdd);
//
//	public abstract void resize();
//
//	public abstract void focus();
//
	
	/**
	 * Gets the size of an UIElement (width and height)
	 * @return the size of the UIElement
	 */
	Size getDims();

	/**
	 * Sets the size of an UIElement (width and height)
	 * @param newSize the size we want to set to the UIElement
	 */
	void setDims(Size newSize);

	/**
	 * Gets the position of an UIElement (X-coordinate and Y-corrdinate)
	 * @return the position of the UIElement
	 */
	Position getPosition();

	/**
	 * Sets the position of an UIElement (X-coordinate and Y-corrdinate)
	 * @param newPosition the position we want to set to the UIElement
	 */
	void setPosition(Position newPosition);

	/**
	 * Gets the visibility status of an UIElement
	 * @return the visibility status of the UIElement (true if the UIElement is visible, false if not)
	 */
	boolean isVisible();

	/**
	 * Sets the visibility status of an UIElement
	 * @param newVisibleState the visibility status we want to set for the UIElement
	 * 			(true if the UIElement is visible, false if not)
	 */
	void setVisible(boolean newVisibleState);

	/**
	 * Draws an UIElement
	 */
	void draw();

//	public abstract Layout getForegroundLayout();
//
//	public abstract void setForegroundLayout(Layout newLayout);
//
//	public abstract Layout getBackgroundLayout();
//
//	public abstract void setBackgroundLayout(Layout newLayout);
//
//	public abstract Layout getTextLayout();
//
//	public abstract void setTextLayout(Layout newLayout);
	
	/**
	 * To set the Background color of the element.
	 * @param color the Color
	 */
	//void setBackground(Color color);
	
	/**
	 * To set the Border color and Border size of the element.
	 * @param lineBorder the LineBorder
	 */
	//void setBorder(LineBorder lineBorder);
	
}

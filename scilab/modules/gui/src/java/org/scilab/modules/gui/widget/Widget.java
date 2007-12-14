
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.widget;

import java.awt.Color;

import org.scilab.modules.gui.dockable.Dockable;

/**
 * Interface for widget associated to objects in Scilab GUIs
 * @author Marouane BEN JELLOUL
 */
public interface Widget extends Dockable {

	/**
	 * To set the Background color of the widget.
	 * @param color the Color
	 */
	void setBackground(Color color);
}

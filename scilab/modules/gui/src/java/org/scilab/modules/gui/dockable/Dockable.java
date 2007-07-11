
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.dockable;

import org.scilab.modules.gui.container.Container;
import org.scilab.modules.gui.uielement.UIElement;

/**
 * Interface for dockable objects in Scilab GUIs
 * @author Vincent COUVERT
 */
public interface Dockable extends UIElement {

	/**
	 * Add this Dockable to the container and return its index
	 * @param container the container in which we add this
	 * @return index of member in ArrayList 
	 * @see 
	 */
	int addAsMemberTo(Container container);
	
}

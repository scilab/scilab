
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.dockable;

import org.scilab.modules.gui.container.SimpleContainer;
import org.scilab.modules.gui.uielement.SimpleUIElement;

/**
 * Interface for SimpleDockable the associated object to Scilab GUIs Dockable
 * @author Marouane BEN JELLOUL
 */
public interface SimpleDockable extends SimpleUIElement {

	/**
	 * Add this Dockable to the container and return its index
	 * @param container the container in which we add this
	 * @return index of member in ArrayList 
	 * @see 
	 */
	int addAsMemberTo(SimpleContainer container);
}

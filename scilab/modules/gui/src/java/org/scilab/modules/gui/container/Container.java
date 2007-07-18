
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.container;

import org.scilab.modules.gui.dockable.Dockable;

/**
 * Interface for Scilab containers in GUIs
 * @author Vincent COUVERT
 */
public interface Container extends Dockable {

	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	int addMember(Dockable member);
	
}

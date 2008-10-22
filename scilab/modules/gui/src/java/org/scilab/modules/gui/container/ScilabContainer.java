/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent Couvert
 * Copyright (C) 2007 - INRIA - Bruno Jofret
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.container;

import java.util.ArrayList;
import java.util.List;

import org.scilab.modules.gui.dockable.Dockable;
import org.scilab.modules.gui.dockable.ScilabDockable;

/**
 * Class for Scilab containers in GUIs
 * @author Vincent COUVERT
 * @author Bruno Jofret
 */
public abstract class ScilabContainer extends ScilabDockable implements Container {

	/**
	 * All dockables objects in the container
	 */
	protected List<ScilabDockable> members = new ArrayList<ScilabDockable>();

	/**
	 * Get all members (all dockable elements) of container
	 * @return the members
	 */
	//public abstract List<ScilabDockable> getMembers();

	/**
	 * Set all members (all dockable elements) of container
	 * @param members the members to set
	 */
	//public abstract void setMembers(List<ScilabDockable> members);

	/**
	 * Get only a member (dockable element) of container by giving its index
	 * @param index position of member in ArrayList
	 * @return the members
	 */
	//public abstract ScilabDockable getMember(int index);

	/**
	 * Set only member (dockable element) of container at position index
	 * @param member the member to set
	 * @param index position of member in ArrayList
	 */
	//public abstract void setMember(ScilabDockable member, int index);

	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	//public abstract int addMember(Console member);

	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	//public abstract int addMember(Frame member);
	
	/**
	 * Add a member (dockable element) to a Scilab container and returns its index
	 * @param member the member to add
	 * @return index of member in container components
	 * @see org.scilab.modules.gui.container.Container#addMember(org.scilab.modules.gui.dockable.Dockable)
	 */
	public int addMember(Dockable member) {
		return addMember(member);
	}

}

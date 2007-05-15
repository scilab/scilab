
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.container;

import java.util.ArrayList;
import java.util.List;

import org.scilab.modules.gui.dockable.ScilabDockable;

/**
 * Class for Scilab containers in GUIs
 * @author Vincent COUVERT
 * @author Bruno JOFRET
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

}

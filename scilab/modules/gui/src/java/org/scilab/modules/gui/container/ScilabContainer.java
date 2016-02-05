/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent Couvert
 * Copyright (C) 2007 - INRIA - Bruno Jofret
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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

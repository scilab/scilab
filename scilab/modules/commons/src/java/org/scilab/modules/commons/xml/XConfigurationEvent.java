/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
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

package org.scilab.modules.commons.xml;

import java.util.Set;

public class XConfigurationEvent {

    private Set<String> changedPaths;

    /**
     * Default constructor
     * @param changedPaths a set containing all the paths which have been modified
     */
    public XConfigurationEvent(Set<String> changedPaths) {
        this.changedPaths = changedPaths;
    }

    /**
     * Get the modified paths
     * @return the modified paths as a set
     */
    public Set<String> getModifiedPaths() {
        return changedPaths;
    }
}
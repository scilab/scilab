/*
 * Scilab (http://www.scilab.org/) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Calixte DENIZET
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

package org.scilab.modules.types;

import java.util.EventListener;
import java.util.Set;

/**
 * The listener to listen to the refresh
 */
public interface ScilabVariablesListener extends EventListener {

    /**
     * Prevent the listener that a variable has been updated
     *
     * @param event
     *            the event
     */
    public void scilabVariableUpdated(ScilabVariablesEvent event);

    /**
     * Gets the listened variables
     *
     * @return the listened variables name
     */
    public Set<String> getListenedVariables();
}

/*
 * Scilab (http://www.scilab.org/) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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

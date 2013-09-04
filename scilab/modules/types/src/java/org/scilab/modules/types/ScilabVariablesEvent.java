/*
 * Scilab (http://www.scilab.org/) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.types;

/**
 * Event thrown when refresh has been done
 */
public class ScilabVariablesEvent {

    private ScilabType var;

    /**
     * Constructor
     *
     * @param var
     *            the new refreshed variable
     */
    public ScilabVariablesEvent(ScilabType var) {
        this.var = var;
    }

    /**
     * @return the Scilab variable newly created
     */
    public ScilabType getScilabType() {
        return var;
    }
}

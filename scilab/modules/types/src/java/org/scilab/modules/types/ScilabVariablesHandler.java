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

/**
 * Interface to handle a Scilab variable coming from C/C++
 */
public interface ScilabVariablesHandler {

    /**
     * Handle a Scilab variable
     *
     * @param var
     *            the variable
     */
    public void handle(ScilabType var);
}

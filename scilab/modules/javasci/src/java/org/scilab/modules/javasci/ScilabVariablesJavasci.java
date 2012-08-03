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

package org.scilab.modules.javasci;

import java.util.HashMap;
import java.util.Map;

import org.scilab.modules.types.GetScilabVariable;
import org.scilab.modules.types.ScilabVariables;
import org.scilab.modules.types.ScilabVariablesHandler;
import org.scilab.modules.types.ScilabType;

/**
 * Class to get Scilab data from a Java call
 */
public final class ScilabVariablesJavasci implements ScilabVariablesHandler {

    private static final Map<Thread, ScilabType> map = new HashMap<Thread, ScilabType>();

    private static int id = -1;

    /**
     * Constructor
     */
    private ScilabVariablesJavasci() { }

    /**
     * {@inheritDoc}
     */
    public void handle(ScilabType var) {
        map.put(Thread.currentThread(), var);
    }

    /**
     * Get a Scilab variable with a given name
     * @param name the variable name
     * @param swapRowCol if true the returned data will be stored row by row
     * @return the corresponding ScilabType object
     */
    public static final ScilabType getScilabVariable(String name, boolean swapRowCol) {
        if (id == -1) {
            id = ScilabVariables.addScilabVariablesHandler(new ScilabVariablesJavasci());
        }

        if (name != null && !name.isEmpty()) {
            GetScilabVariable.getScilabVariable(name, swapRowCol ? 1 : 0, id);
            Thread t = Thread.currentThread();
            ScilabType var = map.get(t);
            map.remove(t);

            return var;
        }

        return null;
    }

    /**
     * Get a Scilab variable with a given name
     * @param name the variable name
     * @return the corresponding ScilabType object where the data are stored row by row
     */
    public static final ScilabType getScilabVariable(String name) {
        return getScilabVariable(name, true);
    }
}

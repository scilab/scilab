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
     * @param byref if true the variable is passed by reference if it is possible
     * @return the corresponding ScilabType object
     */
    public static final ScilabType getScilabVariable(String name, boolean swapRowCol, boolean byref) {
        if (id == -1) {
            id = ScilabVariables.addScilabVariablesHandler(new ScilabVariablesJavasci());
        }

        if (name != null && !name.isEmpty()) {
            if (byref) {
                GetScilabVariable.getScilabVariableAsReference(name, id);
            } else {
                GetScilabVariable.getScilabVariable(name, swapRowCol ? 1 : 0, id);
            }
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
     * @param swapRowCol if true the returned data will be stored row by row
     * @return the corresponding ScilabType object
     */
    public static final ScilabType getScilabVariable(String name, boolean swapRowCol) {
        return getScilabVariable(name, swapRowCol, false);
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

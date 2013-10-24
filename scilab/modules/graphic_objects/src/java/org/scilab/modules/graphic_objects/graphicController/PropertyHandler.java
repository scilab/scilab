/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.graphic_objects.graphicController;

import java.util.ArrayList;
import java.util.List;

import org.scilab.modules.types.ScilabType;
import org.scilab.modules.types.ScilabVariables;
import org.scilab.modules.types.ScilabVariablesHandler;

/**
 * Handler to retrieved Scilab variables.
 */
public class PropertyHandler implements ScilabVariablesHandler {

    private List<ScilabType> list = new ArrayList<ScilabType>();
    private int id;

    /**
     * Default constructor
     */
    public PropertyHandler() {
        id = ScilabVariables.addScilabVariablesHandler(this);
    }

    /**
     * @return the handler id
     */
    public int getHandlerId() {
        return id;
    }

    /**
     * Handle the Scilab's variable
     * @param var the Scilab's var
     */
    public synchronized void handle(ScilabType var) {
        list.add(var);
    }

    /**
     * Get the list containing the retrieved arguments
     * @return the list of arguments
     */
    public synchronized List<ScilabType> getArgumentList() {
        List<ScilabType> ret = list;
        list = new ArrayList<ScilabType>();

        return ret;
    }

    public synchronized void cleanArgumentList() {
        list = new ArrayList<ScilabType>();
    }
}

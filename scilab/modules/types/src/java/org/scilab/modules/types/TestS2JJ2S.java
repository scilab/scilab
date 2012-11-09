/*
 * Scilab (http://www.scilab.org/) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.types;

public final class TestS2JJ2S implements ScilabVariablesHandler {

    private static int id = -1;

    static ScilabType var;

    private TestS2JJ2S() {
        id = ScilabVariables.addScilabVariablesHandler(this);
    }

    public static int getId() {
        if (id == -1) {
            new TestS2JJ2S();
        }

        return id;
    }

    public void handle(ScilabType var) {
        TestS2JJ2S.var = var;
    }

    public static void put(int pos) {
        ScilabStackPutter.put(pos, var);
    }
}

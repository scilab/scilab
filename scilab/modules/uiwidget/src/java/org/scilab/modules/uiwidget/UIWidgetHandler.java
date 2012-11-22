/*
 * Uicontrol2 ( http://forge.scilab.org/index.php/p/uicontrol2/ ) - This file is a part of Uicontrol2
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.uiwidget;

import java.util.ArrayList;
import java.util.List;

import org.scilab.modules.types.ScilabType;
import org.scilab.modules.types.ScilabVariables;
import org.scilab.modules.types.ScilabVariablesHandler;

public class UIWidgetHandler implements ScilabVariablesHandler {

    private List<ScilabType> list = new ArrayList<ScilabType>();
    private int id;

    public UIWidgetHandler() {
        id = ScilabVariables.addScilabVariablesHandler(this);
    }

    public int getHandlerId() {
        return id;
    }

    public void handle(ScilabType var) {
        list.add(var);
    }

    public List<ScilabType> getArgumentList() {
        List<ScilabType> ret = list;
        list = new ArrayList<ScilabType>();

        return ret;
    }
}

/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.uiwidget.callback;

import org.scilab.modules.action_binding.InterpreterManagement;
import org.scilab.modules.uiwidget.UIComponent;

public class UIScilabCodeCallback extends UICallback {

    protected static final String BASE = "if exists('gcbo') then %%oldgcbo=gcbo;end;"
                                         + "gcbo=createUIWidgetHandle(int32(%d));%s;"
                                         + "if exists('%%oldgcbo') then gcbo=%%oldgcbo;else clear('gcbo');end;";

    public UIScilabCodeCallback(UIComponent uicomp, String code) {
        super(uicomp, code);
    }

    public UIScilabCodeCallback(String code) {
        super(code);
    }

    public String getStringToEval(UIComponent c) {
        return String.format(BASE, -c.getUid() - 1, code);
    }

    public void exec(final Object ... args) {
        if (component != null) {
            InterpreterManagement.putCommandInScilabQueue(getStringToEval(component));
        }
    }

    public void exec(final UIComponent comp, final Object ... args) {
        if (comp != null) {
            InterpreterManagement.putCommandInScilabQueue(getStringToEval(comp));
        }
    }
}

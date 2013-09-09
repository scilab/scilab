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

public class UIScilabFunctionCallback extends UICallback {

    public UIScilabFunctionCallback(UIComponent uicomp, String code) {
        super(uicomp, code);
    }

    public UIScilabFunctionCallback(String code) {
        super(code);
    }

    public void exec(final Object ... args) {
        if (component != null) {
            InterpreterManagement.putCommandInScilabQueue(getActionString(component, args));
        }
    }

    public void exec(final UIComponent comp, final Object ... args) {
        if (comp != null) {
            InterpreterManagement.putCommandInScilabQueue(getActionString(comp, args));
        }
    }

    /**
     * Get an action string corresponding to the Scilab callback to execute
     * @param uicomp the UIComponent where the event occured
     * @param command the callback name
     * @param args the arguments
     * @return the corresponding Scilab's code
     */
    private final String getActionString(final UIComponent comp, final Object ... args) {
        StringBuilder buffer = new StringBuilder(128);
        buffer.append(code).append("(createUIWidgetHandle(int32(").append(Integer.toString(-comp.getUid() - 1)).append("))");
        if (args.length >= 1) {
            buffer.append(",");
        }

        for (int i = 0; i < args.length - 1; i++) {
            buffer.append(args[i].toString()).append(",");
        }
        if (args.length - 1 >= 0) {
            buffer.append(args[args.length - 1].toString());
        }

        buffer.append(")");

        return buffer.toString();
    }
}

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

package org.scilab.modules.uiwidget;

import org.scilab.modules.action_binding.InterpreterManagement;

public final class UIWidgetTools {

    public static String getActionString(UIComponent uicomp, String command, Object ... args) {
        StringBuilder buffer = new StringBuilder(128);
        buffer.append(command).append("(mlist(['UIWidget','_id'],").append(Integer.toString(uicomp.getUid())).append(")");
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

    public static void execAction(UIComponent uicomp, String command, Object ... args) {
        InterpreterManagement.requestScilabExec(getActionString(uicomp, command, args));
    }
}
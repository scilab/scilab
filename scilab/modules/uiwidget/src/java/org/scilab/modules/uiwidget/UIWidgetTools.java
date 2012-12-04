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

import java.io.File;
import java.util.HashSet;
import java.util.Set;

import org.scilab.modules.action_binding.InterpreterManagement;

public final class UIWidgetTools {

    private static final Set<File> bases = new HashSet<File>();

    public static void addBaseDir(File f) {
        bases.add(f);
    }

    public static File getFile(String f) {
        File ff = new File(f);
        if (ff.isAbsolute()) {
            return ff;
        }

        for (File base : bases) {
            ff = new File(base, f);
            if (ff.exists() && ff.canRead()) {
                if (ff.isAbsolute()) {
                    return ff;
                } else {
                    return ff.getAbsoluteFile();
                }
            }
        }

        return new File(f);
    }

    public static String getActionString(final UIComponent uicomp, final String command, final Object ... args) {
        StringBuilder buffer = new StringBuilder(128);
        buffer.append(command).append("(createUIWidgetHandle(int32(").append(Integer.toString(-uicomp.getUid() - 1)).append("))");
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

    public static void execAction(final UIComponent uicomp, final String command, final Object ... args) {
        if (command != null && !command.isEmpty()) {
            InterpreterManagement.requestScilabExec(getActionString(uicomp, command, args));
        }
    }
}

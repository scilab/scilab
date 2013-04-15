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

/**
 * Few tools
 */
public final class UIWidgetTools {

    private static final Set<File> bases = new HashSet<File>();

    /**
     * Add a base directory where to search resources
     * @param f the base directory
     * @return true if the directory was added and false if it was already added.
     */
    public static boolean addBaseDir(File f) {
        if (bases.contains(f)) {
            return false;
        }

        bases.add(f);
        return true;
    }

    /**
     * Remove a base directory
     * @param f a base directory to remove
     */
    public static void removeBaseDir(File f) {
        bases.remove(f);
    }

    /**
     * Get a file with the given path (search is made according to registered base directories)
     * @param f a file path
     * @return the corresponding file.
     */
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

    /**
     * Get an action string corresponding to the Scilab callback to execute
     * @param uicomp the UIComponent where the event occured
     * @param command the callback name
     * @param args the arguments
     * @return the corresponding Scilab's code
     */
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

    /**
     * Execute a Scilab code in Scilab environment
     * @param uicomp the UIComponent where the event occured
     * @param command the callback name
     * @param args the arguments
     */
    public static void execAction(final UIComponent uicomp, final String command, final Object ... args) {
        if (command != null && !command.isEmpty()) {
            InterpreterManagement.putCommandInScilabQueue(getActionString(uicomp, command, args));
        }
    }
}

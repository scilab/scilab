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

import java.awt.Component;
import java.io.File;
import java.lang.reflect.Method;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

import javax.swing.JComponent;

import org.scilab.modules.action_binding.InterpreterManagement;
import org.scilab.modules.gui.utils.WebBrowser;
import org.scilab.modules.uiwidget.callback.UICallback;

/**
 * Few tools
 */
public final class UIWidgetTools {

    private static final Set<File> bases = new HashSet<File>();
    private static final Map<String, Method> javaActions = new HashMap<String, Method>();

    public enum CallbackType {
        JAVA, SCI_FUN, SCI_CODE, URL;
    }

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
     * Find and invoke a Java callback of the form: org.foo.bar.mymethod, where org.foo.bar is the base class and mymethod the name of a static method with zero or one argument which can be a UIComponent, a Component or a JComponent.
     * @param uicomp the base uicomponent
     * @param command the command callback
     * @return true if the invokation succeeded.
     */
    public static boolean invokeJavaCallback(final UIComponent uicomp, final String command) {
        Method method = javaActions.get(command);
        if (method == null) {
            int pos = command.lastIndexOf('.');
            if (pos != -1) {
                String methodName = command.substring(pos + 1);
                String className = command.substring(0, pos);
                Class clazz = null;
                try {
                    clazz = Class.forName(className);
                    Method[] methods = clazz.getMethods();

                    for (Method m : methods) {
                        if (m.getName().equals(methodName)) {
                            Class[] argsType = m.getParameterTypes();
                            if (argsType == null || argsType.length == 0 || JComponent.class.isAssignableFrom(argsType[0]) || Component.class.isAssignableFrom(argsType[0]) || UIComponent.class.isAssignableFrom(argsType[0])) {
                                method = m;
                                break;
                            }
                        }
                    }
                } catch (ClassNotFoundException e) {
                    return false;
                }
            }

            if (method != null) {
                javaActions.put(command, method);
            }
        }

        if (method != null) {
            Class[] argsType = method.getParameterTypes();
            try {
                if (argsType == null || argsType.length == 0) {
                    method.invoke(null);
                } else if (JComponent.class.isAssignableFrom(argsType[0]) || Component.class.isAssignableFrom(argsType[0])) {
                    method.invoke(null, uicomp.getModifiableComponent());
                } else {
                    method.invoke(null, uicomp);
                }

                return true;
            } catch (Exception e) {
                return false;
            }
        } else {
            return false;
        }
    }

    /**
     * Guess the callback type.
     * If the command contains only letters and dot it would be considered as a Java callback and if it contains anything else that an allowed character in Scilab identifier it would be considered as a Scilab code.
     * @param command the callback command
     * @return the callback type
     */
    public static CallbackType guessCallbackType(final String command) {
        CallbackType type = CallbackType.SCI_FUN;
        for (int i = 0; i < command.length(); i++) {
            char c = command.charAt(i);
            if (!Character.isLetter(c)) {
                if (c != '.' && !Character.isDigit(c) && c != '_' && c != '%' && c != '#' && c != '!' && c != '?' && c != '$') {
                    type = CallbackType.SCI_CODE;
                    break;
                } else {
                    type = CallbackType.JAVA;
                }
            }
        }

        if (type == CallbackType.SCI_CODE && (command.startsWith("http://") || command.startsWith("file://") || command.startsWith("https://") || command.startsWith("ftp://") || command.startsWith("mailto://"))) {
            type = CallbackType.URL;
        }

        return type;
    }

    /**
     * Execute a Scilab code in Scilab environment
     * @param uicomp the UIComponent where the event occured
     * @param command the callback name
     * @param args the arguments
     */
    public static void execAction(final UIComponent uicomp, final String command, final Object ... args) {
        if (command != null && !command.isEmpty()) {
            CallbackType type = guessCallbackType(command);
            boolean javaFailed = true;
            switch (type) {
                case JAVA:
                    if (!invokeJavaCallback(uicomp, command)) {
                        InterpreterManagement.putCommandInScilabQueue(getActionString(uicomp, command, args));
                    }
                    break;
                case SCI_FUN:
                    InterpreterManagement.putCommandInScilabQueue(getActionString(uicomp, command, args));
                    break;
                case SCI_CODE:
                    String callback = "if exists(\"gcbo\") then %oldgcbo = gcbo; end;"
                                      + "gcbo = createUIWidgetHandle(int32("
                                      + Integer.toString(-uicomp.getUid() - 1)
                                      + "));"
                                      + command
                                      + ";if exists(\"%oldgcbo\") then gcbo = %oldgcbo; else clear gcbo; end;";

                    InterpreterManagement.putCommandInScilabQueue(callback);
                    break;
                case URL:
                    WebBrowser.openUrl(command);
                    break;
            }
        }
    }

    /**
     * Execute a Scilab code in Scilab environment
     * @param uicomp the UIComponent where the event occured
     * @param command the callback name
     * @param args the arguments
     */
    public static void execAction(final UICallback callback, final Object ... args) {
        if (callback != null) {
            callback.exec(args);
        }
    }

    /**
     * Execute a Scilab code in Scilab environment
     * @param uicomp the UIComponent where the event occured
     * @param command the callback name
     * @param args the arguments
     */
    public static void execAction(final UIComponent uicomp, final UICallback callback, final Object ... args) {
        if (callback != null) {
            callback.exec(uicomp, args);
        }
    }
}

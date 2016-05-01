/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
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
package org.scilab.modules.gui.events.callback;

import java.awt.event.ActionEvent;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.HashMap;
import java.util.Map;

import org.scilab.modules.graphic_objects.graphicObject.CallBack;

/**
 * Generic class to hide callback management
 * @author Bruno JOFRET
 */
public abstract class JavaCallBack extends CommonCallBack {

    private static final long serialVersionUID = -6513057558261299432L;
    private static final String DOT = ".";
    private static final Map<Class, ArgConverter> converters = new HashMap<Class, ArgConverter>();

    static {
        converters.put(int.class, new ArgConverter() {
            public Object convert(String s) {
                try {
                    return Integer.decode(s);
                } catch (Exception e) {
                    return null;
                }
            }
        });
    }

    /**
     * @param command : the command to execute.
     */
    private JavaCallBack(String command) {
        super(command, CallBack.UNTYPED);
    }

    /**
     * Callback Factory to easily create a callback
     * just like in scilab.
     * @param command : the command to execute.
     * @return a usable Java callback
     */
    public static JavaCallBack create(String command) {
        return (new JavaCallBack(command) {
            private static final long serialVersionUID = 3116709592411260416L;

            public void callBack() {
                try {
                    String cmd = getCommand();
                    String args = null;
                    int fpindex = cmd.indexOf('(');
                    int lpindex = cmd.lastIndexOf(')');
                    if (fpindex != -1 && lpindex != -1) {
                        args = cmd.substring(fpindex + 1, lpindex);
                        cmd = cmd.substring(0, fpindex);
                    }

                    int lastPoint = cmd.lastIndexOf(DOT);
                    Class invokedClass;
                    invokedClass = Class.forName(cmd.substring(0, lastPoint));
                    String methName = cmd.substring(lastPoint + 1);

                    if (args == null) {
                        Method runMe = invokedClass.getMethod(methName);
                        // Only able to launch method Class.
                        runMe.invoke(invokedClass.getClass(), (Object[]) null);
                    } else {
                        Method[] meths = invokedClass.getMethods();
                        for (Method m : meths) {
                            if (m.getName().equals(methName)) {
                                m.invoke(invokedClass.getClass(), getArguments(m, args));
                                break;
                            }
                        }
                    }
                } catch (ClassNotFoundException e) {
                    e.printStackTrace();
                } catch (SecurityException e) {
                    e.printStackTrace();
                } catch (NoSuchMethodException e) {
                    e.printStackTrace();
                } catch (IllegalArgumentException e) {
                    e.printStackTrace();
                } catch (IllegalAccessException e) {
                    e.printStackTrace();
                } catch (InvocationTargetException e) {
                    e.printStackTrace();
                }
            }
        });
    }

    /**
     * Callback Factory to easily create a callback
     * just like in scilab.
     * @param command : the command to execute.
     * @return a usable Java callback
     */
    public static CommonCallBack createOutOfXclickAndXgetmouse(String command) {
        return (new JavaCallBack(command) {
            private static final long serialVersionUID = -7456188185148029043L;

            public void callBack() {
                try {
                    int lastPoint = getCommand().lastIndexOf(DOT);
                    Class invokedClass;
                    invokedClass = Class.forName(getCommand().substring(0, lastPoint));
                    Method runMe = invokedClass.getMethod(getCommand().substring(lastPoint + 1));
                    // Only able to launch method Class.
                    runMe.invoke(invokedClass.getClass(), (Object[]) null);
                } catch (ClassNotFoundException e) {
                    e.printStackTrace();
                } catch (NoSuchMethodException e) {
                    e.printStackTrace();
                } catch (IllegalAccessException e) {
                    e.printStackTrace();
                } catch (InvocationTargetException e) {
                    e.printStackTrace();
                }
            }

            /**
             * To match the standard Java Action management.
             * @see java.awt.event.ActionListener#actionPerformed(java.awt.event.ActionEvent)
             * @param e The event that launch the callback.
             */
            public void actionPerformed(ActionEvent e) {
                callBack();
            }
        });
    }

    private interface ArgConverter {
        public Object convert(String s);
    }

    private static Object[] getArguments(Method m, String arg) {
        Class<?>[] types = m.getParameterTypes();
        if (types.length == 0 || arg == null || arg.isEmpty()) {
            return null;
        }

        String[] args = arg.split(",");
        final int l = Math.min(types.length, args.length);
        Object[] _args = new Object[l];
        for (int i = 0; i < l; i++) {
            if (converters.containsKey(types[i])) {
                _args[i] = converters.get(types[i]).convert(args[i].trim());
            }
        }

        return _args;
    }
}

/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.gui.events.callback;

import java.awt.event.ActionEvent;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

import org.scilab.modules.graphic_objects.graphicObject.CallBack;

/**
 * Generic class to hide callback management
 * @author Bruno JOFRET
 */
public abstract class JavaCallBack extends CommonCallBack {

    private static final long serialVersionUID = -6513057558261299432L;

    private static final String DOT = ".";

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
                    int lastPoint = getCommand().lastIndexOf(DOT);
                    Class invokedClass;
                    invokedClass = Class.forName(getCommand().substring(0, lastPoint));
                    Method runMe;
                    runMe = invokedClass.getMethod(getCommand().substring(lastPoint + 1));
                    // Only able to launch method Class.
                    runMe.invoke(invokedClass.getClass(), (Object[]) null);
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
}

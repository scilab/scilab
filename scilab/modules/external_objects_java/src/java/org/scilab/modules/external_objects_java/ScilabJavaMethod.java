/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - 2011 - Calixte DENIZET <calixte@contrib.scilab.org>
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

package org.scilab.modules.external_objects_java;

import java.awt.Component;
import java.beans.IntrospectionException;
import java.beans.Introspector;
import java.beans.MethodDescriptor;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;

import javax.swing.SwingUtilities;

/**
 * A method wrapper
 *
 * @author Calixte DENIZET
 */
@SuppressWarnings("serial")
public final class ScilabJavaMethod {

    /**
     * @param obj the object where invoking method
     * @param returnType the class of the result
     * @param args the ids of arguments
     * @return the resulting object
     */
    public static Object invoke(String name, Class clazz, Object obj, Class[] returnType, int[] args) throws ScilabJavaException {
        int nbargs = args.length;
        Class[] cl = new Class[nbargs];
        Object[] argsO = new Object[nbargs];

        for (int i = 0; i < nbargs; i++) {
            argsO[i] = ScilabJavaObject.arraySJO[args[i]].object;
            cl[i] = ScilabJavaObject.arraySJO[args[i]].clazz;

            if (argsO[i] != null && argsO[i] == cl[i]) {
                cl[i] = argsO[i].getClass();
            }
        }

        return call(name, clazz, obj, returnType, argsO, cl);
    }

    /**
     * @param obj the object where invoking method
     * @param returnType the class of the result
     * @param args the Object arguments
     * @param argsClass the Class of the arguments
     * @return the resulting object
     */
    public static Object call(final String name, final Class clazz, final Object obj, final Class[] returnType, final Object[] args, final Class[] argsClass) throws ScilabJavaException {
        MethodDescriptor[] mdesc = null;
        try {
            mdesc = Introspector.getBeanInfo(clazz).getMethodDescriptors();
            if (mdesc == null) {
                throw new ScilabJavaException("No method " + name + " in the class " + clazz.getName() + " or bad arguments type.");
            }
        } catch (IntrospectionException e) {
            throw new ScilabJavaException("Impossible to get method names on the class " + clazz.getName());
        }

        try {
            final Object[] info = FunctionArguments.findMethod(name, mdesc, argsClass, args);
            final Method meth = (Method) info[0];
            final Class returned = meth.getReturnType();
            Object ret = null;
            final Object[] _args;

            if (!meth.isAccessible()) {
                try {
                    meth.setAccessible(true);
                } catch (SecurityException e) { }
            }

            if (info.length == 2) {
                // Method with variable arguments, so they have been modified and are the second element of the returned array
                _args = (Object[]) info[1];
            } else {
                _args = args;
            }

            if (Modifier.isStatic(meth.getModifiers())) {
                ret = meth.invoke(null, _args);
            } else {
                if (Component.class.isAssignableFrom(obj.getClass())) {
                    if (returned == Void.TYPE) {
                        if (SwingUtilities.isEventDispatchThread()) {
                            ret = meth.invoke(obj, _args);
                        } else {
                            SwingUtilities.invokeLater(new Runnable() {

                                public void run() {
                                    try {
                                        meth.invoke(obj, _args);
                                    } catch (Exception e) {
                                        System.err.println(e);
                                    }
                                }
                            });
                        }
                    } else {
                        if (SwingUtilities.isEventDispatchThread()) {
                            ret = meth.invoke(obj, _args);
                        } else {
                            final Object[] ref = new Object[1];
                            SwingUtilities.invokeAndWait(new Runnable() {
                                public void run() {
                                    try {
                                        ref[0] = meth.invoke(obj, _args);
                                    } catch (Exception e) {
                                        System.err.println(e);
                                    }
                                }
                            });

                            ret = ref[0];
                        }
                    }
                } else {
                    ret = meth.invoke(obj, _args);
                }
            }

            returnType[0] = ret != null ? ret.getClass() : returned;

            if (returned == double.class || returned == int.class ||
                    returned == short.class || returned == float.class ||
                    returned == boolean.class || returned == char.class ||
                    returned == byte.class || returned == long.class) {
                returnType[0] = returned;
            }
            return ret;
        } catch (IllegalAccessException e) {
            throw new ScilabJavaException("Illegal access to the method " + name + ".");
        } catch (IllegalArgumentException e) {
            e.printStackTrace();
            throw new ScilabJavaException("Illegal argument in the method " + name + ": \n" + e.getMessage());
        } catch (NullPointerException e) {
            throw new ScilabJavaException("The method " + name + " is called on a null object.");
        } catch (ExceptionInInitializerError e) {
            throw new ScilabJavaException("Initializer error with method " + name + ":\n" + e.getMessage());
        } catch (InvocationTargetException e) {
            throw new ScilabJavaException("An exception has been thrown in calling the method " + name + ":\n" + e.getCause().toString());
        } catch (NoSuchMethodException e) {
            throw new ScilabJavaException("No method " + name + " in the class " + clazz.getName() + " or bad arguments type.");
        } catch (InterruptedException e) {
            throw new ScilabJavaException("EDT has been interrupted...");
        }
    }
}

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

package org.scilab.modules.graphic_objects.graphicObject;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

import javax.swing.SwingUtilities;

import org.scilab.modules.graphic_objects.converters.ScilabTypeConverters;
import org.scilab.modules.graphic_objects.converters.StringConverters;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject.UpdateStatus;
import org.scilab.modules.types.ScilabType;

/**
 * Tools to access to the UIComponents and their properties
 */
public final class GraphicObjectAccessTools {

    /**
     * Add a child to a parent
     * @param parent the parent UIComponent
     * @param uicomp the child
     */

    /**
     * Set a property in using reflection
     * @param obj the object
     * @param name the property name
     * @param value a String containing the value (e.g. value="1.23")
     */
    public static final void setPropertyViaReflection(final Object obj, final String name, final String value) throws GraphicObjectException {
        Class <? extends Object > clazz = obj.getClass();
        String methName = getSetterName(name);
        final Method method = GraphicObjectMethodFinder.findSetter(methName, clazz);

        if (method == null) {
            throw new GraphicObjectException("No attribute " + name + " in " + clazz.getSimpleName());
        }

        GraphicObjectAccessTools.execOnEDT(new Runnable() {
            public void run() {
                try {
                    GraphicObjectAccessTools.invokeSetter(method, obj, value);
                } catch (Exception e) {
                    System.err.println(e);
                }
            }
        });
    }

    /**
     * Set a property in using reflection
     * @param obj the object
     * @param name the property name
     * @param value a ScilabType value
     */
    public static final void setPropertyViaReflection(final Object obj, final String name, final ScilabType value) throws GraphicObjectException {
        Class <? extends Object > clazz = obj.getClass();
        String methName = getSetterName(name);
        final Method method = GraphicObjectMethodFinder.findSetter(methName, clazz, value);

        if (method == null) {
            throw new GraphicObjectException("No attribute " + name + " in " + clazz.getSimpleName());
        }

        GraphicObjectAccessTools.execOnEDT(new Runnable() {
            public void run() {
                try {
                    GraphicObjectAccessTools.invokeSetter(method, obj, value);
                } catch (Exception e) {
                    System.err.println(e);
                }
            }
        });
    }

    /**
     * Invoke a setter
     * @param m the method
     * @param obj the object where to set
     * @param value a String value
     */
    public static final void invokeSetter(final Method m, final Object obj, final String value) throws GraphicObjectException {
        try {
            m.invoke(obj, StringConverters.getObjectFromValue(m.getParameterTypes()[0], value));
        } catch (IllegalAccessException e) {
            // Should not occur
            System.err.println(e);
        } catch (ExceptionInInitializerError e) {
            // Should not occur
            System.err.println(e);
        } catch (IllegalArgumentException e) {
            // Should not occur
            System.err.println(e);
        } catch (NullPointerException e) {
            // Should not occur
            System.err.println(e);
        } catch (InvocationTargetException e) {
            e.getCause().printStackTrace();
            throw new GraphicObjectException("Error in calling " + m.getName() + " in class " + obj.getClass().getName());
        }
    }

    /**
     * Invoke a setter
     * @param m the method
     * @param obj the object where to set
     * @param value a ScilabType value
     */
    public static final UpdateStatus invokeSetter(final Method m, final Object obj, final ScilabType value) throws GraphicObjectException {
        try {
            Class<?> argType = m.getParameterTypes()[0];
            if (argType.isAssignableFrom(value.getClass())) {
                return (UpdateStatus)m.invoke(obj, value);
            } else {
                Object objValue = ScilabTypeConverters.getObjectFromValue(argType, value);
                if (objValue == null) {
                    throw new GraphicObjectException("Error in calling method " + m.getName() + " in class " + obj.getClass().getName());
                }

                return (UpdateStatus)m.invoke(obj, objValue);
            }
        } catch (IllegalAccessException e) {
            // Should not occur
            System.err.println(e);
        } catch (ExceptionInInitializerError e) {
            // Should not occur
            System.err.println(e);
        } catch (IllegalArgumentException e) {
            // Should not occur
            System.err.println(e);
        } catch (NullPointerException e) {
            // Should not occur
            System.err.println(e);
        } catch (InvocationTargetException e) {
            e.getCause().printStackTrace();
            throw new GraphicObjectException("Error in calling method " + m.getName() + " in class " + obj.getClass().getName());
        }
        return UpdateStatus.Fail;
    }

    /**
     * Get a property in using reflection
     * @param obj the object
     * @param name the property name
     * @return the property value
     */
    public static final Object getPropertyViaReflection(final Object obj, final String name) throws GraphicObjectException {
        Class <? extends Object > clazz = obj.getClass();
        String methName = getGetterName(name);
        Method method = GraphicObjectMethodFinder.findGetter(methName, clazz);
        if (method == null) {
            methName = GraphicObjectAccessTools.getIsGetterName(name);
            method = GraphicObjectMethodFinder.findGetter(methName, clazz);
        }

        if (method == null) {
            throw new GraphicObjectException("No attribute " + name + " in " + clazz.getSimpleName());
        }

        if (SwingUtilities.isEventDispatchThread()) {
            return invokeGetter(method, obj);
        } else {
            final Method m = method;
            final Object[] ptr = new Object[1];

            try {
                SwingUtilities.invokeAndWait(new Runnable() {
                    public void run() {
                        try {
                            ptr[0] = invokeGetter(m, obj);
                        } catch (Exception e) {
                            System.err.println(e);
                        }
                    }
                });
            } catch (InterruptedException e) {
                throw new GraphicObjectException("Error in getting property " + name);
            } catch (InvocationTargetException e) {
                e.getCause().printStackTrace();
                throw new GraphicObjectException("Error in getting property " + name);
            }

            return ptr[0];
        }
    }

    /**
     * Invoke a getter
     * @param m the method
     * @param obj the object where to set
     * @return the property value
     */
    public static final Object invokeGetter(final Method m, final Object obj) throws GraphicObjectException {
        try {
            return m.invoke(obj);
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        } catch (ExceptionInInitializerError e) {
            e.printStackTrace();
        } catch (IllegalArgumentException e) {
            e.printStackTrace();
        } catch (NullPointerException e) {
            e.printStackTrace();
        } catch (InvocationTargetException e) {
            e.getCause().printStackTrace();
            throw new GraphicObjectException("Error in calling the method " + m.getName() + " in class " + obj.getClass().getName());
        }

        return null;
    }

    /**
     * Execute a Runnable on the EDT
     * @param runnable the runnable
     */
    public static final void execOnEDT(final Runnable runnable) {
        if (SwingUtilities.isEventDispatchThread()) {
            runnable.run();
        } else {
            SwingUtilities.invokeLater(runnable);
        }
    }

    /**
     * Get a setter name (e.g. "setFoo" for "foo")
     * @param key the base of the setter name
     * @return a setter name
     */
    public static final String getSetterName(final String key) throws GraphicObjectException {
        return getMethodName("set", key);
    }

    /**
     * Get a getter name (e.g. "getFoo" for "foo")
     * @param key the base of the getter name
     * @return a getter name
     */
    public static final String getGetterName(final String key) throws GraphicObjectException {
        return getMethodName("get", key);
    }

    /**
     * Get a "is"-getter name (e.g. "isFoo" for "foo")
     * @param key the base of the getter name
     * @return a getter name
     */
    public static String getIsGetterName(final String key) throws GraphicObjectException {
        return getMethodName("is", key);
    }

    /**
     * Get a method name
     * @param prefix the prefix to use for the method name
     * @param key the base of the name
     * @return a name
     */
    public static final String getMethodName(final String prefix, final String key) throws GraphicObjectException {
        if (key == null || key.isEmpty()) {
            throw new GraphicObjectException("Invalid attribute name: cannot be empty");
        }
        StringBuilder buffer = new StringBuilder(32);
        char[] chars = key.toCharArray();

        buffer.append(prefix).append(Character.toUpperCase(chars[0]));
        int start = 1;
        for (int i = 1; i < chars.length; i++) {
            if (chars[i] == '-' || chars[i] == '_') {
                if (start < i) {
                    buffer.append(chars, start, i - start);
                }
                if (i + 1 < chars.length) {
                    buffer.append(Character.toUpperCase(chars[i + 1]));
                    start = i + 2;
                } else {
                    start = i + 1;
                }
                i++;
            }
        }

        if (start < chars.length) {
            buffer.append(chars, start, chars.length - start);
        }

        return buffer.toString();
    }
}

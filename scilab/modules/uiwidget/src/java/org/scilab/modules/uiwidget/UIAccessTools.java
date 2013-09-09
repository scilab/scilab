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

import java.lang.annotation.Annotation;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.Set;
import java.util.TreeSet;

import javax.swing.JPopupMenu;
import javax.swing.SwingUtilities;

import org.scilab.modules.types.ScilabType;
import org.scilab.modules.uiwidget.components.TextData;
import org.scilab.modules.uiwidget.components.UIKeymap;

/**
 * Tools to access to the UIComponents and their properties
 */
public final class UIAccessTools {

    /**
     * Add a child to a parent
     * @param parent the parent UIComponent
     * @param uicomp the child
     */
    public static final void add(final UIComponent parent, final UIComponent uicomp) throws UIWidgetException {
        final Object comp = uicomp.getComponent();
        if (uicomp instanceof UIListener) {
            parent.addListener((UIListener) uicomp);
            uicomp.updateDependencies(parent);
        } else if (comp instanceof JPopupMenu) {
            parent.addPopupMenu((JPopupMenu) comp);
            uicomp.updateDependencies(parent);
        } else if (uicomp instanceof TextData) {
            String text = ((TextData) uicomp).getText();
            parent.setProperty("text", text);
            uicomp.updateDependencies(parent);
        } else if (uicomp instanceof UIKeymap) {
            ((UIKeymap) uicomp).update();
            uicomp.updateDependencies(parent);
        } else {
            Object base = parent;
            Method method = method = UIMethodFinder.findAdder(base.getClass(), comp.getClass());

            if (method == null) {
                base = parent.getComponent();
                method = UIMethodFinder.findAdder(base.getClass(), comp.getClass());
            }

            if (method == null) {
                throw new UIWidgetException("Cannot add a " + comp.getClass().getName() + " to a " + parent.getClass().getName());
            }

            final Method m = method;
            final Object b = base;

            execOnEDT(new Runnable() {
                public void run() {
                    try {
                        invokeAdder(m, b, comp);
                        uicomp.updateDependencies(parent);
                    } catch (Exception e) {
                        System.err.println(e);
                    }
                }
            });
        }
    }

    /**
     * Invoke an adder
     * @param m the adder method
     * @param base the base object
     * @param obj the adder argument
     */
    private static final void invokeAdder(final Method m, final Object base, final Object obj) throws UIWidgetException {
        try {
            m.invoke(base, obj);
        } catch (Exception e) {
            System.err.println(e);
        }
    }

    /**
     * Set a property in using reflection
     * @param obj the object
     * @param name the property name
     * @param value a String containing the value (e.g. value="1.23")
     */
    public static final void setPropertyViaReflection(final Object obj, final String name, final String value) throws UIWidgetException {
        Class clazz = obj.getClass();
        String methName = getSetterName(name);
        final Method method = UIMethodFinder.findSetter(methName, clazz);

        if (method == null) {
            throw new UIWidgetException("No attribute " + name + " in " + clazz.getSimpleName());
        }

        UIAccessTools.execOnEDT(new Runnable() {
            public void run() {
                try {
                    UIAccessTools.invokeSetter(method, obj, value);
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
    public static final void setPropertyViaReflection(final Object obj, final String name, final ScilabType value) throws UIWidgetException {
        Class clazz = obj.getClass();
        String methName = getSetterName(name);
        final Method method = UIMethodFinder.findSetter(methName, clazz, value);

        if (method == null) {
            throw new UIWidgetException("No attribute " + name + " in " + clazz.getSimpleName());
        }

        UIAccessTools.execOnEDT(new Runnable() {
            public void run() {
                try {
                    UIAccessTools.invokeSetter(method, obj, value);
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
    public static final void invokeSetter(final Method m, final Object obj, final String value) throws UIWidgetException {
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
            throw new UIWidgetException("Error in calling " + m.getName() + " in class " + obj.getClass().getName());
        }
    }

    /**
     * Invoke a setter
     * @param m the method
     * @param obj the object where to set
     * @param value a ScilabType value
     */
    public static final void invokeSetter(final Method m, final Object obj, final ScilabType value) throws UIWidgetException {
        try {
            Class argType = m.getParameterTypes()[0];
            if (argType == value.getClass()) {
                m.invoke(obj, value);
            } else {
                m.invoke(obj, ScilabTypeConverters.getObjectFromValue(argType, value));
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
            throw new UIWidgetException("Error in calling method " + m.getName() + " in class " + obj.getClass().getName());
        }
    }

    /**
     * Get a property in using reflection
     * @param obj the object
     * @param name the property name
     * @return the property value
     */
    public static final Object getPropertyViaReflection(final Object obj, final String name) throws UIWidgetException {
        Class clazz = obj.getClass();
        String methName = getGetterName(name);
        Method method = UIMethodFinder.findGetter(methName, clazz);
        if (method == null) {
            methName = UIAccessTools.getIsGetterName(name);
            method = UIMethodFinder.findGetter(methName, clazz);
        }

        if (method == null) {
            throw new UIWidgetException("No attribute " + name + " in " + clazz.getSimpleName());
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
                throw new UIWidgetException("Error in getting property " + name);
            } catch (InvocationTargetException e) {
                e.getCause().printStackTrace();
                throw new UIWidgetException("Error in getting property " + name);
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
    public static final Object invokeGetter(final Method m, final Object obj) throws UIWidgetException {
        try {
            return m.invoke(obj);
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
            //System.err.println(e.getCause());
            e.getCause().printStackTrace();
            throw new UIWidgetException("Error in calling the method " + m.getName() + " in class " + obj.getClass().getName());
        }

        return null;
    }

    /**
     * Create a new instance in calling the newInstance method of the UIComponent
     * @param uicomp the component to instanciate
     * @param attributes the UIComponent attributes
     * @return a Set of the unused attributes.
     */
    public static final Set<String> createNewInstance(final UIComponent uicomp, final ConvertableMap attributes) throws UIWidgetException {
        final Set<String> uselessAttrs = new TreeSet<String>(attributes.keySet());
        Method method = UIMethodFinder.findNewer(uicomp.getClass());
        Annotation annotation = null;
        Object[] args = null;

        if (method == null) {
            try {
                method = uicomp.getClass().getMethod("newInstance");
            } catch (NoSuchMethodException e) { }

            if (!method.isAccessible()) {
                method.setAccessible(true);
            }
        } else {
            annotation = method.getAnnotation(UIComponentAnnotation.class);
        }

        if (annotation != null) {
            String[] argsName = ((UIComponentAnnotation) annotation).attributes();
            Class[] argsType = method.getParameterTypes();

            if (argsName.length != argsType.length) {
                throw new UIWidgetException("Invalid annotation: the number of attributes must be the same as the number of arguments");
            }

            args = new Object[argsType.length];

            for (int i = 0; i < args.length; i++) {
                args[i] = attributes.get(argsType[i], argsName[i]);
                uselessAttrs.remove(argsName[i]);
            }
        } else {
            args = new Object[0];
        }

        final Object[] fargs = args;
        final Method m = method;

        if (SwingUtilities.isEventDispatchThread()) {
            try {
                uicomp.setComponent(m.invoke(uicomp, fargs));
            } catch (InvocationTargetException e) {
                e.getCause().printStackTrace();
            } catch (Exception e) {
                System.err.println(e);
            }
        } else {
            try {
                SwingUtilities.invokeAndWait(new Runnable() {
                    public void run() {
                        try {
                            uicomp.setComponent(m.invoke(uicomp, fargs));
                        } catch (InvocationTargetException e) {
                            e.getCause().printStackTrace();
                        } catch (Exception e) {
                            System.err.println(e);
                        }
                    }
                });
            } catch (Exception e) {
                System.err.println(e);
            }
        }

        return uselessAttrs;
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
    public static final String getSetterName(final String key) throws UIWidgetException {
        return getMethodName("set", key);
    }

    /**
     * Get a getter name (e.g. "getFoo" for "foo")
     * @param key the base of the getter name
     * @return a getter name
     */
    public static final String getGetterName(final String key) throws UIWidgetException {
        return getMethodName("get", key);
    }

    /**
     * Get a "is"-getter name (e.g. "isFoo" for "foo")
     * @param key the base of the getter name
     * @return a getter name
     */
    public static String getIsGetterName(final String key) throws UIWidgetException {
        return getMethodName("is", key);
    }

    /**
     * Get a method name
     * @param prefix the prefix to use for the method name
     * @param key the base of the name
     * @return a name
     */
    public static final String getMethodName(final String prefix, final String key) throws UIWidgetException {
        if (key == null || key.isEmpty()) {
            throw new UIWidgetException("Invalid attribute name: cannot be empty");
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

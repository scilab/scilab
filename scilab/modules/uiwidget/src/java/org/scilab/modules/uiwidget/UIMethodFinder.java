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

import java.lang.reflect.Method;
import java.lang.reflect.Modifier;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.scilab.modules.types.ScilabType;

/**
 * TODO:
 * on peut ameliorer les choses pr determiner la "bonne" methode a utiliser:
 * par ex: si on a foo(A), foo(B), ... declarees et que l'on cherche la foo appeler
 * pr un argument de type X, on calcule la distance de X a A, B, ... (dist(X,A)=le nombre de
 * de derivation pr passer de A a X) puis on cherche la methode qui minimise cette distance.
 **/

/**
 * Class to find the accessible methods to get/set properties on the UIComponents
 */
public final class UIMethodFinder {

    private static final Map<Class, MethodsInfo> info = new HashMap<Class, MethodsInfo>();

    /**
     * Clear the cache
     */
    public static void clearCache() {
        for (Map.Entry<Class, MethodsInfo> entry : info.entrySet()) {
            entry.getValue().clear();
        }

        info.clear();
    }

    /**
     * Find a adder
     * @param baseClass the class where to search
     * @param clazz the class of the argument of the adder method
     * @return the corresponding method
     */
    public static Method findAdder(final Class baseClass, final Class clazz) {
        MethodsInfo i = info.get(baseClass);
        if (i == null) {
            i = getInterestingMethods(baseClass);
            info.put(baseClass, i);
        }

        for (Method m : i.adders) {
            if (m.getParameterTypes()[0].isAssignableFrom(clazz)) {
                return m;
            }
        }

        return null;
    }

    /**
     * Get a setter
     * @param baseClass the class where to search
     * @param out a map containing method name -- method
     */
    public static void getSetter(final Class baseClass, final Map<String, Method> out) {
        MethodsInfo i = info.get(baseClass);
        if (i == null) {
            i = getInterestingMethods(baseClass);
            info.put(baseClass, i);
        }

        if (i.setters != null) {
            for (Map.Entry<String, List<Method>> entry : i.setters.entrySet()) {
                List<Method> list = entry.getValue();
                if (list != null && !list.isEmpty()) {
                    out.put(entry.getKey(), list.get(0));
                }
            }
        }
    }

    /**
     * Find a setter
     * @param name the base setter name (e.g. foo for setFoo)
     * @param baseClass the class where to search
     * @return the corresponding method
     */
    public static List<Method> getSetterMethods(final String name, final Class baseClass) {
        MethodsInfo i = info.get(baseClass);
        if (i == null) {
            i = getInterestingMethods(baseClass);
            info.put(baseClass, i);
        }

        List<Method> methods = i.setters.get(name);
        if (methods == null || methods.isEmpty()) {
            // The name is not good but maybe it is just a case problem.
            for (Map.Entry<String, List<Method>> entry : i.setters.entrySet()) {
                if (name.equalsIgnoreCase(entry.getKey())) {
                    methods = entry.getValue();
                    break;
                }
            }

            if (methods != null) {
                i.setters.put(name, methods);
            } else {
                return null;
            }
        }

        return methods;
    }

    /**
     * Find a setter
     * @param name the base setter name (e.g. foo for setFoo)
     * @param baseClass the class where to search
     * @return the corresponding method
     */
    public static Method findSetter(final String name, final Class baseClass) {
        List<Method> methods = getSetterMethods(name, baseClass);

        if (methods != null) {
            return methods.get(0);
        }

        return null;
    }

    /**
     * Find a setter
     * @param name the base setter name (e.g. foo for setFoo)
     * @param baseClass the class where to search
     * @return the corresponding method
     */
    public static Method findSetter(final String name, final Class baseClass, final ScilabType arg) {
        List<Method> methods = getSetterMethods(name, baseClass);

        if (methods != null) {
            final Class argClass = arg.getClass();
            final int r = arg.getHeight();
            final int c = arg.getWidth();
            Method potential = null;
            for (Method m : methods) {
                Class argType = m.getParameterTypes()[0];
                if (argType.isAssignableFrom(arg.getClass())) {
                    return m;
                }
                Class<? extends ScilabType> st = ScilabTypeConverters.preferred.get(argType);
                if (st == argClass) {
                    if (!argType.isArray()) {
                        if (r == 1 && c == 1) {
                            return m;
                        } else {
                            potential = m;
                        }
                    } else {
                        if (argType.getComponentType().isArray()) {
                            if (r != 1 && c != 1) {
                                return m;
                            } else {
                                potential = m;
                            }
                        } else {
                            if (r == 1 || c == 1) {
                                return m;
                            } else {
                                potential = m;
                            }
                        }
                    }
                }
            }

            if (potential == null) {
                return methods.get(0);
            } else {
                return potential;
            }
        }

        return null;
    }

    /**
     * Find a getter
     * @param name the base getter name (e.g. foo for getFoo)
     * @param baseClass the class where to search
     * @return the corresponding method
     */
    public static List<Method> getGetterMethods(final String name, final Class baseClass) {
        MethodsInfo i = info.get(baseClass);
        if (i == null) {
            i = getInterestingMethods(baseClass);
            info.put(baseClass, i);
        }

        List<Method> methods = i.getters.get(name);
        if (methods == null || methods.isEmpty()) {
            // The name is not good but maybe it is just a case problem.
            for (Map.Entry<String, List<Method>> entry : i.getters.entrySet()) {
                if (name.equalsIgnoreCase(entry.getKey())) {
                    methods = entry.getValue();
                    break;
                }
            }

            if (methods != null) {
                i.getters.put(name, methods);
            } else {
                return null;
            }
        }

        return methods;
    }

    /**
     * Find a getter
     * @param name the base getter name (e.g. foo for getFoo)
     * @param baseClass the class where to search
     * @return the corresponding method
     */
    public static Method findGetter(final String name, final Class baseClass) {
        List<Method> methods = getGetterMethods(name, baseClass);
        if (methods != null) {
            return methods.get(0);
        }

        return null;
    }

    /**
     * Find a newer (newInstance method annotated with UIComponentAnnotation)
     * @param baseClass the class where to search
     * @return the corresponding method
     */
    public static Method findNewer(final Class baseClass) {
        MethodsInfo i = info.get(baseClass);
        if (i == null) {
            i = getInterestingMethods(baseClass);
            info.put(baseClass, i);
        }

        return i.newers.isEmpty() ? null : i.newers.get(0);
    }

    private static final MethodsInfo getInterestingMethods(final Class clazz) {
        Class base = clazz;
        MethodsInfo info = new MethodsInfo();
        final String ni = "newInstance".intern();
        final String add = "add".intern();
        while (base != null) {
            Method[] methods = base.getDeclaredMethods();
            for (Method m : methods) {
                int modifiers = m.getModifiers();
                if (Modifier.isPublic(modifiers)) {
                    String name = m.getName();
                    Class[] argsTypes = m.getParameterTypes();
                    int len = argsTypes.length;
                    if (len == 0) {
                        if (name.startsWith("get") || name.startsWith("is")) {
                            info.add(name, m, info.getters);
                        }
                    } else {
                        if (name == ni) {
                            if (m.getAnnotation(UIComponentAnnotation.class) != null) {
                                checkMethod(m);
                                info.newers.add(m);
                            }
                        } else if (len == 1) {
                            if (name == add) {
                                checkMethod(m);
                                info.adders.add(m);
                            } else if (name.startsWith("set") && (StringConverters.containConverter(argsTypes[0]) || ScilabType.class.isAssignableFrom(argsTypes[0]))) {
                                info.add(name, m, info.setters);
                            }
                        }
                    }
                }
            }
            base = base.getSuperclass();
        }

        return info;
    }

    /**
     * If a method is inaccessible, modify its accessibility
     * @param m the the method to check
     */
    private static void checkMethod(Method m) {
        if (!m.isAccessible()) {
            try {
                m.setAccessible(true);
            } catch (SecurityException e) { }
        }
    }

    /**
     * Inner class to cache all interesting methods in a class
     */
    private static class MethodsInfo {

        final Map<String, List<Method>> getters;
        final Map<String, List<Method>> setters;
        final List<Method> adders;
        final List<Method> newers;

        /**
         * Default constructor
         */
        MethodsInfo() {
            getters = new HashMap<String, List<Method>>();
            setters = new HashMap<String, List<Method>>();
            adders = new ArrayList<Method>();
            newers = new ArrayList<Method>();
        }

        /**
         * Add a method in the map
         * @param name method name
         * @param m method object
         * @param map the map where to put info
         */
        static void add(String name, Method m, Map<String, List<Method>> map) {
            List<Method> ms = map.get(name);
            if (ms == null) {
                ms = new ArrayList<Method>();
                map.put(name, ms);
            }
            checkMethod(m);
            ms.add(m);
        }

        void clear() {
            for (Map.Entry<String, List<Method>> entry : getters.entrySet()) {
                entry.getValue().clear();
            }
            getters.clear();
            for (Map.Entry<String, List<Method>> entry : setters.entrySet()) {
                entry.getValue().clear();
            }
            setters.clear();
            adders.clear();
            newers.clear();
        }
    }
}

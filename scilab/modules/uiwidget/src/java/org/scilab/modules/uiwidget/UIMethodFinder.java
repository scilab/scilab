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
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * TODO:
 * on peut ameliorer les choses pr determiner la "bonne" methode a utiliser:
 * par ex: si on a foo(A), foo(B), ... declarees et que l'on cherche la foo appeler
 * pr un argument de type X, on calcule la distance de X a A, B, ... (dist(X,A)=le nombre de
 * de derivation pr passer de A a X) puis on cherche la methode qui minimise cette distance.
 **/

public final class UIMethodFinder {

    private static final Map<Class, MethodsInfo> info = new HashMap<Class, MethodsInfo>();

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

    public static Method findSetter(final String name, final Class baseClass) {
        MethodsInfo i = info.get(baseClass);
        if (i == null) {
            i = getInterestingMethods(baseClass);
            info.put(baseClass, i);
        }

        List<Method> methods = i.setters.get(name);
        if (methods == null || methods.isEmpty()) {
            return null;
        }

        return methods.get(0);
    }

    public static Method findGetter(final String name, final Class baseClass) {
        MethodsInfo i = info.get(baseClass);
        if (i == null) {
            i = getInterestingMethods(baseClass);
            info.put(baseClass, i);
        }

        List<Method> methods = i.getters.get(name);
        if (methods == null) {
            return null;
        }

        return methods.get(0);
    }

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
                        } else if (name.startsWith("set")) {
                            if (StringConverters.containConverter(argsTypes[0])) {
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

    private static void checkMethod(Method m) {
        if (!m.isAccessible()) {
            try {
                m.setAccessible(true);
            } catch (SecurityException e) { }
        }
    }

    private static class MethodsInfo {

        final Map<String, List<Method>> getters;
        final Map<String, List<Method>> setters;
        final List<Method> adders;
        final List<Method> newers;

        MethodsInfo() {
            getters = new HashMap<String, List<Method>>();
            setters = new HashMap<String, List<Method>>();
            adders = new ArrayList<Method>();
            newers = new ArrayList<Method>();
        }

        void add(String name, Method m, Map<String, List<Method>> map) {
            List<Method> ms = map.get(name);
            if (ms == null) {
                ms = new ArrayList<Method>();
                map.put(name, ms);
            }
            checkMethod(m);
            ms.add(m);
        }
    }
}

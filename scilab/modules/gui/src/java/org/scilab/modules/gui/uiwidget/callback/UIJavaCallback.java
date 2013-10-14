/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.uiwidget.callback;

import java.awt.Component;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;

import javax.swing.JComponent;

import org.scilab.modules.gui.uiwidget.UIComponent;

public class UIJavaCallback extends UICallback {

    private enum ArgType {
        VOID, UICOMPONENT, JCOMPONENT, COMPONENT;
    }

    protected Class<? extends UIComponent> clazz;
    protected Method method;
    protected ArgType type;

    public UIJavaCallback(UIComponent uicomp, String code) {
        super(uicomp, code);
        getMethod();
    }

    public UIJavaCallback(Class<? extends UIComponent> clazz, String code) {
        super(code);
        this.clazz = clazz;
        getMethod();
    }

    public void exec(final Object ... args) {
        if (method != null) {
            try {
                switch (type) {
                    case VOID:
                        method.invoke(null);
                        break;
                    case UICOMPONENT:
                        method.invoke(null, component);
                        break;
                    case JCOMPONENT:
                    case COMPONENT:
                        method.invoke(null, component.getModifiableComponent());
                        break;
                }
            } catch (Exception e) { }
        }
    }

    public void exec(final UIComponent comp, final Object ... args) {
        if (method != null) {
            try {
                switch (type) {
                    case VOID:
                        method.invoke(null);
                        break;
                    case UICOMPONENT:
                        method.invoke(null, comp);
                        break;
                    case JCOMPONENT:
                    case COMPONENT:
                        method.invoke(null, comp.getModifiableComponent());
                        break;
                }
            } catch (Exception e) { }
        }
    }

    private final void getMethod() {
        int pos = code.lastIndexOf('.');
        if (pos != -1) {
            String methodName = code.substring(pos + 1);
            String className = code.substring(0, pos);
            Class baseClass = null;
            try {
                baseClass = Class.forName(className);
                Method[] methods = baseClass.getMethods();

                for (Method m : methods) {
                    if (m.getName().equals(methodName) && Modifier.isStatic(m.getModifiers())) {
                        Class[] argsType = m.getParameterTypes();
                        if (argsType.length >= 2) {
                            continue;
                        }

                        if (argsType == null || argsType.length == 0) {
                            type = ArgType.VOID;
                            method = m;
                            break;
                        } else if (JComponent.class.isAssignableFrom(argsType[0])) {
                            type = ArgType.JCOMPONENT;
                            method = m;
                            break;
                        } else if (Component.class.isAssignableFrom(argsType[0])) {
                            type = ArgType.COMPONENT;
                            method = m;
                            break;
                        } else if (UIComponent.class.isAssignableFrom(clazz == null ? argsType[0] : clazz)) {
                            type = ArgType.UICOMPONENT;
                            method = m;
                            break;
                        }
                    }
                }
            } catch (ClassNotFoundException e) { }
        }

        if (method != null && !method.isAccessible()) {
            try {
                method.setAccessible(true);
            } catch (SecurityException e) { }
        }
    }
}

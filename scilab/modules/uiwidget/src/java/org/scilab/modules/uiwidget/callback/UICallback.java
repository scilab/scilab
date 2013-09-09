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

package org.scilab.modules.uiwidget.callback;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import org.scilab.modules.uiwidget.UIComponent;

public abstract class UICallback implements ActionListener {

    public enum Type {
        JAVA, SCI_FUN, SCI_CODE, URL, UNKNOWN;

        public static Type intToType(int type) {
            switch (type) {
                case -1:
                    return UNKNOWN;
                case 0:
                    return SCI_CODE;
                case 1:
                    return SCI_CODE;
                case 2:
                    return SCI_FUN;
                case 3:
                    return JAVA;
                default:
                    return SCI_CODE;
            }
        }
    }

    protected String code;
    protected UIComponent component;

    protected UICallback(String code) {
        this.code = trim(code);
    }

    protected UICallback(UIComponent comp, String code) {
        this(code);
        this.component = comp;
    }

    public abstract void exec(final Object ... args);

    public abstract void exec(final UIComponent comp, final Object ... args);

    public void actionPerformed(ActionEvent e) {
        if (component != null) {
            exec();
        }
    }

    public static UICallback newInstance(final UIComponent comp, final String code) {
        if (code == null || code.isEmpty()) {
            return null;
        }

        final String str = trim(code);
        final Type type = CallbackParser.guessCallbackType(str);

        return newInstance(comp, str, type);
    }

    public static UICallback newInstance(final Class< ? extends UIComponent> clazz, final String code) {
        if (code == null || code.isEmpty()) {
            return null;
        }

        final String str = trim(code);
        final Type type = CallbackParser.guessCallbackType(str);

        return newInstance(clazz, str, type);
    }

    public static UICallback newInstance(final Class<? extends UIComponent> clazz, final String code, final Type type) {
        if (code == null || code.isEmpty()) {
            return null;
        }

        switch (type) {
            case JAVA:
                UIJavaCallback cb = new UIJavaCallback(clazz, code);
                if (cb.method == null) {
                    return new UIScilabFunctionCallback(code);
                } else {
                    return cb;
                }
            case SCI_FUN:
                return new UIScilabFunctionCallback(code);
            case SCI_CODE:
                return new UIScilabCodeCallback(code);
            case URL:
                return new UIURLCallback(code);
            case UNKNOWN:
                String str = trim(code);
                Type typ = CallbackParser.guessCallbackType(str);
                // the parser doesn't return an UNKNOWN type so no recursion pbs
                return newInstance(clazz, str, typ);
            default:
                return null;
        }
    }

    public static UICallback newInstance(final UIComponent comp, final String code, final Type type) {
        if (code == null || code.isEmpty()) {
            return null;
        }

        switch (type) {
            case JAVA:
                UIJavaCallback cb = new UIJavaCallback(comp, code);
                if (cb.method == null) {
                    return new UIScilabFunctionCallback(comp, code);
                } else {
                    return cb;
                }
            case SCI_FUN:
                return new UIScilabFunctionCallback(comp, code);
            case SCI_CODE:
                return new UIScilabCodeCallback(comp, code);
            case URL:
                return new UIURLCallback(comp, code);
            case UNKNOWN:
                String str = trim(code);
                Type typ = CallbackParser.guessCallbackType(str);
                // the parser doesn't return an UNKNOWN type so no recursion pbs
                return newInstance(comp, str, typ);
            default:
                return null;
        }
    }

    public String toString() {
        return code;
    }

    public boolean equals(Object o) {
        if (o.getClass() == this.getClass()) {
            UICallback cb = (UICallback) o;
            return cb.code.equals(code);
        }

        return false;
    }

    /**
     * @param the String to trim
     * @return a trimed String
     */
    private static String trim(String buf) {
        final int end = buf.length();
        int N = 0;
        int M = end;
        char c;
        for (int i = 0; i < end; i++) {
            c = buf.charAt(i);
            if (c != ' ' && c != '\t' && c != '\r' && c != '\n' && c != '\f') {
                N = i;
                break;
            }
        }
        for (int i = end - 1; i >= 0; i--) {
            c = buf.charAt(i);
            if (c != ' ' && c != '\t' && c != '\r' && c != '\n' && c != '\f') {
                M = i + 1;
                break;
            }
        }

        if (N != 0 || M != end) {
            return buf.substring(N, M);
        }

        return buf;
    }
}

/*
 * JIMS ( http://forge.scilab.org/index.php/p/JIMS/ ) - This file is a part of JIMS
 * Copyright (C) 2010 - 2011 - Calixte DENIZET <calixte@contrib.scilab.org>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.external_objects_java;

import java.util.ArrayList;
import java.util.List;
import java.lang.reflect.Method;
import java.lang.reflect.Constructor;

/**
 * A constructor wrapper
 *
 * @author Calixte DENIZET
 */
public class FunctionArguments {

    /**
     * Find a valid method. An argument can be null (thanks to Fabien Viale).
     * @param name the method name
     * @param functions the set of the valid functions in the class
     * @param argsClass the class of the arguments
     * @param args the arguments
     * @return a valid method
     */
    public static final Method findMethod(String name, Method[] functions, Class[] argsClass, Object[] args) throws NoSuchMethodException, TooManyMethodsException {
        String internName = name.intern();
        boolean modified = false;
        List<Method> valids = new ArrayList<Method>(functions.length);
        for (Method f : functions) {
            if (f.getName() == internName) {
                Class[] types = f.getParameterTypes();
                if (types.length == argsClass.length) {
                    boolean ok = true;
                    for (int i = 0; i < types.length; i++) {
                        if (args[i] != null && !argsClass[i].equals(Void.class) && isCompatibleTypes(types[i], argsClass[i], args[i]) == 0) {
                            ok = false;
                            break;
                        }
                    }
                    if (ok) {
                        valids.add(f);
                    }
                }
            }
        }

        if (valids.size() == 1) {
            Method f = valids.get(0);
            Class[] types = f.getParameterTypes();
            for (int i = 0; i < types.length; i++) {
                if (args[i] != null && !argsClass[i].equals(Void.class) && isCompatibleTypes(types[i], argsClass[i], args[i]) == 3) {
                    argsClass[i] = int.class;
                    args[i] = ((Double) args[i]).intValue();
                }
            }
            return f;
        }

        if (valids.size() == 0) {
            throw new NoSuchMethodException("");
        }

        for (Method f : valids) {
            Class[] types = f.getParameterTypes();
            boolean ok = true;
            for (int i = 0; i < types.length; i++) {
                if (args[i] != null && !argsClass[i].equals(Void.class) && isCompatibleTypes(types[i], argsClass[i], args[i]) != 1) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                return f;
            }
        }

        throw new TooManyMethodsException("");
    }

    /**
     * Find a valid constructor. An argument can be null (thanks to Fabien Viale).
     * @param functions the set of all the constructors
     * @param argsClass the class of the arguments
     * @param args the arguments
     * @return a valid constructor
     */
    public static final Constructor findConstructor(Constructor[] functions, Class[] argsClass, Object[] args) throws NoSuchMethodException, TooManyMethodsException {
        boolean modified = false;
        List<Constructor> valids = new ArrayList<Constructor>(functions.length);
        for (Constructor f : functions) {
            Class[] types = f.getParameterTypes();
            if (types.length == argsClass.length) {
                boolean ok = true;
                for (int i = 0; i < types.length; i++) {
                    if (args[i] != null && !argsClass[i].equals(Void.class) && isCompatibleTypes(types[i], argsClass[i], args[i]) == 0) {
                        ok = false;
                        break;
                    }
                }
                if (ok) {
                    valids.add(f);
                }
            }
        }

        if (valids.size() == 1) {
            Constructor f = valids.get(0);
            Class[] types = f.getParameterTypes();
            for (int i = 0; i < types.length; i++) {
                if (args[i] != null && !argsClass[i].equals(Void.class) && isCompatibleTypes(types[i], argsClass[i], args[i]) == 3) {
                    argsClass[i] = int.class;
                    args[i] = ((Double) args[i]).intValue();
                }
            }
            return f;
        }

        if (valids.size() == 0) {
            throw new NoSuchMethodException("");
        }

        for (Constructor f : valids) {
            Class[] types = f.getParameterTypes();
            boolean ok = true;
            for (int i = 0; i < types.length; i++) {
                if (args[i] != null && !argsClass[i].equals(Void.class) && isCompatibleTypes(types[i], argsClass[i], args[i]) != 1) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                return f;
            }
        }

        throw new TooManyMethodsException("");
    }

    /**
     * Handle the autoboxing
     */
    private static final int isCompatibleTypes(Class A, Class B, Object arg) {
        if (A.isAssignableFrom(B)) {
            return 1;
        }
        Class wrapper = ScilabJavaObject.primTypes.get(B);
        if (wrapper != null && A.isAssignableFrom(wrapper)) {
            return 2;
        }
        if (B == double.class && ((Double) arg).intValue() == ((Double) arg).doubleValue()) {
            return 3;
        }

        return 0;
    }

    static class TooManyMethodsException extends Exception {

        TooManyMethodsException(String str) {
            super(str);
        }
    }
}
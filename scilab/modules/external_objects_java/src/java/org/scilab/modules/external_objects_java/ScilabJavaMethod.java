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
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;
import java.lang.reflect.InvocationTargetException;

/**
 * A method wrapper
 *
 * @author Calixte DENIZET
 */
@SuppressWarnings("serial")
public class ScilabJavaMethod {

    protected Class<?> clazz;
    protected String name;

    //We keep in memory the map between class and accessible methods
    protected static Map<Class, Map<String, ScilabJavaMethod>> methsInClass = new HashMap<Class, Map<String, ScilabJavaMethod>>();

    /**
     * Constructor
     * @param clazz the class where the method belongs to
     * @param name the method name
     */
    public ScilabJavaMethod(Class clazz, String name) {
        this.clazz = clazz;
        this.name = name;
    }

    /**
     * @param obj the object where invoking method
     * @param returnType the class of the result
     * @param args the ids of arguments
     * @return the resulting object
     */
    public Object invoke(Object obj, Class[] returnType, int[] args) throws ScilabJavaException {
        int nbargs = args.length;
        Class[] cl = new Class[nbargs];
        Object[] argsO = new Object[nbargs];

        for (int i = 0; i < nbargs; i++) {
            argsO[i] = ScilabJavaObject.arraySJO[args[i]].object;
            cl[i] = ScilabJavaObject.arraySJO[args[i]].clazz;
        }

        return call(obj, returnType, argsO, cl);
    }

    /**
     * @param obj the object where invoking method
     * @param returnType the class of the result
     * @param args the Object arguments
     * @param argsClass the Class of the arguments
     * @return the resulting object
     */
    protected Object call(Object obj, Class[] returnType, Object[] args, Class[] argsClass) throws ScilabJavaException {
        try {
            Method meth = FunctionArguments.findMethod(name, clazz.getMethods(), argsClass, args);
            Object ret;
            if (Modifier.isStatic(meth.getModifiers())) {
                ret = meth.invoke(null, args);
            } else {
                ret = meth.invoke(obj, args);
            }

            Class returned = meth.getReturnType();
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
            throw new ScilabJavaException("Illegal argument in the method " + name + ": \n" + e.getMessage());
        } catch (NullPointerException e) {
            throw new ScilabJavaException("The method " + name + " is called on a null object.");
        } catch (ExceptionInInitializerError e) {
            throw new ScilabJavaException("Initializer error with method " + name + ":\n" + e.getMessage());
        } catch (InvocationTargetException e) {
            throw new ScilabJavaException("An exception has been thrown in calling the method " + name + ":\n" + e.getCause().toString());
        } catch (NoSuchMethodException e) {
            throw new ScilabJavaException("No method " + name + " in the class " + clazz.getName() + " or bad arguments type.");
        } catch (FunctionArguments.TooManyMethodsException e) {
            throw new ScilabJavaException("Too many possible methods named " + name + " in the class " + clazz.getName() + ".");
        }
    }

    /**
     * @param clazz the class where to search methods
     * @return the method belonging to clazz
     */
    public static Map<String, ScilabJavaMethod> getMethods(Class clazz) {
        Map<String, ScilabJavaMethod> hm = methsInClass.get(clazz);

        if (hm != null) {
            return hm;
        }

        hm = new HashMap<String, ScilabJavaMethod>();
        Method[] meth = clazz.getMethods();

        for (int i = 0; i < meth.length; i++) {
            int modif = meth[i].getModifiers();
            if (Modifier.isPublic(modif)) {
                String name = meth[i].getName();
                if (!hm.containsKey(name)) {
                    hm.put(name, new ScilabJavaMethod(clazz, name));
                }
            }
        }
        methsInClass.put(clazz, hm);

        return hm;
    }
}

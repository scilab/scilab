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

import java.util.HashMap;
import java.util.Map;
import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;

/**
 * A constructor wrapper
 *
 * @author Calixte DENIZET
 */
@SuppressWarnings("serial")
public class ScilabJavaConstructor {

    protected Class<?> clazz;

    /**
     * Constructor
     * @param clazz the class where the constructor belongs to
     */
    public ScilabJavaConstructor(Class clazz) {
        this.clazz = clazz;
    }

    /**
     * @param args the ids of arguments
     * @return the resulting object
     */
    public Object invoke(int[] args) throws ScilabJavaException {
        int nbargs = args.length;
        Class[] cls = new Class[nbargs];
        Object[] argsO = new Object[nbargs];

        for (int i = 0; i < nbargs; i++) {
            argsO[i] = ScilabJavaObject.arraySJO[args[i]].object;
            cls[i] = ScilabJavaObject.arraySJO[args[i]].clazz;
        }

        try {
            return FunctionArguments.findConstructor(clazz.getConstructors(), cls, argsO).newInstance(argsO);
        } catch (IllegalAccessException e) {
            throw new ScilabJavaException("Illegal access to the constructor of class " + clazz.getName() + ".");
        } catch (IllegalArgumentException e) {
            throw new ScilabJavaException("Illegal argument in the constructor of class " + clazz.getName() + ": \n" + e.getMessage());
        } catch (InstantiationException e) {
            throw new ScilabJavaException("The class " + clazz.getName() + "is abstract and cannot be instantiated.");
        } catch (ExceptionInInitializerError e) {
            throw new ScilabJavaException("Initializer error with constructor of class " + clazz.getName() + ":\n" + e.getMessage());
        } catch (InvocationTargetException e) {
            throw new ScilabJavaException("An exception has been thrown in calling the constructor of class " + clazz.getName() + ":\n" + e.getMessage());
        } catch (NoSuchMethodException e) {
            throw new ScilabJavaException("No such constructor in the class " + clazz.getName() + ".");
        } catch (FunctionArguments.TooManyMethodsException e) {
            throw new ScilabJavaException("Too many possible constructors in the class " + clazz.getName() + ".");
        }
    }
}

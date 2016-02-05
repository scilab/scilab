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
import java.lang.reflect.Constructor;
import java.util.HashMap;
import java.util.Map;
import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;

import javax.swing.SwingUtilities;

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
    public Object invoke(final int[] args) throws ScilabJavaException {
        final int nbargs = args.length;
        final Class[] cls = new Class[nbargs];
        final Object[] argsO = new Object[nbargs];

        for (int i = 0; i < nbargs; i++) {
            argsO[i] = ScilabJavaObject.arraySJO[args[i]].object;
            cls[i] = ScilabJavaObject.arraySJO[args[i]].clazz;

            if (argsO[i] != null && argsO[i] == cls[i]) {
                cls[i] = argsO[i].getClass();
            }
        }

        try {
            final Object[] info = FunctionArguments.findConstructor(clazz.getConstructors(), cls, argsO);
            final Constructor constructor = (Constructor) info[0];
            final Object[] _args;
            if (info.length == 2) {
                // Constructor with variable arguments, so they have been modified and are the second element of the returned array
                _args = (Object[]) info[1];
            } else {
                _args = argsO;
            }

            if (!constructor.isAccessible()) {
                try {
                    constructor.setAccessible(true);
                } catch (SecurityException e) { }
            }

            if (Component.class.isAssignableFrom(clazz)) {
                if (SwingUtilities.isEventDispatchThread()) {
                    return constructor.newInstance(_args);
                } else {
                    final Object[] ref = new Object[1];
                    SwingUtilities.invokeAndWait(new Runnable() {

                        public void run() {
                            try {
                                ref[0] = constructor.newInstance(_args);
                            } catch (Exception e) {
                                System.err.println(e);
                            }
                        }
                    });

                    return ref[0];
                }
            }

            return constructor.newInstance(_args);
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
        } catch (InterruptedException e) {
            throw new ScilabJavaException("EDT has been interrupted...");
        }
    }
}

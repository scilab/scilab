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

import java.net.URL;
import java.net.URLClassLoader;
import java.security.CodeSource;
import java.util.HashMap;
import java.util.Map;
import java.util.logging.Level;

/**
 * A class loader... When a class is needed, the first time the default system class loader is used
 * but the next times, an URLClassLoader is used. It provides a common way to reload the classes if needed.
 *
 * @author Calixte DENIZET
 */
@SuppressWarnings("serial")
public class ScilabClassLoader {

    protected static Map<String, Integer> clazz = new HashMap<String, Integer>();
    protected static Map<Integer, String> zzalc = new HashMap<Integer, String>();

    private ScilabClassLoader() { }

    /**
     * Load a class by its name
     * @param name the class name
     * @return the id of the corresponding class
     */
    public static int loadJavaClass(String name) throws ScilabJavaException {
        return loadJavaClass(name, true);
    }

    /**
     * Load a class by its name
     * @param name the class name
     * @param allowReload if true the class will be reloaded if needed
     * @return the id of the corresponding class
     */
    public static int loadJavaClass(String name, boolean allowReload) throws ScilabJavaException {
        Integer id = clazz.get(name);
        int retId = -1;

        if (ScilabJavaObject.debug) {
            ScilabJavaObject.logger.log(Level.INFO, "Load Java class \'" + name + "\' and reload=" + allowReload);
        }

        try {
            if (id != null) {
                if (allowReload) {
                    ScilabJavaClass sjc = (ScilabJavaClass) ScilabJavaObject.arraySJO[id];
                    URL url = sjc.getURL();
                    if (url == null) {
                        System.err.println("Warning: The class " + name + " could not be reloaded");
                        ScilabJavaObject clone = ScilabJavaObject.arraySJO[id].clone();
                        retId = clone.id;
                    } else {
                        Class cl = new URLClassLoader(new URL[] {url}, null).loadClass(name);
                        ScilabJavaMethod.methsInClass.remove(sjc.clazz);
                        ScilabJavaObject.removeScilabJavaObject(id);
                        sjc = new ScilabJavaClass(cl);
                        sjc.setURL(url);
                        clazz.put(name, sjc.id);
                        zzalc.put(sjc.id, name);
                        retId = sjc.id;
                    }
                } else {
                    ScilabJavaObject clone = ScilabJavaObject.arraySJO[id].clone();
                    retId = clone.id;
                }
            } else {
                ScilabJavaClass sjc = new ScilabJavaClass(ClassLoader.getSystemClassLoader().loadClass(name));
                sjc.setURL(getLocation(sjc.clazz));
                clazz.put(name, sjc.id);
                zzalc.put(sjc.id, name);
                retId = sjc.id;
            }
        } catch (Exception e) {
            System.err.println(e);
            e.printStackTrace();
            throw new ScilabJavaException("Cannot find the class " + name + ". Check the name or if the classpath contains it.");
        }

        if (ScilabJavaObject.debug) {
            ScilabJavaObject.logger.log(Level.INFO, "The loaded Java class has id=" + retId + " and URL=" + ((ScilabJavaClass) ScilabJavaObject.arraySJO[retId]).getURL());
        }

        return retId;
    }

    /**
     * Remove an id from the map
     * @param id the id to remove
     */
    protected static void removeID(int id) {
        String name = zzalc.get(id);
        if (name != null) {
            clazz.remove(name);
            zzalc.remove(id);
        }
    }

    /**
     * @param cl the class which we want the location
     * @return the corresponding URL
     */
    private static final URL getLocation(Class cl) {
        CodeSource cs = cl.getProtectionDomain().getCodeSource();
        if (cs == null) {
            return null;
        }

        return cs.getLocation();
    }
}

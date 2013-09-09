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

package org.scilab.modules.uiwidget;

import java.io.File;
import java.io.IOException;
import java.net.JarURLConnection;
import java.net.URL;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.Map;
import java.util.jar.JarEntry;
import java.util.jar.JarFile;

/**
 * Class to find the UIComponent class corresponding to a name
 */
public final class UIClassFinder {

    private static final Map<String, Map<String, String>> info = new HashMap<String, Map<String, String>>();

    /**
     * Clear the cache
     */
    public static void clearCache() {
        for (Map.Entry<String, Map<String, String>> entry : info.entrySet()) {
            entry.getValue().clear();
        }

        info.clear();
    }

    /**
     * Find a class according to its class name
     * @param pack the package name
     * @param className the class name
     * @return the corresponding class
     */
    public static Class findClass(final String pack, final String className) throws ClassNotFoundException {
        Class clazz = null;
        try {
            clazz = Class.forName(pack + "." + className);
        } catch (ClassNotFoundException e) {
            String name = getCorrectClassName(pack, className);
            if (name != null) {
                clazz = Class.forName(pack + "." + name);
                Map<String, String> map = info.get(pack);
                if (map == null) {
                    map = new HashMap<String, String>();
                    info.put(pack, map);
                }

                map.put(className, name);
            }
        }

        return clazz;
    }

    /**
     * Find a class according to its class name in ignoring the case
     * @param pack the package name
     * @param className the class name
     * @return the correct name
     */
    public static String getCorrectClassName(final String pack, final String className) throws ClassNotFoundException {
        Map<String, String> map = info.get(pack);
        if (map != null) {
            String name = map.get(className);
            if (name != null) {
                return name;
            }
        }

        try {
            ClassLoader classLoader = Thread.currentThread().getContextClassLoader();
            URL url = classLoader.getResource(pack.replace(".", "/"));

            if (url.getProtocol().equals("jar")) {
                JarURLConnection connection = (JarURLConnection) url.openConnection();
                JarFile file = connection.getJarFile();
                String parent = connection.getJarEntry().getName();

                Enumeration<JarEntry> entries = file.entries();

                while (entries.hasMoreElements()) {
                    String name = entries.nextElement().getName();
                    File f = new File(name);
                    if (parent.equals(f.getParent())) {
                        String n = f.getName();
                        int dot = n.lastIndexOf('.');
                        if (dot != -1) {
                            n = n.substring(0, dot);
                        }
                        if (className.equalsIgnoreCase(n)) {
                            return n;
                        }
                    }
                }
            } else {
                File dir = new File(url.getFile());
                File[] list = dir.listFiles();

                for (File f : list) {
                    String name = f.getName();
                    int dot = name.lastIndexOf('.');
                    if (dot != -1) {
                        name = name.substring(0, dot);
                        if (name.equalsIgnoreCase(className)) {
                            return name;
                        }
                    }
                }
            }
        } catch (IOException e) {

        }

        return null;
    }
}

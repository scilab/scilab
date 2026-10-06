/*--------------------------------------------------------------------------*/
/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * Copyright (C) 2008 - INRIA - Sylvestre LEDRU
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

/*--------------------------------------------------------------------------*/
package org.scilab.modules.jvm;
/*--------------------------------------------------------------------------*/
import java.io.File;
import java.io.IOException;
import java.net.URL;
import java.net.URLClassLoader;
import java.util.Iterator;
import java.util.Vector;
/*--------------------------------------------------------------------------*/
/**
 * ClassPath to overload java classpath.
 */
public class ClassPath {

	private static final Class<?>[] parameters = new Class<?>[] {URL.class};

	private static Vector<URL> queued = new Vector<URL>();
    
    /**
     * add a filename to java classpath.
     * @param s a filename
     * @throws IOException if an error occurs
     */
    public static void addFile(final String s, int i) throws IOException {
        addFile(new File(s), i);
    }
    /*-----------------------------------------------------------------------*/
    /**
     * add a file to java classpath.
     * @param  f a file
     * @throws IOException if an error occurs
     */
    public static void addFile(final File f, int i) throws IOException {
        addURL(f.toURI().toURL(), i);
    }

    /*-----------------------------------------------------------------------*/
    /**
     * Add a URL to classpath.
     * @param u URL of the classes (jar or path)
     * @param i the type of load: i=0 startup / i=1 background / i=2 onUse
     */
    public static void addURL(final URL u, int i) {
        switch (i) {
            case 0: /* Load now */
            	((ScilabClassLoader) ClassLoader.getSystemClassLoader()).addURL(u);
                break;
            case 1: /* Load later (background) */
                queued.add(u);
                break;
        }
    }
    /*-----------------------------------------------------------------------*/
    /**
     * Get the classpath loaded
     * @return classpath The list of the classpath
     */
    public static String[] getClassPath() {

        URLClassLoader sysloader = (URLClassLoader) ClassLoader.getSystemClassLoader();
        URL[] path = sysloader.getURLs();
        String[] paths = new String[path.length];
        for (int i = 0; i < path.length; i++) {
            paths[i] = path[i].getFile();
        }
        return paths;
    }


    /**
     * Load all the classpath in dedicated threads in background
     */
    public static void loadBackGroundClassPath() {
        Thread backgroundLoader = new Thread() {
            public void run() {
                try {

                    Iterator<URL> urlIt = queued.iterator();

                    while (urlIt.hasNext()) {
                        ClassPath.addURL(urlIt.next(), 0);
                    }

                } catch (Exception e) {
                    System.err.println("Error : " + e.getLocalizedMessage());
                }
            }
        };
        backgroundLoader.start();
    }
}
/*--------------------------------------------------------------------------*/


/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2023-2026 - Dassault Systèmes S.E. - Vincent COUVERT
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

package org.scilab.modules.jvm;

import java.io.File;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLClassLoader;
import java.nio.file.Paths;
import java.util.List;
import java.util.concurrent.CopyOnWriteArrayList;

public class ScilabClassLoader extends URLClassLoader {

	/**
	 * Directories searched for native libraries when a class loaded by this
	 * class loader calls System.loadLibrary(name).
	 */
	private static final List<String> nativeSearchPaths = new CopyOnWriteArrayList<>();

	/**
	 * Constructor with one parameter of {@link ClassLoader} type
	 * Needed to be able to pass this class as parameter to the JVM (-Djava.class.loader=...)
	 * @param parent parent class loader
	 */
	public ScilabClassLoader(ClassLoader parent) {
		super(new URL[0], parent);
	}

	/**
	 * Register a directory to be searched for native libraries.
	 * @param path directory to add
	 */
	public static void addNativeLibrarySearchPath(String path) {
		if (path != null && !nativeSearchPaths.contains(path)) {
			nativeSearchPaths.add(path);
		}
	}

	/**
	 * Called by the JVM when System.loadLibrary(libname) is invoked from a class
	 * loaded by this class loader, before falling back on java.library.path.
	 * @param libname the platform-independent library name (e.g. "scilocalization")
	 * @return the absolute path of the library if found, null otherwise
	 */
	@Override
	protected String findLibrary(String libname) {
		String mapped = System.mapLibraryName(libname);
		for (String dir : nativeSearchPaths) {
			File f = new File(dir, mapped);
			if (f.isFile()) {
				return f.getAbsolutePath();
			}
		}
		return super.findLibrary(libname);
	}

	/**
	 * Give access to protected {@link URLClassLoader#addURL(URL)}
	 * This avoid to use reflection in {@link ClassPath#addURL(URL, int)}
	 */
	public void addURL(URL url) {
        super.addURL(url);
    }

	/**
	 * Method used to load a class
	 * @param name the name of the class to be loaded
	 */
	@Override
	public Class<?> loadClass(String name) throws ClassNotFoundException {
		return super.loadClass(name);
	}

    /**
     * Called by the JVM to support dynamic additions to the class path.
     * Needed when debugging Javasci applications using Eclipse for example.
     */
    final void appendToClassPathForInstrumentation(String jar) {
        try {
            super.addURL(Paths.get(jar).toUri().toURL());
        } catch (MalformedURLException e) {
            e.printStackTrace();
        }
    }
}

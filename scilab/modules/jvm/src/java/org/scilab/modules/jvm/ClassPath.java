/*--------------------------------------------------------------------------*/
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
package org.scilab.modules.jvm;
/*--------------------------------------------------------------------------*/ 
import java.lang.reflect.Method;
import java.lang.reflect.InvocationTargetException;
import java.io.File;
import java.io.IOException;
import java.net.URL;
import java.net.URLClassLoader;
import java.net.URI;
/*--------------------------------------------------------------------------*/
/**
 * ClassPath to overload java classpath.
 */
public class ClassPath {

private static final Class[] parameters = new Class[]{URL.class};

/**
 * Constructor
 */
protected ClassPath() {
	/*  indicate that the requested operation is not supported */
	throw new UnsupportedOperationException(); 		
}

/**
 * add a filename to java classpath.
 * @param s a filename
 * @throws IOException if an error occurs
 */
public static void addFile(final String s) throws IOException {
File f = new File(s);
addFile(f);
}
/*--------------------------------------------------------------------------*/
/**
 * add a file to java classpath.
 * @param  f a file
 * @throws IOException if an error occurs
 */
public static void addFile(final File f) throws IOException {
	
URI uri = f.toURI();
addURL(uri.toURL());
}
/*--------------------------------------------------------------------------*/
/**
 * add a URL to classpath.
 * @param u URL
 * @throws IOException if an error occurs 
 */
public static void addURL(final URL u) throws IOException {

URLClassLoader sysloader = (URLClassLoader) ClassLoader.getSystemClassLoader();
Class sysclass = URLClassLoader.class;

try {
Method method = sysclass.getDeclaredMethod("addURL", parameters);
method.setAccessible(true);
method.invoke(sysloader , new Object[] {u });
} catch (NoSuchMethodException e) {
	throw new IOException("Error NoSuchMethodException, could not add URL to system classloader");
} catch (IllegalAccessException e) {
	throw new IOException("Error IllegalAccessException, could not add URL to system classloader");
} catch (InvocationTargetException e) {
	throw new IOException("Error InvocationTargetException, could not add URL to system classloader");
}

}
/*--------------------------------------------------------------------------*/
/**
 * get the scilab classpath.
 * @return classpath
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
/*--------------------------------------------------------------------------*/
}
/*--------------------------------------------------------------------------*/


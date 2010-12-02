/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
package javasci;
/*--------------------------------------------------------------------------*/
import java.io.IOException;
import java.io.File;
import java.lang.reflect.Field;
/*--------------------------------------------------------------------------*/
/*http://forum.java.sun.com/thread.jspa?threadID=135560&start=15&tstart=0 */
/*--------------------------------------------------------------------------*/

/**
 * LibraryPath to overload java.library.path.
 */
public class LibraryPath {

	private static final String JAVALIBRARYPATH = "java.library.path";
	/**
	 * Constructor
	 */
	protected LibraryPath() {
		/*  indicate that the requested operation is not supported */
		throw new UnsupportedOperationException(); 		
	}
/*--------------------------------------------------------------------------*/	
	/**
	 * checks if pathToAdd already exists
     * @deprecated
     * @param currentpaths list of current paths
     * @param pathToAdd path to add
     * @return a boolean true if path already exists
     */
	private static boolean pathAlreadyExists(String currentpaths, String pathToAdd) {
    String[] paths = currentpaths.split("" + File.pathSeparatorChar);
        for (String libraryPath : paths) {
            if (libraryPath.equalsIgnoreCase(pathToAdd)) { return true; }
        }
        return false;
    }
/*--------------------------------------------------------------------------*/
	/**
	 * add a path to java.library.path 
     * @deprecated
	 * @param p path to add
	 * @throws IOException return a exception
	 */
	public static void addPath(final String p) throws IOException {
	if (!pathAlreadyExists(System.getProperty(JAVALIBRARYPATH), p)) {
		String newLibPath = p + File.pathSeparator + System.getProperty(JAVALIBRARYPATH);
		System.setProperty(JAVALIBRARYPATH, newLibPath);
		try {
			Field fieldSysPath = ClassLoader.class.getDeclaredField("sys_paths");
			fieldSysPath.setAccessible(true);
			if (fieldSysPath != null) {
	    	fieldSysPath.set(System.class.getClassLoader(), null);
	    	}
	    } catch (NoSuchFieldException e) {
	    	throw new IOException("Error NoSuchFieldException, could not add path to " + JAVALIBRARYPATH);
	    } catch (IllegalAccessException e) {
	    	throw new IOException("Error IllegalAccessException, could not add path to " + JAVALIBRARYPATH);
	    }
		}
  }
 
/*--------------------------------------------------------------------------*/
/**
 * get the scilab java.library.path.
 * @deprecated
 * @return librarypath
 */
	public static String[] getLibraryPath() {
		String librarypath = System.getProperty(JAVALIBRARYPATH);
		String[] paths = librarypath.split("" + File.pathSeparatorChar);
		return paths;
	}
}
/*--------------------------------------------------------------------------*/

/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Sylvestre LEDRU
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.helptools;

import com.sun.java.help.search.Indexer; /* jhall (Java Help) */
import java.io.File;
import java.io.FileOutputStream;
import java.io.FilenameFilter;
import java.io.FileInputStream;
import java.util.zip.ZipEntry;

import java.util.jar.JarOutputStream;


/**
 * This class manages the build of the Java Help
 */
public class BuildJavaHelp {

    /**
     * Private method which is trying to build the jar
     *
     * @param outputDirectory Where to build the jar file
     * @param language In which language (for the file name)
     * @return The result of the operation
	 */
	private static boolean buildJar(String outputDirectory, String language) {
		String baseName = "scilab_" + language + "_help";
		JarOutputStream jarFile = null;
		FileOutputStream fileOutputStream = null;
		final int compressionLevel = 5;
		try {
			fileOutputStream = new FileOutputStream(outputDirectory + "/../" + baseName + ".jar");
		} catch (java.io.IOException e) {
		}
		try {
			jarFile = new JarOutputStream(fileOutputStream);
		} catch (java.io.FileNotFoundException e) {
		} catch (java.io.IOException e) {

		}
		jarFile.setLevel(compressionLevel);

		FilenameFilter filter = new FilenameFilter() {
				public boolean accept(File dir, String name) {
					return name.endsWith(".html") || name.endsWith(".xml") || name.endsWith(".jhm") || name.endsWith(".hs");
				}
			};

        File currentDir = new File(outputDirectory);
		File []allFiles = currentDir.listFiles(filter);
		for (int i = 0; i < allFiles.length; i++) {			
			try {
			FileInputStream fileInputStream = null;
			fileInputStream = new FileInputStream(allFiles[i]);
 
			int length = (int) allFiles[i].length();
			byte[] buffer = new byte[length];

			try {
				fileInputStream.read(buffer, 0, length);
			} catch (java.io.IOException e) {
			}
 
			ZipEntry zipEntry = new ZipEntry(baseName + "/" + allFiles[i].getName());
			jarFile.putNextEntry(zipEntry);

			jarFile.write(buffer, 0, length);
 
			fileInputStream.close();
			} catch (java.io.IOException e) {
			}

		}
		try {
			jarFile.close();
		} catch (java.io.IOException e) {
		}
		return true;
	}

    /**
     * After the saxon process, create the Jar 
     *
     * @param outputDirectory Where the files are available and 
     * @param language In which language (for the file name)
     * @return The result of the process
	 */
	public static boolean buildJavaHelp(String outputDirectory, String language) {
		System.out.println("Building in " + outputDirectory);

		Indexer indexer = new Indexer();
		try {
			String[] args = new String[] {
				outputDirectory
			};
			indexer.compile(args);
		} catch (Exception e) {
			System.err.println("Error building search index: " + e.toString());
			return false;
		}

		BuildJavaHelp.buildJar(outputDirectory, language);
			
		return true;
	}

}

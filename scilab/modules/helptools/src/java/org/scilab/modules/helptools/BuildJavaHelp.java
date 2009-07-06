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
import java.io.FileInputStream;
import java.util.zip.ZipEntry;
import java.util.jar.JarOutputStream;

import java.util.ArrayList;

/**
 * This class manages the build of the Java Help
 */
public final class BuildJavaHelp {
	
	private static final String JAVAHELPSEARCH_DIR = "/JavaHelpSearch/";
	private static final String COULD_NOT_FIND = "buildDoc: Could not find/access to ";
	private static final String LEFT_PAR = " ( ";
	private static final String RIGHT_PAR = " )";
	private static Indexer indexer = new Indexer();
	
	/**
	 * Default constructor (must no be used)
	 */
	private BuildJavaHelp() {
		throw new UnsupportedOperationException();
	}

	/**
	 * Get the list of the files in a directory
	 * @param directory the directory where files have to be searched
	 * @return the list of the files found
	 */
	private static ArrayList<File> buildFileList(File directory,  String language) {
	    String baseNameJar = Helpers.getBaseName(language) + ".jar";
		ArrayList<File> listFile = new ArrayList<File>();
		
		File [] files = directory.listFiles();
		for (int i = 0; i < files.length; i++) {
			if (files[i].isDirectory()) {
				listFile.addAll(buildFileList(files[i], language));
			} else {
			    /* bug 4407 */
			    /* we do not add scilab_xx_XX_help.jar file to the list */
			    if (files[i].compareTo(new File(baseNameJar)) != 0) {
				listFile.add(files[i]);
				}
			}
		}
		return listFile;
	}

    /**
     * Private method which is trying to build the jar
     *
     * @param outputDirectory Where to build the jar file
     * @param language In which language (for the file name)
     * @return The result of the operation
	 */
	private static boolean buildJar(String outputDirectory, String language) {
		String baseName = Helpers.getBaseName(language);
		JarOutputStream jarFile = null;
		FileOutputStream fileOutputStream = null;
		final int compressionLevel = 5;
		/* Stored into SCI/modules/helptools/jar */
		String fileName = outputDirectory + "/" + baseName + ".jar";
		
		/* bug 4407 */
		/* we do list of files before to create scilab_xx_XX_help.jar */
		ArrayList<File> fileList = BuildJavaHelp.buildFileList(new File(outputDirectory), language);

		try {

			fileOutputStream = new FileOutputStream(fileName);
			jarFile = new JarOutputStream(fileOutputStream);

		} catch (java.io.FileNotFoundException e) {
			System.err.println(COULD_NOT_FIND + fileName + LEFT_PAR + e.getLocalizedMessage() + RIGHT_PAR);
		} catch (java.io.IOException e) {
			System.err.println(COULD_NOT_FIND + fileName + LEFT_PAR + e.getLocalizedMessage() + RIGHT_PAR);
		}

		jarFile.setLevel(compressionLevel);
		
		File []allFiles = fileList.toArray(new File [fileList.size()]);
		for (int i = 0; i < allFiles.length; i++) {			
			try {
				File workingFile = allFiles[i];
				FileInputStream fileInputStream = new FileInputStream(workingFile);
 
				int length = (int) workingFile.length();
				byte[] buffer = new byte[length];
				try {
					fileInputStream.read(buffer, 0, length);
				} catch (java.io.IOException e) {
					System.err.println(COULD_NOT_FIND + workingFile + LEFT_PAR + e.getLocalizedMessage() + RIGHT_PAR);
				}
				String relativeFileName = null;
				if (workingFile.getPath().indexOf("JavaHelpSearch") == -1) {
					relativeFileName = baseName + "/" + workingFile.getName();
				} else {
 					relativeFileName = baseName + JAVAHELPSEARCH_DIR + workingFile.getName();
				}
				ZipEntry zipEntry = new ZipEntry(relativeFileName);
				jarFile.putNextEntry(zipEntry);

				jarFile.write(buffer, 0, length);
 
				fileInputStream.close();
			} catch (java.io.IOException e) {
				System.err.println("buildDoc: An error occurs while building the JavaHelp ( " + e.getLocalizedMessage() + RIGHT_PAR);
			}

		}
		try {
			jarFile.close();
		} catch (java.io.IOException e) {
			System.err.println("buildDoc: An error occurs while closing the JavaHelp ( " + e.getLocalizedMessage() + RIGHT_PAR);
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
	public static String buildJavaHelp(String outputDirectory, String language) {
        
		String outputJavaHelp = new String(outputDirectory + JAVAHELPSEARCH_DIR);
        
        try {
			/* Purge the directory before launching the index */
			/* because the JavaHelp Indexer failed when launched twice on the same directory */
			Helpers.deleteDirectory(outputJavaHelp); 
			File directory = new File(outputJavaHelp);
			directory.mkdirs();

			String[] args = new String[] {"."};

			indexer.compile(args);
		} catch (Exception e) {
			System.err.println("buildDoc: Error building search index: " + e.getLocalizedMessage());
			return null;
		}

		BuildJavaHelp.buildJar(outputDirectory, language);
			
		return outputDirectory;
	}

}

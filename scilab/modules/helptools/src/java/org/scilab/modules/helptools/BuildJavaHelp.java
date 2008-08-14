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

import java.util.ArrayList;

/**
 * This class manages the build of the Java Help
 */
public class BuildJavaHelp {

	private static ArrayList<File> buildFileList(File directory) {
		ArrayList<File> listFile = new ArrayList<File>();

		File [] files = directory.listFiles();
		for (int i = 0; i < files.length; i++) {

			if (files[i].isDirectory()) {
				listFile.addAll(buildFileList(files[i]));
			}else{
				listFile.add(files[i]);
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
		String fileName = outputDirectory + "/../../../jar/" + baseName + ".jar";

		try {

			fileOutputStream = new FileOutputStream(fileName);
			jarFile = new JarOutputStream(fileOutputStream);

		} catch (java.io.FileNotFoundException e) {
			System.err.println("buildDoc: Could not find/access to " + fileName + " ( " + e.getLocalizedMessage() + " )");
		} catch (java.io.IOException e) {
			System.err.println("buildDoc: Could not find/access to " + fileName + " ( " + e.getLocalizedMessage() + " )");
		}

		jarFile.setLevel(compressionLevel);
		ArrayList<File> fileList=BuildJavaHelp.buildFileList(new File(outputDirectory));
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
					System.err.println("buildDoc: Could not find/access to " + workingFile + " ( " + e.getLocalizedMessage() + " )");
				}
				String relativeFileName=null;
				if (workingFile.getPath().indexOf("JavaHelpSearch") == -1) {
					relativeFileName = baseName + "/" + workingFile.getName();
				} else {
					relativeFileName = baseName + "/JavaHelpSearch/" + workingFile.getName();
				}
				ZipEntry zipEntry = new ZipEntry(relativeFileName);
				jarFile.putNextEntry(zipEntry);

				jarFile.write(buffer, 0, length);
 
				fileInputStream.close();
			} catch (java.io.IOException e) {
				System.err.println("buildDoc: An error occurs while building the JavaHelp ( " + e.getLocalizedMessage() + " )");
			}

		}
		try {
			jarFile.close();
		} catch (java.io.IOException e) {
			System.err.println("buildDoc: An error occurs while closing the JavaHelp ( " + e.getLocalizedMessage() + " )");
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
		Indexer indexer = new Indexer();

		try {
			String[] args = new String[] {
				"-db",
				outputDirectory + "/JavaHelpSearch/", /* Where the Java Help Index should be created */
				outputDirectory
			};
			indexer.compile(args);
		} catch (Exception e) {
			System.err.println("buildDoc: Error building search index: " + e.getLocalizedMessage());
			return false;
		}

		BuildJavaHelp.buildJar(outputDirectory, language);
			
		return true;
	}

}

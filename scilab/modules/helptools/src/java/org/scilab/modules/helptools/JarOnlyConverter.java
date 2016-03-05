/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Sylvestre LEDRU
 * Copyright (C) 2013 - Scilab Enterprises - Clement DAVID
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
package org.scilab.modules.helptools;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.FilenameFilter;
import java.util.ArrayList;
import java.util.jar.JarOutputStream;
import java.util.zip.ZipEntry;

import org.scilab.modules.commons.ScilabConstants;

import com.sun.java.help.search.Indexer;

public class JarOnlyConverter extends ContainerConverter {

    private static final String SCI = ScilabConstants.SCI.getPath();

    private static final String JAVAHELPSEARCH_DIR = "/JavaHelpSearch/";
    private static final String COULD_NOT_FIND = "buildDoc: Could not find/access to ";
    private static final String LEFT_PAR = " ( ";
    private static final String RIGHT_PAR = " )";
    private static final String JAR_EXT = ".jar";
    private static final String SLASH = "/";
    private static final int JAR_COMPRESSION_LEVEL = 9;
    private static Indexer indexer = new Indexer();

    private final boolean isToolbox;
    private final String outImages;

    public JarOnlyConverter(SciDocMain sciDocMain) {
        super(sciDocMain.getOutputDirectory(), sciDocMain.getLanguage());

        isToolbox = sciDocMain.isToolbox();

        /*
         * Reuse the shared generated images directory from scilab
         */
        String images = sciDocMain.getOutputDirectory() + File.separator;
        if (!isToolbox) {
            images = ScilabConstants.SCI.getPath() + "/modules/helptools/images";
            File dir = new File(images);
            if (!dir.exists()) {
                dir.mkdir();
            }
        }
        outImages = images;
    }

    /**
     * Embed the javahelp html files to a jar
     */
    @Override
    public void convert() {
        String outputJavaHelp = new String(outputDirectory + JAVAHELPSEARCH_DIR);

        try {
            /* Purge the directory before launching the index */
            /* because the JavaHelp Indexer failed when launched twice on the same directory */
            Helpers.deleteDirectory(outputJavaHelp);
            File directory = new File(outputJavaHelp);
            directory.mkdirs();
            String jhs = new File(outputJavaHelp).getAbsolutePath();

            String[] args = new String[] {"-db", jhs, "-nostopwords", "."};

            indexer.compile(args);
        } catch (Exception e) {
            System.err.println("buildDoc: Error building search index: " + e.getLocalizedMessage());
            return;
        }

        buildJar(outputDirectory, language);
    }

    /**
     * Embed the images files to another jar for non-toolboxes
     */
    @Override
    public void install() {
        /*
         * Toolboxes images are not in a separate jar.
         */
        if (isToolbox) {
            return;
        }

        JarOutputStream jarFile = null;
        FileOutputStream fileOutputStream = null;

        /* Stored into SCI/modules/helptools/jar */
        String fileName = SCI + "/modules/helptools/jar" + SLASH + "scilab_images" + JAR_EXT;
        try {
            fileOutputStream = new FileOutputStream(fileName);
            jarFile = new JarOutputStream(fileOutputStream);
            jarFile.setLevel(JAR_COMPRESSION_LEVEL);
        } catch (java.io.FileNotFoundException e) {
            System.err.println(COULD_NOT_FIND + fileName + LEFT_PAR + e.getLocalizedMessage() + RIGHT_PAR);
        } catch (java.io.IOException e) {
            System.err.println(COULD_NOT_FIND + fileName + LEFT_PAR + e.getLocalizedMessage() + RIGHT_PAR);
        }

        File[] allFiles = new File(outImages).listFiles();
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
                ZipEntry zipEntry = new ZipEntry(workingFile.getName());
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
    }

    /**
     * Get the list of the files in a directory
     * @param directory the directory where files have to be searched
     * @param language String 'fr_FR'
     * @return the list of the files found
     */
    private static ArrayList<File> buildFileList(File directory,  String language) {
        final String baseNameJar = Helpers.getBaseName(language) + JAR_EXT;
        ArrayList<File> listFile = new ArrayList<File>();

        File[] files = directory.listFiles(new FilenameFilter() {
            public boolean accept(File dir, String name) {
                return !name.equals(baseNameJar);
            }
        });
        for (int i = 0; i < files.length; i++) {
            if (files[i].isDirectory()) {
                listFile.addAll(buildFileList(files[i], language));
            } else {
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
    static boolean buildJar(String outputDirectory, String language) {
        String baseName = Helpers.getBaseName(language);
        JarOutputStream jarFile = null;
        FileOutputStream fileOutputStream = null;

        /* Stored into SCI/modules/helptools/jar */
        String fileName = outputDirectory + SLASH + baseName + JAR_EXT;
        /* bug 4407 */
        /* we do list of files before to create scilab_xx_XX_help.jar */
        ArrayList<File> fileList = buildFileList(new File(outputDirectory), language);

        try {
            fileOutputStream = new FileOutputStream(fileName);
            jarFile = new JarOutputStream(fileOutputStream);
        } catch (java.io.FileNotFoundException e) {
            System.err.println(COULD_NOT_FIND + fileName + LEFT_PAR + e.getLocalizedMessage() + RIGHT_PAR);
        } catch (java.io.IOException e) {
            System.err.println(COULD_NOT_FIND + fileName + LEFT_PAR + e.getLocalizedMessage() + RIGHT_PAR);
        }

        jarFile.setLevel(JAR_COMPRESSION_LEVEL);

        File[] allFiles = fileList.toArray(new File [fileList.size()]);
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
                    relativeFileName = baseName + SLASH + workingFile.getName();
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
}

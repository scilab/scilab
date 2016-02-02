/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises - Simon MARCHETTO
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

package org.scilab.modules.external_objects_java;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.BufferedInputStream;
import java.io.IOException;
import java.util.List;
import java.util.ArrayList;
import java.util.jar.JarEntry;
import java.util.jar.JarOutputStream;
import java.util.jar.Manifest;


/**
 * Class to create JAR files (for JIMS)
 */
public class ScilabJarCreator {
    /**
     * Create a Jar archive from a set of files
     * @param jarFilePath the destination jar path
     * @param filePaths the set of file paths to be jar-ed
     * @param filesRootPath the root path of files from which the relative paths in jar will be computed
     * @param manifestFilePath the path of manifest file
     * @param keepAbsolutePaths keep absolute paths of files in jar
     */
    public static int createJarArchive(String jarFilePath, String[] filePaths, String filesRootPath,
                                       String manifestFilePath, boolean keepAbsolutePaths) throws ScilabJavaException {
        JarOutputStream jarOutputStream = null;
        try {
            // Normalize path (Windows short path => long path, remove '..')
            String[] normalizedFilePaths = new String[filePaths.length];
            int i = 0;
            for (String filePath : filePaths) {
                normalizedFilePaths[i++] = new File(filePath).getCanonicalPath();
            }

            // Use the given root path or compute it
            String commonPath = null;
            if ((filesRootPath == null) || (filesRootPath.isEmpty())) {
                if (!keepAbsolutePaths) {
                    commonPath = getCommonPath(normalizedFilePaths);
                }
            } else {
                // Normalize root path
                commonPath = new File(filesRootPath).getCanonicalPath();
            }

            // Finds all the list of all the files in the directory trees
            List<String> expandedFilePaths = expandPaths(normalizedFilePaths);

            // Manifest
            // if specified take the one given in argument, otherwise take META-INF/MANIFEST.MF
            // if does not exist, create a manifest
            Manifest manifest = null;
            if ((manifestFilePath == null) || (manifestFilePath.isEmpty())) {
                manifestFilePath = commonPath + File.separator + "META-INF" + File.separator + "MANIFEST.MF";
            }
            if (new File(manifestFilePath).exists()) {
                manifest = new Manifest(new FileInputStream(manifestFilePath));
            } else {
                manifest = new Manifest();
            }

            // Now create the jar with all the files
            FileOutputStream stream = new FileOutputStream(jarFilePath);
            jarOutputStream = new JarOutputStream(stream, manifest);

            for (String filePath : expandedFilePaths) {
                File file = new File(filePath);
                String pathInJar;
                if ((commonPath != null) && (!commonPath.isEmpty())) {
                    pathInJar = getRelativePath(commonPath, filePath);
                } else {
                    if (keepAbsolutePaths) {
                        pathInJar = file.getPath();
                    } else {
                        pathInJar = file.getName();
                    }
                }

                // Skip manifest file, it will be created automatically
                if (pathInJar.equals("META-INF/MANIFEST.MF")) {
                    continue;
                }

                addFileToJarArchive(file, pathInJar, jarOutputStream);
            }

            closeJarArchive(jarOutputStream);
            jarOutputStream = null;
            return 0;
        } catch (Exception e) {
            try {
                closeJarArchive(jarOutputStream);
                deleteJarArchive(jarFilePath);
            } catch (Exception e2) {}
            throw new ScilabJavaException(String.format("Cannot create jar archive %s: %s", jarFilePath, e.getMessage()));
        }
    }

    /**
     * Recursively list all children file paths contained in a list of paths
     * @param paths the paths to expand
     */
    private static List<String> expandPaths(String... paths) throws IOException {
        ArrayList<String> expandedPaths = new ArrayList<String>();
        for (String filePath : paths) {
            File file = new File(filePath);
            if (file.isDirectory()) {
                for (File childFile : file.listFiles()) {
                    expandedPaths.addAll(expandPaths(childFile.getPath()));
                }
            } else {
                expandedPaths.add(filePath);
            }
        }
        return expandedPaths;
    }

    /**
     * Adds a file to an opened Jar archive
     * @param file the file to add
     * @param pathInJar the path of file in jar
     * @param jar the opened Jar in which the file is added
     */
    private static void addFileToJarArchive(File file, String pathInJar, JarOutputStream jar) throws IOException {
        BufferedInputStream in = null;
        try {
            pathInJar.replace("\\", "/");

            JarEntry entry = new JarEntry(pathInJar);
            entry.setTime(file.lastModified());
            jar.putNextEntry(entry);
            in = new BufferedInputStream(new FileInputStream(file));

            byte[] buffer = new byte[4096];
            while (true) {
                int count = in.read(buffer);
                if (count == -1) {
                    break;
                }
                jar.write(buffer, 0, count);
            }
            jar.closeEntry();
        } finally {
            if (in != null) {
                in.close();
            }
        }
    }

    /**
    * Returns the common path of a set of paths
    * ex: /tmp/dummy/foo and /tmp/dummy/bar/foo have /tmp/dummy as common path
    * @param paths the input paths
    */
    private static String getCommonPath(String[] paths) throws IOException {
        int nbPaths = paths.length;
        if (nbPaths == 0) {
            return "";
        } else if (nbPaths == 1) {
            File path = new File(paths[0]);
            if (path.isDirectory()) {
                return path.getAbsolutePath();
            } else {
                return path.getParentFile().getAbsolutePath();
            }
        }

        // Escape because Windows backslash crashes split (regexp)
        String fileSep = java.util.regex.Pattern.quote(System.getProperty("file.separator"));

        String commonPath = "";
        String[][] folders = new String[nbPaths][];

        // Split each path in all its folders
        int k = 0;
        for (String path : paths) {
            folders[k++] = path.split(fileSep);
        }

        // For each folder of first path
        for (int j = 0; j < folders[0].length; j++) {
            String folderToMatch = folders[0][j];

            // Compare with the folder at same position in all the other paths
            boolean allMatched = true;
            for (int i = 1; i < folders.length && allMatched; i++) {
                if (j >= folders[i].length) {
                    return commonPath;
                }
                allMatched &= (new File(folders[i][j]).compareTo(new File(folderToMatch)) == 0);
            }

            // Update common path
            if (allMatched) {
                commonPath += folderToMatch + File.separator;
            } else {
                return commonPath;
            }
        }

        return commonPath;
    }

    /**
    * Returns the relative path of a path given a base path
    * @param base the base path
    * @param path the path for which we want the relative path
    */
    private static String getRelativePath(String base, String path) {
        return new File(base).toURI().relativize(new File(path).toURI()).getPath();
    }

    /**
    * Closes a jar
    * @param jarOutputStream the jar output stream
    */
    private static void closeJarArchive(JarOutputStream jarOutputStream) throws ScilabJavaException {
        try {
            if (jarOutputStream != null) {
                jarOutputStream.close();
            }
        } catch (IOException e) {
            throw new ScilabJavaException(String.format("Cannot close jar stream: %s\n", e.getMessage()));
        }
    }

    /**
    * Deletes a jar
    * @param jarFilePath the file path of jar to be deleted
    */
    private static void deleteJarArchive(String jarFilePath) throws ScilabJavaException {
        File f = new File(jarFilePath);
        if (f.exists()) {
            if (f.canWrite()) {
                if (!f.delete()) {
                    throw new ScilabJavaException(String.format("Cannot delete jar archive %s.\n", jarFilePath));
                }
            } else {
                throw new ScilabJavaException(String.format("Cannot delete jar archive %s: File is write protected.\n", jarFilePath));
            }
        } else {
            throw new ScilabJavaException(String.format("Cannot delete jar archive %s: No such file.\n", jarFilePath));
        }
    }
}

/*
 * Scilab (http://www.scilab.org/) - This file is part of Scilab
 * Copyright (C) 2011 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.commons;

import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.lang.reflect.Method;
import java.math.BigInteger;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

/**
 * Basic utils
 * @author Calixte DENIZET
 */
public final class ScilabCommonsUtils {

    private static final int BUFFERSIZE = 8192;

    private static MessageDigest MD5;
    static {
        try {
            MD5 = MessageDigest.getInstance("MD5");
        } catch (NoSuchAlgorithmException e) {
            System.err.println(e);
            MD5 = null;
        }
    }

    /**
     * Get a MD5 string of the input string
     * @param str the string to convert
     * @return the MD5 representation of str
     */
    public static String getMD5(String str) {
        if (MD5 == null) {
            return null;
        }

        byte[] bytes = MD5.digest(str.getBytes());
        return new BigInteger(1, bytes).toString(16);
    }

    /**
     * Copy a file
     * @param inFile src file
     * @param outFile dest file
     * @return true if the operation succeeded
     */
    public static boolean copyFile(File inFile, File outFile) {
        FileReader in = null;
        FileWriter out = null;
        boolean success = false;

        try {
            in = new FileReader(inFile);
            out = new FileWriter(outFile);
            char[] buffer = new char[BUFFERSIZE];
            int n;

            while ((n = in.read(buffer)) != -1) {
                out.write(buffer, 0, n);
            }

            success = true;
        } catch (IOException e) {
            System.err.println("Error in copying file " + inFile.getAbsolutePath() + " to " + outFile.getAbsolutePath());
            System.err.println(e);
        } finally {
            if (in != null) {
                try {
                    in.close();
                } catch (IOException e) {
                    System.err.println(e);
                }
            }
            if (out != null) {
                try {
                    out.close();
                } catch (IOException e) {
                    System.err.println(e);
                }
            }
        }

        return success;
    }

    /**
     * Load on use
     * @param str the action
     */
    public static void loadOnUse(String str) {
        try {
            Class jvmLoadClassPathClass = Class.forName("org.scilab.modules.jvm.LoadClassPath");
            Method loadOnUseMethod = jvmLoadClassPathClass.getDeclaredMethod("loadOnUse", new Class[] { String.class });
            loadOnUseMethod.invoke(null, str);
        } catch (java.lang.ClassNotFoundException ex) {
            System.err.println("Could not find the Scilab class to load the export dependencies: " + ex);
        } catch (java.lang.NoSuchMethodException ex) {
            System.err.println("Could not find the Scilab method to load the export dependencies: " + ex);
        } catch (java.lang.IllegalAccessException ex) {
            System.err.println("Could not access to the Scilab method to load the export dependencies: " + ex);
        } catch (java.lang.reflect.InvocationTargetException ex) {
            System.err.println("Could not invoke the Scilab method to load the export dependencies: " + ex);
        }
    }
}

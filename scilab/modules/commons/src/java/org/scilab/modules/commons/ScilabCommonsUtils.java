/*
 * Scilab (http://www.scilab.org/) - This file is part of Scilab
 * Copyright (C) 2011 - Calixte DENIZET
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

package org.scilab.modules.commons;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.math.BigInteger;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

import javax.swing.SwingUtilities;

/**
 * Basic utils
 * @author Calixte DENIZET
 */
public final class ScilabCommonsUtils {

    private static final int BUFFERSIZE = 8192;
    private static Thread scilabThread;

    private static MessageDigest MD5;
    static {
        try {
            MD5 = MessageDigest.getInstance("MD5");
        } catch (NoSuchAlgorithmException e) {
            System.err.println(e);
            MD5 = null;
        }
    }

    public static int[] getScilabVersions() {
        return new int[] {ScilabCommons.getScilabVersionMajor(), ScilabCommons.getScilabVersionMinor(), ScilabCommons.getScilabVersionMaintenance(), ScilabCommons.getScilabVersionTimestamp()};
    }

    public static String getScilabVersion() {
        return ScilabCommons.getScilabVersionMajor() + "." + ScilabCommons.getScilabVersionMinor() + "." + ScilabCommons.getScilabVersionMaintenance();
    }

    public static String getScilabVersionAsString() {
        return ScilabCommons.getScilabVersionAsString().replaceFirst("scilab-", "");
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

        try {
            byte[] bytes = MD5.digest(str.getBytes("UTF-8"));
            return new BigInteger(1, bytes).toString(16);
        } catch (Exception e) {
            return null;
        }
    }

    /**
     * Copy a file
     * @param inFile src file
     * @param outFile dest file
     * @return true if the operation succeeded
     */
    public static boolean copyFile(File inFile, File outFile) {
        FileInputStream in = null;
        FileOutputStream out = null;
        boolean success = false;

        try {
            in = new FileInputStream(inFile);
            out = new FileOutputStream(outFile);
            byte[] buffer = new byte[BUFFERSIZE];
            int n;

            while ((n = in.read(buffer)) != -1) {
                out.write(buffer, 0, n);
            }
            out.flush();
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
     * Replace ~ by user home, SCI by Scilab main directory, ...
     * @param baseDir the base directory
     * @return correct base directory
     */
    public static String getCorrectedPath(String path) {
        String sciUnixStyle = "";
        path = path.trim();
        String pathLinux = path.replace("\\", "/");
        if (path != null && !path.isEmpty()) {
            if (path.startsWith("~" + "/") || path.equals("~")) {
                sciUnixStyle = ScilabConstants.USERHOME.replace("\\", "/");
                return path.replaceFirst("~", sciUnixStyle);
            } else if (path.startsWith("SCI/") || path.equals("SCI")) {
                try {
                    sciUnixStyle = ScilabConstants.SCI.getCanonicalPath().replace("\\", "/");
                } catch (IOException e) {
                    sciUnixStyle = ScilabConstants.SCI.getAbsolutePath().replace("\\", "/");
                }
                return pathLinux.replaceFirst("SCI", sciUnixStyle).replace("/", File.separator);
            } else if (path.startsWith("SCIHOME/") || path.equals("SCIHOME")) {
                sciUnixStyle = ScilabConstants.SCIHOME.toString().replace("\\", "/");
                return path.replaceFirst("SCIHOME", sciUnixStyle).replace("/", File.separator);
            } else if (path.startsWith("TMPDIR/") || path.equals("TMPDIR")) {
                sciUnixStyle = ScilabCommons.getTMPDIR().replace("\\", "/");
                return path.replaceFirst("TMPDIR", sciUnixStyle).replace("/", File.separator);
            }
        }

        return path;
    }

    /**
     * Load on use
     * @param str the action
     */
    public static void loadOnUse(String str) {
        final String finalStr = str;
        try {
            if (SwingUtilities.isEventDispatchThread()) {
                try {
                    Class jvmLoadClassPathClass = Class.forName("org.scilab.modules.jvm.LoadClassPath");
                    Method loadOnUseMethod = jvmLoadClassPathClass.getDeclaredMethod("loadOnUse", new Class[] { String.class });
                    loadOnUseMethod.invoke(null, finalStr);
                } catch (java.lang.ClassNotFoundException ex) {
                    System.err.println("Could not find the Scilab class to load dependency: " + ex);
                } catch (java.lang.NoSuchMethodException ex) {
                    System.err.println("Could not find the Scilab method to load dependency: " + ex);
                } catch (java.lang.IllegalAccessException ex) {
                    System.err.println("Could not access to the Scilab method to load dependency: " + ex);
                } catch (java.lang.reflect.InvocationTargetException ex) {
                    System.err.println("Could not invoke the Scilab method to load dependency: " + ex);
                }
            } else {
                SwingUtilities.invokeAndWait(new Runnable() {
                    @Override
                    public void run() {
                        try {
                            Class jvmLoadClassPathClass = Class.forName("org.scilab.modules.jvm.LoadClassPath");
                            Method loadOnUseMethod = jvmLoadClassPathClass.getDeclaredMethod("loadOnUse", new Class[] { String.class });
                            loadOnUseMethod.invoke(null, finalStr);
                        } catch (java.lang.ClassNotFoundException ex) {
                            System.err.println("Could not find the Scilab class to load dependency: " + ex);
                        } catch (java.lang.NoSuchMethodException ex) {
                            System.err.println("Could not find the Scilab method to load dependency: " + ex);
                        } catch (java.lang.IllegalAccessException ex) {
                            System.err.println("Could not access to the Scilab method to load dependency: " + ex);
                        } catch (java.lang.reflect.InvocationTargetException ex) {
                            System.err.println("Could not invoke the Scilab method to load dependency: " + ex);
                        }
                    }
                });
            }
        } catch (final InterruptedException e) {
        } catch (final InvocationTargetException e) {
        }
    }


    /**
     * Set the scilab thread as the current thread
     * Called from org.scilab.modules.core.Scilab::executeInitialHooks called itself
     * in GetCommandLine.c just after the first display of the prompt.
     */
    public static void registerScilabThread() {
        scilabThread = Thread.currentThread();
    }

    /**
     * @return true if Thread.currentThread() is the main Scilab thread
     */
    public static boolean isScilabThread() {
        return scilabThread != null && scilabThread == Thread.currentThread();
    }
}

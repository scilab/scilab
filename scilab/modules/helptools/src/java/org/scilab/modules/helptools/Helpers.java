/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Hussein SHAFIE
 * Copyright (C) 2008 - INRIA - Sylvestre LEDRU
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

import java.io.IOException;
import java.io.File;
import java.io.InputStream;
import java.io.FileInputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.FileOutputStream;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;

public final class Helpers {

    public static final boolean IS_WINDOWS = (File.pathSeparatorChar == ';');

    private Helpers() { }

    // -----------------------------------------------------------------------

    public static String reason(Throwable e) {
        String reason = e.getMessage();
        if (reason == null) {
            return e.getClass().getName();
        }

        return reason;
    }

    // -----------------------------------------------------------------------

    public static void escapeXML(String text, PrintWriter out) {
        char[] chars = text.toCharArray();
        escapeXML(chars, 0, chars.length, out);
    }

    public static void escapeXML(char[] chars, int offset, int length,
                                 PrintWriter out) {
        int end = offset + length;
        for (int i = offset; i < end; ++i) {
            char c = chars[i];

            switch (c) {
                case '\'':
                    out.write("&apos;");
                    break;
                case '\"':
                    out.write("&quot;");
                    break;
                case '<':
                    out.write("&lt;");
                    break;
                case '>':
                    out.write("&gt;");
                    break;
                case '&':
                    out.write("&amp;");
                    break;
                default:
                    out.write(c);
                    break;
            }
        }
    }

    // -----------------------------------------------------------------------

    public static String getBaseName(String language) {
        return "scilab_" + language + "_help";
    }

    public static String getTemporaryNameFo(String baseDirectory) {
        return baseDirectory + "/__doc.fo";
    }

    public static String getFileExtension(File file) {
        return getFileExtension(file.getPath());
    }

    public static String getFileExtension(String path) {
        int dot = indexOfDot(path);
        if (dot < 0) {
            return null;
        } else {
            return path.substring(dot + 1);
        }
    }

    private static int indexOfDot(String path) {
        int baseNameStart = path.lastIndexOf(File.separatorChar);
        if (baseNameStart < 0) {
            baseNameStart = 0;
        } else {
            ++baseNameStart;
        }

        int dot = -1;

        int pos = path.length();
        while ((pos = path.lastIndexOf('.', pos - 1)) > baseNameStart) {
            dot = pos;
        }

        return dot;
    }

    public static File setFileExtension(File file, String extension) {
        String path = setFileExtension(file.getPath(), extension);
        return new File(path);
    }

    public static String setFileExtension(String path, String extension) {
        if (path.endsWith(File.separator)) {
            return path;
        }

        int dot = indexOfDot(path);
        if (dot < 0) {
            if (extension == null) {
                return path;
            } else {
                return path + "." + extension;
            }
        } else {
            if (extension == null) {
                return path.substring(0, dot);
            } else {
                return path.substring(0, dot + 1) + extension;
            }
        }
    }

    // -----------------------------------------------------------------------

    public static void copyFile(File srcFile, File dstFile)
    throws IOException {
        FileInputStream src = new FileInputStream(srcFile);
        try {
            copyFile(src, dstFile);
        } finally {
            src.close();
        }
    }

    public static void copyFile(InputStream src, File dstFile)
    throws IOException {
        FileOutputStream dst = new FileOutputStream(dstFile);
        byte[] bytes = new byte[65536];
        int count;

        try {
            while ((count = src.read(bytes)) != -1) {
                dst.write(bytes, 0, count);
            }

            dst.flush();

        } finally {
            dst.close();
        }
    }

    // -----------------------------------------------------------------------

    /**
     * Delete a directory and all his content
     *
     * @param dir The path to the directory
     */
    public static void deleteDirectory(String dir) {
        deleteDirectory(new File(dir));
    }

    /**
     * Delete a directory and all his content
     *
     * @param dir The file object of the path to the directory
     */
    public static void deleteDirectory(File dir) {

        String[] files = dir.list();
        if (files == null) {
            files = new String[0];
        }
        for (int i = 0; i < files.length; i++) {
            File file = new File(dir, files[i]);
            if (file.isDirectory()) {
                Helpers.deleteDirectory(file);
            } else {
                file.delete();
            }
        }
        dir.delete();

    }

    // -----------------------------------------------------------------------

    public static String[] split(String string, char separatorChar) {
        // Count elements ---

        int elementCount = 0;
        int sep = 0;
        while ((sep = string.indexOf(separatorChar, sep)) >= 0) {
            ++elementCount;
            ++sep;
        }
        ++elementCount;

        // Build element array ---

        String[] elements = new String[elementCount];

        elementCount = 0;
        sep = 0;
        int nextSep;
        while ((nextSep = string.indexOf(separatorChar, sep)) >= 0) {
            elements[elementCount++] =
                (sep == nextSep) ? "" : string.substring(sep, nextSep);
            sep = nextSep + 1;
        }
        elements[elementCount++] = string.substring(sep);

        return elements;
    }

    // -----------------------------------------------------------------------

    public static File findInPath(String appName) {
        String pathEnvVar = System.getenv("PATH");
        if (pathEnvVar == null) {
            return null;
        }

        String[] split = Helpers.split(pathEnvVar, File.pathSeparatorChar);
        for (int i = 0; i < split.length; ++i) {
            String path = split[i].trim();
            if (path.length() == 0) {
                continue;
            }

            if (!path.endsWith(File.separator)) {
                path += File.separator;
            }

            File file = new File(path + appName);
            if (file.isFile()) {
                return file;
            }

            if (IS_WINDOWS && appName.lastIndexOf('.') < 0) {
                String[] suffixes = {".COM", ".EXE", ".BAT", ".CMD" };

                String pathExtEnvVar = System.getenv("PATHEXT");
                if (pathExtEnvVar != null) {
                    suffixes = Helpers.split(pathExtEnvVar,
                                             File.pathSeparatorChar);
                }

                for (int j = 0; j < suffixes.length; ++j) {
                    String suffix = suffixes[j].trim();
                    if (!suffix.startsWith(".")) {
                        continue;
                    }

                    file = new File(path + appName + suffix);
                    if (file.isFile()) {
                        return file;
                    }
                }
            }
        }

        return null;
    }

    public static Process shellStart(String command, String[] envp, File dir)
    throws IOException {
        Process process;

        if (IS_WINDOWS) {
            process = Runtime.getRuntime().exec(
                          new String[] {"cmd.exe", "/s", "/c", "\"" + command + "\"" },
                          envp, dir);
        } else {
            process = Runtime.getRuntime().exec(
                          new String[] {"/bin/sh", "-c", command }, envp, dir);
        }

        return process;
    }

    public static int shellExec(String command, String[] envp, File dir,
                                boolean verbose)
    throws IOException, InterruptedException {
        if (verbose) {
            System.out.println(command);
        }

        Process process = shellStart(command, envp, dir);

        // Without these consumer threads, any shell command that outputs
        // something lengthy will block on Windows NT.

        InputConsumer consumer1;
        InputConsumer consumer2;
        if (verbose) {
            consumer1 = new InputConsumer(process.getInputStream(), System.out);
            consumer2 = new InputConsumer(process.getErrorStream(), System.err);
        } else {
            consumer1 = new InputConsumer(process.getInputStream());
            consumer2 = new InputConsumer(process.getErrorStream());
        }
        consumer1.start();
        consumer2.start();

        int exitStatus = process.waitFor();

        consumer1.join();
        consumer2.join();

        return exitStatus;
    }

    private static class InputConsumer extends Thread {
        private InputStream in;
        private OutputStream out;
        private byte[] bytes = new byte[4096];

        public InputConsumer(InputStream in) {
            this(in, null);
        }

        public InputConsumer(InputStream in, OutputStream out) {
            this.in = in;
            this.out = out;
        }

        public void run() {
            for (;;) {
                int count;
                try {
                    count = in.read(bytes);
                } catch (IOException e) {
                    //e.printStackTrace();
                    count = -1;
                }
                if (count < 0) {
                    break;
                }

                if (count > 0 && out != null) {
                    try {
                        out.write(bytes, 0, count);
                        out.flush();
                    } catch (IOException e) {
                        //e.printStackTrace();
                        out = null;
                    }
                }
            }
        }
    }

    // -----------------------------------------------------------------------

    public static String loadString(File file, String charsetName)
    throws IOException {
        InputStream in = new FileInputStream(file);

        String loaded = null;
        try {
            loaded = loadString(in, charsetName);
        } finally {
            in.close();
        }

        return loaded;
    }

    public static String loadString(InputStream stream, String charsetName)
    throws IOException {
        InputStreamReader in;
        if (charsetName == null) {
            in = new InputStreamReader(stream);
        } else {
            in = new InputStreamReader(stream, charsetName);
        }

        char[] chars = new char[8192];
        StringBuffer buffer = new StringBuffer(chars.length);
        int count;

        while ((count = in.read(chars, 0, chars.length)) != -1) {
            if (count > 0) {
                buffer.append(chars, 0, count);
            }
        }

        return buffer.toString();
    }

    public static void saveString(String string, File file,
                                  String charsetName)
    throws IOException {
        OutputStream out = new FileOutputStream(file);

        try {
            saveString(string, out, charsetName);
        } finally {
            out.close();
        }
    }

    private static void saveString(String string, OutputStream stream,
                                   String charsetName)
    throws IOException {
        OutputStreamWriter out;
        if (charsetName == null) {
            out = new OutputStreamWriter(stream);
        } else {
            out = new OutputStreamWriter(stream, charsetName);
        }

        out.write(string, 0, string.length());
        out.flush();
    }
}


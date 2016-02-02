/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - 2011 - Calixte DENIZET <calixte@contrib.scilab.org>
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

import java.io.BufferedWriter;
import java.io.CharArrayWriter;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.io.Reader;
import java.io.StringWriter;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.net.MalformedURLException;
import java.net.URI;
import java.net.URLClassLoader;
import java.net.URL;
import java.util.Arrays;
import java.util.ArrayList;
import java.util.List;
import java.util.Locale;
import java.util.ServiceLoader;
import java.util.logging.Level;

import javax.swing.SwingUtilities;
import javax.tools.Diagnostic;
import javax.tools.DiagnosticCollector;
import javax.tools.FileObject;
import javax.tools.ForwardingJavaFileManager;
import javax.tools.JavaCompiler;
import javax.tools.StandardLocation;
import javax.tools.JavaFileObject;
import javax.tools.SimpleJavaFileObject;
import javax.tools.StandardJavaFileManager;
import javax.tools.ToolProvider;
import javax.tools.JavaCompiler.CompilationTask;
import javax.tools.JavaFileObject.Kind;

import org.scilab.modules.commons.ScilabCommonsUtils;

/**
 * Class to provide a java compiler to JIMS.
 * Try to find the compiler provide with JDK and if it is not found, use the Eclipse Compiler for Java
 * @author Calixte DENIZET
 */
@SuppressWarnings("serial")
public class ScilabJavaCompiler {

    private static final String JAVACOMPILER = "javax.tools.JavaCompiler";
    private static final String BINPATH = System.getProperty("java.io.tmpdir") + File.separator + "JIMS" + File.separator + "bin";

    private static JavaCompiler compiler;
    private static boolean ecjLoaded = false;

    private static boolean isECJ;


    static {
        new File(System.getProperty("java.io.tmpdir") + File.separator + "JIMS").mkdir();
        new File(BINPATH).mkdir();
        try {
            URL binURL = new File(BINPATH).toURI().toURL();
            addURLToClassPath(binURL);
        } catch (MalformedURLException e) {
            System.err.println(e);
        }
    }

    /**
     * Just find a compiler
     */
    private static void findCompiler() throws ScilabJavaException {
        if (compiler == null) {
            try {
                compiler = ToolProvider.getSystemJavaCompiler();
            } catch (Exception e) { }

            if (compiler == null) {
                ServiceLoader<JavaCompiler> jcompilers = ServiceLoader.load(JavaCompiler.class);
                for (JavaCompiler jc : jcompilers) {
                    if (jc != null) {
                        compiler = jc;
                        break;
                    }
                }
            }

            if (compiler == null) {
                if (ecjLoaded) {
                    throw new ScilabJavaException("No java compiler in the classpath\nCheck for tools.jar (comes from JDK) or ecj-4.4.x.jar (Eclipse Compiler for Java)");
                }

                // Compiler should be in thirdparty so we load it
                ScilabCommonsUtils.loadOnUse("external_objects_java");
                ecjLoaded = true;
                findCompiler();
            }

            isECJ = compiler.getClass().getSimpleName().indexOf("Eclipse") != -1;
        }
    }

    /**
     * Compile code got as string
     * @param className the class name
     * @param code the lines giving the code to compile
     * @return an integer corresponding to the compiled and loaded class.
     */
    public static int compileCode(String className, String[] code) throws ScilabJavaException {
        if (SwingUtilities.isEventDispatchThread()) {
            findCompiler();
        } else {
            try {
                SwingUtilities.invokeAndWait(new Runnable() {
                    @Override
                    public void run() {
                        try {
                            findCompiler();
                        } catch (ScilabJavaException e) {
                            // TODO Auto-generated catch block
                            e.printStackTrace();
                        }
                    }
                });
            } catch (InvocationTargetException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            } catch (InterruptedException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            }
        }

        DiagnosticCollector<JavaFileObject> diagnostics = new DiagnosticCollector<JavaFileObject>();
        StandardJavaFileManager stdFileManager = compiler.getStandardFileManager(null, Locale.getDefault(), null);
        String cp = null;

        if (isECJ) {
            // it seems that with the embedded ecj, the only way to set the cp is to use java.class.path...
            cp = getClasspath();
            System.setProperty("java.class.path", cp + File.pathSeparatorChar + System.getProperty("java.class.path"));
        } else {
            try {
                stdFileManager.setLocation(StandardLocation.CLASS_PATH, getClasspathFiles());
            } catch (Exception e) { }
        }

        ClassFileManager manager = new ClassFileManager(stdFileManager);
        List<SimpleJavaFileObject> compilationUnits = new ArrayList<SimpleJavaFileObject>();
        CharArrayWriter caw = new CharArrayWriter();
        BufferedWriter out = new BufferedWriter(caw);
        boolean isFile = true;
        SourceString sourceString = null;
        for (String s : code) {
            File f = new File(s);
            if (!f.exists() || !f.canRead()) {
                isFile = false;
                break;
            }
        }

        if (isFile) {
            for (String s : code) {
                File f = new File(s);
                compilationUnits.add(new SourceFile(f));
            }
        } else {
            sourceString = new SourceString(className, code);
            compilationUnits.add(sourceString);
        }

        String[] compileOptions = new String[] {"-d", BINPATH};
        Iterable<String> options = Arrays.asList(compileOptions);

        CompilationTask task = compiler.getTask(out, manager, diagnostics, options, null, compilationUnits);
        boolean success = task.call();

        if (cp != null) {
            final String s = System.getProperty("java.class.path").replace(cp + File.pathSeparatorChar, "");
            System.setProperty("java.class.path", s);
        }

        String error = "";

        try {
            out.flush();
            error = caw.toString();
        } catch (IOException e) {

        } finally {
            try {
                out.close();
            } catch (IOException e) { }
        }

        if (success) {
            if (isFile) {
                return -1;
            } else {
                if (manager.className != null && !manager.className.isEmpty()) {
                    return ScilabClassLoader.loadJavaClass(BINPATH, manager.className);
                } else {
                    return -1;
                }
            }
        } else {
            if (!isECJ) {
                error = getCompilerErrors(diagnostics);
            }

            throw new ScilabJavaException(error);
        }
    }

    /**
     * Returns the compilation errors from the diagnostics
     * @param diagnostics the diagnostics returned by the compiler
     * @return a string containing the errors
     */
    public static String getCompilerErrors(DiagnosticCollector<JavaFileObject> diagnostics) {
        StringBuffer buffer = new StringBuffer();
        int cpt = 1;
        buffer.append("----------\n");
        for (Diagnostic <? extends JavaFileObject > d : diagnostics.getDiagnostics()) {
            buffer.append(Integer.toString(cpt++)).append(". ").append(d.getKind());
            if (d.getSource() != null) {
                buffer.append(" in ").append(d.getSource().toUri().getPath()).append(" (at line ").append(Long.toString(d.getLineNumber())).append(")\n");

                Reader reader = null;
                try {
                    reader = d.getSource().openReader(true);
                    reader.skip(d.getStartPosition());
                    char[] data = new char[(int) (d.getEndPosition() - d.getStartPosition() + 1)];
                    reader.read(data);
                    buffer.append("        ").append(data).append('\n');
                    Arrays.fill(data, '^');
                    buffer.append("        ").append(data).append('\n');
                } catch (IOException e) {

                } finally {
                    if (reader != null) {
                        try {
                            reader.close();
                        } catch (IOException e) { }
                    }
                }
            } else {
                // this is not a file related error
                buffer.append('\n');
            }
            buffer.append(d.getMessage(Locale.getDefault())).append('\n');
        }

        buffer.append("----------\n");
        return buffer.toString();
    }

    /**
     * Get the current classpath with the correct separator (':' under Unix and ';' under Windows)
     * @return the classpath
     */
    public static String getClasspath() {
        URLClassLoader loader = (URLClassLoader) ClassLoader.getSystemClassLoader();
        URL[] urls = loader.getURLs();
        StringBuffer buffer = new StringBuffer();

        for (URL url : urls) {
            buffer.append(url.getPath()).append(File.pathSeparatorChar);
        }
        buffer.append(".");

        return buffer.toString();
    }

    /**
     * Get the files in the classpath
     * @return the files
     */
    public static List<File> getClasspathFiles() {
        URLClassLoader loader = (URLClassLoader) ClassLoader.getSystemClassLoader();
        URL[] urls = loader.getURLs();
        List<File> files = new ArrayList<File>(urls.length);

        for (URL url : urls) {
            try {
                files.add(new File(url.toURI()));
            } catch (Exception e) { }
        }

        return files;
    }

    /**
     * Add a class in the classpath
     * @param url the class url
     */
    public static void addURLToClassPath(URL url) {
        URLClassLoader sysloader = (URLClassLoader) ClassLoader.getSystemClassLoader();
        try {
            final Method method = URLClassLoader.class.getDeclaredMethod("addURL", new Class[] {URL.class});
            method.setAccessible(true);
            method.invoke(sysloader , new Object[] {url});
        } catch (NoSuchMethodException e) {
            System.err.println("Error: Cannot find the declared method: " + e.getLocalizedMessage());
        } catch (IllegalAccessException e) {
            System.err.println("Error: Illegal access: " + e.getLocalizedMessage());
        } catch (InvocationTargetException e) {
            System.err.println("Error: Could not invocate target: " + e.getLocalizedMessage());
        }
    }

    /**
     * Inner class to handle String as File
     */
    private static class SourceString extends SimpleJavaFileObject {

        private final String code;

        private SourceString(String className, String[] code) {
            super(new File(BINPATH + "/" + className.replace('.', '/') + Kind.SOURCE.extension).toURI(), Kind.SOURCE);

            StringBuffer buf = new StringBuffer();
            for (String str : code) {
                buf.append(str);
                buf.append("\n");
            }
            this.code = buf.toString();
        }

        public CharSequence getCharContent(boolean ignoreEncodingErrors) {
            return code;
        }
    }

    /**
     * Inner class to handle String as File
     */
    private static class SourceFile extends SimpleJavaFileObject {

        final File f;

        private SourceFile(File f) {
            super(f.toURI(), Kind.SOURCE);
            this.f = f;
        }

        public CharSequence getCharContent(boolean ignoreEncodingErrors) {
            try {
                FileReader reader = new FileReader(f);
                char[] buffer = new char[1024];
                StringBuffer sb = new StringBuffer();
                int r;

                while ((r = reader.read(buffer, 0, 1024)) != -1) {
                    sb.append(buffer, 0, r);
                }

                reader.close();

                return sb;
            } catch (Exception e) {
                return null;
            }
        }
    }

    private static class ClassFileManager extends ForwardingJavaFileManager {

        String className;

        public ClassFileManager(StandardJavaFileManager standardManager) {
            super(standardManager);
        }

        @Override
        public JavaFileObject getJavaFileForOutput(Location location, String className, Kind kind, FileObject sibling) throws IOException {
            if (sibling instanceof SourceString && className != null && className.indexOf('$') == -1) {
                this.className = className.replace('/', '.');
            }

            if (ScilabJavaObject.debug) {
                ScilabJavaObject.logger.log(Level.INFO, "Compilation of class \'" + className + "\'");
            }

            return super.getJavaFileForOutput(location, className, kind, sibling);
        }
    }
}

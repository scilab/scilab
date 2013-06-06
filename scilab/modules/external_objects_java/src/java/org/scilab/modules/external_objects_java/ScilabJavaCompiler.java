/*
 * JIMS ( http://forge.scilab.org/index.php/p/JIMS/ ) - This file is a part of JIMS
 * Copyright (C) 2010 - 2011 - Calixte DENIZET <calixte@contrib.scilab.org>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.external_objects_java;

import java.io.File;
import java.io.StringWriter;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.net.MalformedURLException;
import java.net.URLClassLoader;
import java.net.URI;
import java.net.URL;
import java.util.Arrays;
import java.util.Locale;
import java.util.ServiceLoader;
import java.util.logging.Level;

import javax.tools.Diagnostic;
import javax.tools.DiagnosticCollector;
import javax.tools.JavaCompiler;
import javax.tools.JavaFileObject;
import javax.tools.SimpleJavaFileObject;
import javax.tools.StandardJavaFileManager;
import javax.tools.ToolProvider;
import javax.tools.JavaCompiler.CompilationTask;
import javax.tools.JavaFileObject.Kind;

/**
 * Class to provide a java compiler to JIMS.
 * Try to find the compiler provide with JDK and if it is not found, use the Eclipse Compiler for Java
 * @author Calixte DENIZET
 */
@SuppressWarnings("serial")
public class ScilabJavaCompiler {

    private static final String JAVACOMPILER = "javax.tools.JavaCompiler";
    private static final String binpath = System.getProperty("java.io.tmpdir") + File.separator + "JIMS" + File.separator + "bin";

    private static JavaCompiler compiler;

    static {
        new File(System.getProperty("java.io.tmpdir") + File.separator + "JIMS").mkdir();
        new File(binpath).mkdir();
        try {
            URL binURL = new File(binpath).toURI().toURL();
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
                throw new ScilabJavaException("No java compiler in the classpath\nCheck for tools.jar (comes from JDK) or ecj-3.6.x.ajr (Eclipse Compiler for Java)");
            }
        }
    }

    /**
     * Compile code got as string
     * @param className the class name
     * @param code the lines giving the code to compile
     * @return an integer corresponding to the compiled and loaded class.
     */
    public static int compileCode(String className, String[] code) throws ScilabJavaException {
        findCompiler();

        if (ScilabJavaObject.debug) {
            ScilabJavaObject.logger.log(Level.INFO, "Compilation of class \'" + className + "\'");
        }

        DiagnosticCollector<JavaFileObject> diagnostics = new DiagnosticCollector<JavaFileObject>();

        SourceString file = new SourceString(className, code);
        StandardJavaFileManager stdFileManager = compiler.getStandardFileManager(null, Locale.getDefault(), null);
        Iterable <? extends JavaFileObject > compilationUnits = Arrays.asList(file);
        String[] compileOptions = new String[] {"-d", binpath} ;
        Iterable<String> options = Arrays.asList(compileOptions);

        CompilationTask task = compiler.getTask(null, stdFileManager, diagnostics, options, null, compilationUnits);

        boolean success = task.call();
        StringBuffer buf = new StringBuffer();
        for (Diagnostic diagnostic : diagnostics.getDiagnostics()) {
            buf.append(diagnostic.toString());
            buf.append("\n");
        }

        if (success) {
            return ScilabClassLoader.loadJavaClass(className, true);
        } else {
            throw new ScilabJavaException(buf.toString());
        }
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
            super(new File(binpath + "/" + className.replace('.', '/') + Kind.SOURCE.extension).toURI(), Kind.SOURCE);

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
}

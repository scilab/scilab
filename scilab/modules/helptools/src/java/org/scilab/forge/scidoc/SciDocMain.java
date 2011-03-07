/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.forge.scidoc;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;

import java.util.HashMap;
import java.util.Map;

import org.scilab.modules.commons.ScilabConstants;
import org.scilab.modules.helptools.BuildJavaHelp;
import org.scilab.modules.helptools.Helpers;

import org.scilab.forge.scidoc.external.HTMLMathMLHandler;
import org.scilab.forge.scidoc.external.HTMLSVGHandler;

/**
 * Class to convert DocBook to HTML
 * @author Calixte DENIZET
 */
public final class SciDocMain {

    private static final String SCI = ScilabConstants.SCI.getPath();
    private static final String VERSION = SCI + "/Version.incl";

    private String input;
    private String outputDirectory;
    private String language;
    private String format;
    private String template;
    private String sciprim;
    private String scimacro;
    private String version;
    private String imagedir;

    /**
     * Set the directory where files must be exported
     * Note that directory is created
     *
     * @param outputDirectoryectory The path to the directory
     * @return If the directory exists
     */
    public boolean setOutputDirectory(String outputDirectory) {
        File directory = new File(outputDirectory);
        if (!directory.isDirectory()) {
            if (!directory.mkdirs()) {
                return false;
            }
        }
        this.outputDirectory = outputDirectory;
        return true;
    }

    /**
     * Defines the language
     *
     * @param language the language (xx_XX ex: en_US, fr_FR)
     */
    public void setWorkingLanguage(String language) {
        this.language = language;
    }

    /**
     * Defines the export format
     * @param format the format (among the list CHM, HTML, PDF, JH, PS)
     */
    public void setExportFormat(String format) {
        this.format = format;
    }

    public static void main(String[] args) {
        SciDocMain sciDoc = new SciDocMain();
        sciDoc.buildDocumentationCommandLine(args);
    }

    public void buildDocumentationCommandLine(String[] args) {
        Map<String, String> map = parseCommandLine(args);
        if (map.containsKey("help")) {
            System.out.println("Usage scidoc [OPTION]... file");
            System.out.println("SciDoc is a tool to generate html, chm or javahelp files from DocBook");
            System.out.println("");
            System.out.println("-input        Input DocBook file");
            System.out.println("-output       The output directory");
            System.out.println("-template     A template file used for the generation");
            System.out.println("-imagedir     The directory which will contain the generated images");
            System.out.println("              the path is relative to output");
            System.out.println("-javahelp     No expected argument, just precise the kind of output");
            System.out.println("-html         No expected argument, just precise the kind of output (default)");
            System.out.println("-web          No expected argument, just precise the kind of output");
            System.out.println("-sciprim      A file containing the list of the Scilab primitives");
            System.out.println("-scimacro     A file containing the list of the Scilab macros");
            System.out.println("-version      A string with the version of Scilab");
            //System.out.println("-checklast    true or false, just say if the date of the last generation");
            //System.out.println("              must be checked, it could be useful to regenerate only the changed files");
            System.out.println("");
            System.out.println("Report bugs on: <http://bugzilla.scilab.org>");
            System.out.println("Project page: <http://forge.scilab.org/index.php/p/scidoc>");
            return;
        }

        if (!map.containsKey("input")) {
            System.err.println("No input file");
            System.err.println("Use the option -help");
            return;
        }

        if (!map.containsKey("template")) {
            System.err.println("No template to generate files");
            System.err.println("Use the option -help");
            return;
        }

        if (!map.containsKey("imagedir")) {
            System.err.println("No image directory specified");
            System.err.println("Use the option -help");
            return;
        }

        input = map.get("input");
        outputDirectory = map.get("output");
        if (outputDirectory == null || outputDirectory.length() == 0) {
            outputDirectory = ".";
        }
        template = map.get("template");
        sciprim = map.get("sciprim");
        scimacro = map.get("scimacro");
        version = map.get("version");
        imagedir = map.get("imagedir");

		process(input, "");
    }

    /* Stylesheet is useless and just kept to keep the consistency with
     * builddoc V1 */
    public String process(String sourceDoc, String styleSheet)  {
        template = SCI + "/modules/helptools/data/template/template_" + format.toLowerCase() + ".html";
        /* TODO: make this file generated at build time of Scilab */
        sciprim = SCI + "/modules/helptools/data/configuration/scilab_primitives.txt";
        scimacro = SCI + "/modules/helptools/data/configuration/scilab_macros.txt";
        version = getVersion(version);
        imagedir = ".";//the path must be relative to outputDirectory


        if (!new File(sourceDoc).isFile()) {
            System.err.println("Could not find master document: " + sourceDoc);
			return null;
        }

        if (!new File(template).isFile()) {
            System.err.println("Could not find template document: " + template);
			return null;
        }

        boolean checkLast = false;//Boolean.parseBoolean(map.get("checklast"));
        try {
            DocbookTagConverter converter = null;
            if (format.equalsIgnoreCase("javahelp")) {
                converter = new JavaHelpDocbookTagConverter(sourceDoc, outputDirectory, sciprim, scimacro, template, version, imagedir, checkLast);
            } else if (format.equalsIgnoreCase("html") || format.equalsIgnoreCase("web")) {
                converter = new HTMLDocbookTagConverter(sourceDoc, outputDirectory, sciprim, scimacro, template, version, imagedir, checkLast);
            } else if (format.equalsIgnoreCase("chm")) {
                converter = new CHMDocbookTagConverter(sourceDoc, outputDirectory, sciprim, scimacro, template, version, imagedir, checkLast, language);
            }

            converter.registerExternalXMLHandler(new HTMLMathMLHandler(outputDirectory, imagedir));
            converter.registerExternalXMLHandler(new HTMLSVGHandler(outputDirectory, imagedir));
            converter.convert();
            Helpers.copyFile(new File(SCI + "/modules/helptools/data/css/scilab_code.css"), new File(outputDirectory + "/scilab_code.css"));
            Helpers.copyFile(new File(SCI + "/modules/helptools/data/css/style.css"), new File(outputDirectory + "/style.css"));
            if (format.equalsIgnoreCase("javahelp")) {
                BuildJavaHelp.buildJavaHelp(outputDirectory, language); // replace en_US by language
            }

        } catch (Exception e) {
            System.err.println("An error occured during the conversion:\n");
            e.printStackTrace();
        }
        return outputDirectory;
    }

    /**
     * Get the version in Version.incl if ver==null.
     * @param ver the actual version
     * @return the version in Version.incl
     */
    private static String getVersion(String ver) {
        String ret = "";
        if (ver == null) {
            try {
                BufferedReader reader = new BufferedReader(new FileReader(VERSION));
                ret = reader.readLine().split("=")[1];
                reader.close();
            } catch (IOException e) { }
        } else {
            ret = ver;
        }

        return ret;
    }

    /**
     * Parse the command line
     * @param args the command line arguments
     * @return a map argsname-&gt;argsvalue
     */
    private static Map<String, String> parseCommandLine(String[] args) {
        String option = null;
        boolean in = false;
        Map<String, String> map = new HashMap();
        for (int i = 0; i < args.length; i++) {
            if (args[i].length() >= 2 && args[i].charAt(0) == '-') {
                if (option != null) {
                    map.put(option, "");
                    option = null;
                    option = args[i];
                }
                if (args[i].charAt(1) == '-') {
                    option = args[i].substring(2);
                } else {
                    option = args[i].substring(1);
                }
            } else {
                if (option != null) {
                    map.put(option, args[i]);
                    option = null;
                } else if (!in) {
                    map.put("input", args[i]);
                    in = true;
                } else {
                    System.err.println("Not an argument " + args[i]);
                    return null;
                }
            }
        }
        if (option != null) {
            map.put(option, "");
        }

        return map;
    }
}

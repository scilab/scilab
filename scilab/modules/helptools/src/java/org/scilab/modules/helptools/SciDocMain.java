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

package org.scilab.modules.helptools;

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

import org.scilab.modules.helptools.external.HTMLMathMLHandler;
import org.scilab.modules.helptools.external.HTMLSVGHandler;

/**
 * Class to convert DocBook to HTML
 * @author Calixte DENIZET
 */
public final class SciDocMain {

    private static final String SCI = ScilabConstants.SCI.getPath();

    private String outputDirectory;
    private String language;
    private String format;
    private String template;
    private String version;
    private String imagedir;
    private String[] sciprim;
    private String[] scimacro;
    private boolean isToolbox;

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
     * @param isToolbox must be true if we compile a toolbox doc
     */
    public void setIsToolbox(boolean isToolbox) {
        this.isToolbox = isToolbox;
    }

    /**
     * Defines the export format
     * @param format the format (among the list CHM, HTML, PDF, JH, PS)
     */
    public void setExportFormat(String format) {
        this.format = format;
    }

    /* Stylesheet is useless and just kept to keep the consistency with
     * builddoc V1 */
    public String process(String sourceDoc, String styleSheet)  {
        SciDocConfiguration conf = new SciDocConfiguration();
        template = conf.getTemplate(format.toLowerCase());
        /* TODO: make this file generated at build time of Scilab */
        sciprim = conf.getBuiltins();
        scimacro = conf.getMacros();
        version = conf.getVersion();
        imagedir = ".";//the path must be relative to outputDirectory

        if (!new File(sourceDoc).isFile()) {
            System.err.println("Could not find master document: " + sourceDoc);
            return null;
        }

        if (!new File(template).isFile()) {
            System.err.println("Could not find template document: " + template);
            return null;
        }

        try {
            DocbookTagConverter converter = null;
            String urlBase = null;

            if (format.equalsIgnoreCase("javahelp")) {
                converter = new JavaHelpDocbookTagConverter(sourceDoc, outputDirectory, sciprim, scimacro, template, version, imagedir, isToolbox, "scilab://");
            } else {
                if (isToolbox) {
                    urlBase = conf.getWebSiteURL() + language + "/";
                }
                if (format.equalsIgnoreCase("html") || format.equalsIgnoreCase("web")) {
                    converter = new HTMLDocbookTagConverter(sourceDoc, outputDirectory, sciprim, scimacro, template, version, imagedir, isToolbox, urlBase);
                } else if (format.equalsIgnoreCase("chm")) {
                    converter = new CHMDocbookTagConverter(sourceDoc, outputDirectory, sciprim, scimacro, template, version, imagedir, conf.getWebSiteURL(), isToolbox, urlBase, language);
                }
            }

            converter.registerExternalXMLHandler(new HTMLMathMLHandler(outputDirectory, imagedir));
            converter.registerExternalXMLHandler(new HTMLSVGHandler(outputDirectory, imagedir));
            converter.convert();
            Helpers.copyFile(new File(SCI + "/modules/helptools/data/css/scilab_code.css"), new File(outputDirectory + "/scilab_code.css"));
            Helpers.copyFile(new File(SCI + "/modules/helptools/data/css/xml_code.css"), new File(outputDirectory + "/xml_code.css"));
            Helpers.copyFile(new File(SCI + "/modules/helptools/data/css/c_code.css"), new File(outputDirectory + "/c_code.css"));
            Helpers.copyFile(new File(SCI + "/modules/helptools/data/css/style.css"), new File(outputDirectory + "/style.css"));
            Helpers.copyFile(new File(SCI + "/modules/gui/images/icons/media-playback-start.png"), new File(outputDirectory + "/ScilabExecute.png"));
            Helpers.copyFile(new File(SCI + "/modules/gui/images/icons/accessories-text-editor.png"), new File(outputDirectory + "/ScilabEdit.png"));
            if (format.equalsIgnoreCase("javahelp")) {
                if (!isToolbox) {
                    Helpers.copyFile(new File(SCI + "/modules/helptools/data/pages/error.html"), new File(outputDirectory + "/ScilabErrorPage.html"));
                    File homepage = new File(SCI + "/modules/helptools/data/pages/homepage-" + language + ".html");
                    if (!homepage.isFile()) {
                        /* could not find the localized homepage. Switch to english */
                        homepage = new File(SCI + "/modules/helptools/data/pages/homepage-en_US.html");
                    }
                    Helpers.copyFile(homepage, new File(outputDirectory + "/ScilabHomePage.html"));

                    File homepageImage = new File(SCI + "/modules/helptools/data/pages/ban-" + language + ".png");
                    if (!homepageImage.isFile()) {
                        homepageImage = new File(SCI + "/modules/helptools/data/pages/ban-en_US.png");
                    }
                    Helpers.copyFile(homepageImage, new File(outputDirectory + "/ban_en_US.png"));
                }
                BuildJavaHelp.buildJavaHelp(outputDirectory, language);
            }

        } catch (Exception e) {
            System.err.println("An error occured during the conversion:\n");
            e.printStackTrace();
        }

        return outputDirectory;
    }
}

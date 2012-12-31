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

import java.io.File;

import org.xml.sax.SAXException;

import org.scilab.modules.commons.ScilabCommonsUtils;
import org.scilab.modules.commons.ScilabConstants;
import org.scilab.modules.gui.utils.ScilabSwingUtilities;
import org.scilab.modules.helptools.external.HTMLMathMLHandler;
import org.scilab.modules.helptools.external.HTMLScilabHandler;
import org.scilab.modules.helptools.external.HTMLSVGHandler;
import org.scilab.modules.helptools.image.ScilabImageConverter;

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
        this.outputDirectory = new File(outputDirectory).getAbsolutePath();
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
        String imageOut = outputDirectory;
        String fileToExec = null;

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
                converter = new JavaHelpDocbookTagConverter(sourceDoc, outputDirectory, sciprim, scimacro, template, version, imagedir, isToolbox, "scilab://", language);
                if (!isToolbox) {
                    imageOut = ((JavaHelpDocbookTagConverter) converter).outImages;
                }
            } else {
                if (isToolbox) {
                    urlBase = conf.getWebSiteURL() + language + "/";
                }
                if (format.equalsIgnoreCase("html")) {
                    converter = new HTMLDocbookTagConverter(sourceDoc, outputDirectory, sciprim, scimacro, template, version, imagedir, isToolbox, urlBase, language, HTMLDocbookTagConverter.GenerationType.HTML);
                } else if (format.equalsIgnoreCase("web")) {
                    converter = new HTMLDocbookTagConverter(sourceDoc, outputDirectory, sciprim, scimacro, template, version, imagedir, isToolbox, urlBase, language, HTMLDocbookTagConverter.GenerationType.WEB);
                } else if (format.equalsIgnoreCase("chm")) {
                    converter = new CHMDocbookTagConverter(sourceDoc, outputDirectory, sciprim, scimacro, template, version, imagedir, conf.getWebSiteURL(), isToolbox, urlBase, language);
                }
            }

            converter.registerExternalXMLHandler(HTMLMathMLHandler.getInstance(imageOut, imagedir));
            converter.registerExternalXMLHandler(HTMLSVGHandler.getInstance(imageOut, imagedir));
            converter.registerExternalXMLHandler(HTMLScilabHandler.getInstance(imageOut, imagedir));
            converter.convert();

            HTMLMathMLHandler.clean();
            HTMLSVGHandler.clean();
            HTMLScilabHandler.clean();

            fileToExec = ScilabImageConverter.getFileWithScilabCode();

            ScilabCommonsUtils.copyFile(new File(SCI + "/modules/helptools/data/css/scilab_code.css"), new File(outputDirectory + "/scilab_code.css"));
            ScilabCommonsUtils.copyFile(new File(SCI + "/modules/helptools/data/css/xml_code.css"), new File(outputDirectory + "/xml_code.css"));
            ScilabCommonsUtils.copyFile(new File(SCI + "/modules/helptools/data/css/c_code.css"), new File(outputDirectory + "/c_code.css"));
            ScilabCommonsUtils.copyFile(new File(SCI + "/modules/helptools/data/css/style.css"), new File(outputDirectory + "/style.css"));
            ScilabCommonsUtils.copyFile(new File(ScilabSwingUtilities.findIcon("media-playback-start")), new File(imageOut + "/ScilabExecute.png"));
            ScilabCommonsUtils.copyFile(new File(ScilabSwingUtilities.findIcon("accessories-text-editor")), new File(imageOut + "/ScilabEdit.png"));
            ScilabCommonsUtils.copyFile(new File(ScilabSwingUtilities.findIcon("dialog-information")), new File(imageOut + "/ScilabNote.png"));
            ScilabCommonsUtils.copyFile(new File(ScilabSwingUtilities.findIcon("dialog-warning")), new File(imageOut + "/ScilabWarning.png"));
            ScilabCommonsUtils.copyFile(new File(ScilabSwingUtilities.findIcon("dialog-warning")), new File(imageOut + "/ScilabCaution.png"));
            ScilabCommonsUtils.copyFile(new File(ScilabSwingUtilities.findIcon("dialog-information")), new File(imageOut + "/ScilabTip.png"));
            ScilabCommonsUtils.copyFile(new File(ScilabSwingUtilities.findIcon("emblem-important")), new File(imageOut + "/ScilabImportant.png"));
            if (format.equalsIgnoreCase("javahelp")) {
                if (!isToolbox) {
                    ScilabCommonsUtils.copyFile(new File(SCI + "/modules/helptools/data/pages/error.html"), new File(outputDirectory + "/ScilabErrorPage.html"));
                    File homepage = new File(SCI + "/modules/helptools/data/pages/homepage-" + language + ".html");
                    if (!homepage.isFile()) {
                        /* could not find the localized homepage. Switch to english */
                        homepage = new File(SCI + "/modules/helptools/data/pages/homepage-en_US.html");
                    }
                    ScilabCommonsUtils.copyFile(homepage, new File(outputDirectory + "/ScilabHomePage.html"));

                    File homepageImage = new File(SCI + "/modules/helptools/data/pages/ban-" + language + ".png");
                    if (!homepageImage.isFile()) {
                        homepageImage = new File(SCI + "/modules/helptools/data/pages/ban-en_US.png");
                    }
                    ScilabCommonsUtils.copyFile(homepageImage, new File(imageOut + "/ban_en_US.png"));
                }

                if (fileToExec == null) {
                    generateJavahelp(outputDirectory, language, isToolbox);
                }
            }

        } catch (SAXException e) {
            System.err.println("An error occurred during the conversion:");
            System.err.println(e.toString());
        } catch (Exception e) {
            System.err.println("An error occurred during the conversion:\n");
            e.printStackTrace();
        }

        return fileToExec;
    }

    public static void generateJavahelp(String outputDirectory, String language, boolean isToolbox) {
        BuildJavaHelp.buildJavaHelp(outputDirectory, language);
        if (!isToolbox) {
            BuildJavaHelp.buildJarImages(SCI + "/modules/helptools/images", SCI + "/modules/helptools/jar");
        }
    }
}

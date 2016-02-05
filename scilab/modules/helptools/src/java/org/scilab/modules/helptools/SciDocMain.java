/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
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
import org.scilab.modules.commons.ScilabConstants;
import org.scilab.modules.helptools.Converter.Backend;
import org.scilab.modules.helptools.image.ImageConverter;
import org.xml.sax.SAXException;

/**
 * Class to convert DocBook
 * @author Calixte DENIZET
 */
public final class SciDocMain {
    /**
     * The output directory
     */
    private String outputDirectory;
    /**
     * The target language as locale string descriptor (en_US, fr_FR, ...)
     */
    private String language;
    /**
     * Target format
     */
    private Backend format;
    /**
     * Image directory relative to the outputDirectory
     */
    private String imagedir = ".";
    /**
     * The configuration of the current generation
     */
    private SciDocConfiguration conf = new SciDocConfiguration();
    /**
     * True if the documentation should be generated for a toolbox, false for Scilab
     */
    private boolean isToolbox;

    /*
     * GIWS Exported methods
     */

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
        final String f = format.toUpperCase().replace('-', '_');
        try {
            this.format = Enum.valueOf(Backend.class, f);
            return;
        } catch (IllegalArgumentException e) {
        }

        Backend[] values = Backend.values();
        final StringBuilder str = new StringBuilder();
        str.append('[');
        str.append(values[0]);
        for (int i = 1; i < values.length; i++) {
            str.append(',').append(' ');
            str.append(values[i].toString().toLowerCase());
        }
        str.append(']');
        System.err.printf("%s is not a supported format : one of %s expected.\n", format.toString(), str);

    }

    /* Stylesheet is useless and just kept to keep the consistency with
     * builddoc V1 */
    public String process(String sourceDoc, String styleSheet) throws Throwable  {
        String fileToExec = null;

        if (!new File(sourceDoc).isFile()) {
            System.err.println("Could not find master document: " + sourceDoc);
            return null;
        }

        try {
            Converter converter = null;
            ImageConverter imgConvert = new ImageConverter();

            switch (format) {
                case JAVAHELP:
                    if (!isToolbox) {
                        imgConvert.loadMD5s(ScilabConstants.SCI.getPath() + "/modules/helptools/etc");
                    }
                    converter = new JavaHelpDocbookTagConverter(sourceDoc, this, imgConvert);
                    break;
                case HTML:
                    converter = new HTMLDocbookTagConverter(sourceDoc, this, imgConvert);
                    break;
                case WEB:
                    if (!isToolbox) {
                        imgConvert.loadMD5s(ScilabConstants.SCI.getPath() + "/modules/helptools/etc");
                    }
                    converter = new HTMLDocbookTagConverter(sourceDoc, this, imgConvert);
                    break;
                case CHM:
                    if (!isToolbox) {
                        imgConvert.loadMD5s(ScilabConstants.SCI.getPath() + "/modules/helptools/etc");
                    }
                    converter = new CHMDocbookTagConverter(sourceDoc, this, imgConvert);
                    break;
                case FO:
                    converter = new FODocbookTagConverter(sourceDoc, this, imgConvert);
                    break;
                case JAR_ONLY:
                    converter = new JarOnlyConverter(this);
                    break;
                case PDF:
                case PS:
                    converter = new FopConverter(this);
                    break;
                default:
                    System.err.printf("%s is not a supported format.\n", format);
                    return null;
            }

            converter.registerAllExternalXMLHandlers();

            converter.convert();

            converter.install();

            if (imgConvert.getScilabImageConverter() != null) {
                fileToExec = imgConvert.getScilabImageConverter().getFileWithScilabCode();
            }

        } catch (SAXException e) {
            System.err.println("XML parsing error while generating documentation for file " + sourceDoc + ":");
            System.err.println(e.toString());
        } catch (Throwable e) {
            System.err.println("Unhandled error while generating documentation for file " + sourceDoc + ":");
            e.printStackTrace();
            throw e;
        }

        return fileToExec;
    }

    /*
     * Getters
     */

    public String getOutputDirectory() {
        return outputDirectory;
    }

    public String getLanguage() {
        return language;
    }

    public Backend getFormat() {
        return format;
    }

    public String getImagedir() {
        return imagedir;
    }

    public SciDocConfiguration getConf() {
        return conf;
    }

    public boolean isToolbox() {
        return isToolbox;
    }
}

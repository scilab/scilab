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

package org.scilab.modules.helptools.external;

import java.io.File;
import java.util.HashMap;
import java.util.Map;

import org.xml.sax.Attributes;

import org.scilab.modules.helptools.image.ImageConverter;

/**
 * Handle the included SCILAB code
 * @author Calixte DENIZET
 */
public class HTMLScilabHandler extends ExternalXMLHandler {

    private static final String SCILAB = "image";
    private static final String BASENAME = "_";

    private static HTMLScilabHandler instance;

    private int compt = 1;
    private StringBuilder buffer = new StringBuilder(8192);
    private String baseDir;
    private String outputDir;

    /**
     * Constructor
     * @param baseDir the base directory where to put the generated images
     */
    private HTMLScilabHandler(String outputDir, String baseDir) {
        this.outputDir = outputDir + File.separator + baseDir;
        this.baseDir = baseDir + "/";
    }

    public static HTMLScilabHandler getInstance(String outputDir, String baseDir) {
        if (instance == null) {
            instance = new HTMLScilabHandler(outputDir, baseDir);
        }

        return instance;
    }

    public static HTMLScilabHandler getInstance() {
        return instance;
    }

    public void resetCompt() {
        compt = 1;
    }

    /**
     * {@inheritDoc}
     */
    public String getURI() {
        return "http://www.scilab.org";
    }

    /**
     * {@inheritDoc}
     */
    public StringBuilder startExternalXML(String localName, Attributes attributes) {
        recreateTag(buffer, localName, attributes);
        if (SCILAB.equals(localName)) {
            return buffer;
        }

        return null;
    }

    /**
     * {@inheritDoc}
     */
    public String endExternalXML(String localName) {
        if (SCILAB.equals(localName)) {
            recreateTag(buffer, localName, null);
            String currentFileName = getConverter().getCurrentFileName();
            String baseName = new File(currentFileName).getName();
            int dotpos = baseName.lastIndexOf('.');
            if (dotpos != -1) {
                baseName = baseName.substring(0, dotpos);
            }
            File f = new File(outputDir, baseName + BASENAME + (compt++) + ".png");
            Map<String, String> attributes = new HashMap<String, String>();

            String ret = ImageConverter.getImageByCode(currentFileName, buffer.toString(), attributes, "image/scilab", f, baseDir + f.getName());
            buffer.setLength(0);

            return ret;
        }

        recreateTag(buffer, localName, null);

        return null;
    }
}

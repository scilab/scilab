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
import java.io.FileFilter;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;

import org.xml.sax.Attributes;

import org.scilab.modules.helptools.image.ImageConverter;
import org.scilab.modules.helptools.image.ScilabImageConverter;

/**
 * Handle the included SCILAB code
 * @author Calixte DENIZET
 */
public class HTMLScilabHandler extends ExternalXMLHandler {

    private static final String IMAGE = "image";
    private static final String BASENAME = "_";

    private static HTMLScilabHandler instance;

    private int compt = 1;
    private StringBuilder buffer = new StringBuilder(8192);
    private String baseDir;
    private String outputDir;
    private boolean isLocalized;

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

    public static void clean() {
        instance = null;
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
        if (localName.equals("image")) {
            String v = attributes.getValue("localized");
            isLocalized = "true".equalsIgnoreCase(v);
        }

        if (IMAGE.equals(localName)) {
            return buffer;
        } else {
            recreateTag(buffer, localName, attributes);
        }

        return null;
    }

    /**
     * {@inheritDoc}
     */
    public String endExternalXML(String localName) {
        if (IMAGE.equals(localName)) {
            String currentFileName = getConverter().getCurrentFileName();
            String baseName = new File(currentFileName).getName();
            int dotpos = baseName.lastIndexOf('.');
            if (dotpos != -1) {
                baseName = baseName.substring(0, dotpos);
            }
            String fileName = baseName + BASENAME + (compt++) + ".png";
            File f = new File(outputDir, fileName);
            Map<String, String> attributes = new HashMap<String, String>();

            String ret;
            File existing;
            if (isLocalized || (existing = getExistingFile(outputDir, fileName)) == null) {
                ret = ImageConverter.getImageByCode(currentFileName, buffer.toString(), attributes, "image/scilab", f, baseDir + f.getName());
            } else {
                ret = ImageConverter.getImageByFile(attributes, null, existing.getAbsolutePath(), outputDir, ".");
                ret = ScilabImageConverter.getInstance().getHTMLCodeToReturn(buffer.toString(), ret);
            }

            buffer.setLength(0);

            return ret;
        }

        recreateTag(buffer, localName, null);

        return null;
    }

    private static File getExistingFile(String outputDir, String filename) {
        try {
            final File outDir = new File(outputDir).getCanonicalFile();
            FileFilter filter = new FileFilter() {
                    public boolean accept(File f) {
                        return f.isDirectory() && !f.equals(outDir);
                    }
                };
            File[] dirs = outDir.getParentFile().listFiles(filter);
            File im = new File(filename);
            for (File dir : dirs) {
                File f = new File(dir, im.getName());
                if (f.exists() && f.canRead()) {
                    return f;
                }
            }
        } catch (IOException e) { }

        return null;
    }
}

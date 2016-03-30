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

package org.scilab.modules.helptools.external;

import java.io.File;
import java.io.FileFilter;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;

import org.xml.sax.Attributes;
import org.xml.sax.Locator;

import org.scilab.modules.helptools.HTMLDocbookTagConverter;

/**
 * Handle the included SCILAB code
 * @author Calixte DENIZET
 */
public class HTMLScilabHandler extends ExternalXMLHandler {

    private static final String IMAGE = "image";
    private static final String BASENAME = "_";

    private StringBuilder buffer = new StringBuilder(8192);
    private String baseDir;
    private String outputDir;
    private Boolean isLocalized;
    private int line;

    /**
     * Constructor
     * @param baseDir the base directory where to put the generated images
     */
    public HTMLScilabHandler(String outputDir, String baseDir) {
        this.outputDir = outputDir + File.separator + baseDir;
        this.baseDir = baseDir + "/";
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
    public StringBuilder startExternalXML(String localName, Attributes attributes, Locator locator) {
        if (localName.equals("image")) {
            isLocalized = getLocalized(null, attributes);
            line = locator.getLineNumber();
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
            String language = ((HTMLDocbookTagConverter) getConverter()).getLanguage();
            String fileName;
            if (isLocalized != null && isLocalized.booleanValue()) {
                fileName = baseName + BASENAME + language + BASENAME + (compt++) + ".png";
            } else {
                fileName = baseName + BASENAME + (compt++) + ".png";
            }

            File f = new File(outputDir, fileName);
            Map<String, String> attributes = new HashMap<String, String>();

            String ret;
            File existing;
            String baseImagePath = "";
            if (getConverter() instanceof HTMLDocbookTagConverter) {
                baseImagePath = ((HTMLDocbookTagConverter) getConverter()).getBaseImagePath();
            }
            if ((isLocalized != null && isLocalized.booleanValue()) || (existing = getExistingFile(outputDir, fileName)) == null) {
                ret = getConverter().getImageConverter().getImageByCode(currentFileName, buffer.toString(), attributes, "image/scilab", f, baseDir + f.getName(), baseImagePath, line, language, isLocalized);
            } else {
                ret = getConverter().getImageConverter().getImageByFile(attributes, null, existing.getAbsolutePath(), outputDir, ".", baseImagePath);
            }

            buffer.setLength(0);
            return ret;
        }

        recreateTag(buffer, localName, null);

        buffer.setLength(0);
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

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
import java.util.HashMap;
import java.util.Map;

import org.xml.sax.Attributes;
import org.xml.sax.Locator;

import org.scilab.modules.helptools.HTMLDocbookTagConverter;

/**
 * Handle the included SVG code
 * @author Calixte DENIZET
 */
public class HTMLSVGHandler extends ExternalXMLHandler {

    private static final String SVG = "svg";
    private static final String BASENAME = "_SVG_";

    private StringBuilder buffer = new StringBuilder(8192);
    private String baseDir;
    private String outputDir;
    private Boolean isLocalized;
    private int line;

    /**
     * Constructor
     * @param baseDir the base directory where to put the generated images
     */
    public HTMLSVGHandler(String outputDir, String baseDir) {
        this.outputDir = outputDir + File.separator + baseDir;
        this.baseDir = baseDir + "/";
    }

    /**
     * {@inheritDoc}
     */
    public String getURI() {
        return "http://www.w3.org/2000/svg";
    }

    /**
     * {@inheritDoc}
     */
    public StringBuilder startExternalXML(String localName, Attributes attributes, Locator locator) {
        if (SVG.equals(localName)) {
            isLocalized = getLocalized(getScilabURI(), attributes);
            line = locator.getLineNumber();
        }

        recreateTag(buffer, localName, attributes);
        if (SVG.equals(localName)) {
            return buffer;
        }

        return null;
    }

    /**
     * {@inheritDoc}
     */
    public String endExternalXML(String localName) {
        if (SVG.equals(localName)) {
            recreateTag(buffer, localName, null);
            File f;
            String language = ((HTMLDocbookTagConverter) getConverter()).getLanguage();
            if (isLocalized != null && isLocalized.booleanValue()) {
                f = new File(outputDir, BASENAME + getConverter().getCurrentBaseName() + "_" + language + "_" + (compt++) + ".png");
            } else {
                if ("ru_RU".equals(language) && HTMLDocbookTagConverter.containsCyrillic(buffer)) {
                    System.err.println("Warning: SVG code in " + getConverter().getCurrentFileName() + " at line " + line + " contains cyrillic character. The tag <svg> should contain the attribute scilab:localized=\"true\"");
                } else if ("ja_JP".equals(language) && HTMLDocbookTagConverter.containsCJK(buffer)) {
                    System.err.println("Warning: SVG code in " + getConverter().getCurrentFileName() + " at line " + line + " contains CJK character. The tag <svg> should contain the attribute scilab:localized=\"true\"");
                }
                f = new File(outputDir, BASENAME + getConverter().getCurrentBaseName() + "_" + (compt++) + ".png");
            }

            Map<String, String> attributes = new HashMap<String, String>();
            String baseImagePath = "";
            if (getConverter() instanceof HTMLDocbookTagConverter) {
                baseImagePath = ((HTMLDocbookTagConverter) getConverter()).getBaseImagePath();
            }

            String ret = getConverter().getImageConverter().getImageByCode(getConverter().getCurrentFileName(), buffer.toString(), attributes, "image/svg", f, baseDir + f.getName(), baseImagePath, line, language, isLocalized);
            buffer.setLength(0);

            return ret;
        }

        recreateTag(buffer, localName, null);

        return null;
    }
}

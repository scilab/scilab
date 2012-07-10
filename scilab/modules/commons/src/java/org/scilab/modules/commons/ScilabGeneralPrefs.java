/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.commons;

import java.awt.Font;

import org.w3c.dom.Document;

import org.scilab.modules.commons.xml.ScilabXMLUtilities;
import org.scilab.modules.commons.xml.XConfiguration;
import org.scilab.modules.commons.xml.XConfigurationEvent;
import org.scilab.modules.commons.xml.XConfigurationListener;
import static org.scilab.modules.commons.xml.XConfiguration.XConfAttribute;

public class ScilabGeneralPrefs implements XConfigurationListener {

    private static final String ENV_PATH = "//general/body/environment";
    private static final String FONT_PATH = "//fonts/body/fonts";
    private static final ScilabGeneralPrefs instance = new ScilabGeneralPrefs();
    private static Font desktopFont;

    private ScilabGeneralPrefs() { }

    public static ScilabGeneralPrefs getInstance() {
        return instance;
    }

    public void configurationChanged(XConfigurationEvent e) {
        boolean all = e.getModifiedPaths().contains("ALL");
        if (all || e.getModifiedPaths().contains(ENV_PATH)) {
            Document doc = XConfiguration.getXConfigurationDocument();
            GeneralEnvironment ge = XConfiguration.get(GeneralEnvironment.class, doc, ENV_PATH)[0];
            ScilabCommons.setieee(ge.code);
            ScilabCommons.setformat(ge.format, ge.width);
        }

        if (all || e.getModifiedPaths().contains(FONT_PATH)) {
            desktopFont = null;
        }
    }

    public static Font getDesktopFont() {
        if (desktopFont == null) {
            Document doc = XConfiguration.getXConfigurationDocument();
            DesktopFont df = XConfiguration.get(DesktopFont.class, doc, FONT_PATH)[0];
            desktopFont = df.font;
        }

        return desktopFont;
    }

    @XConfAttribute
    private static class DesktopFont {

        public Font font;

        private DesktopFont() { }

        @XConfAttribute(tag = "fonts", attributes = {"font-face", "font-name", "font-size", "system"})
        private void set(String fontFace, String fontName, int fontSize, boolean useSystemFont) {
            if (useSystemFont) {
                this.font = new Font("monospaced", Font.PLAIN, 13);
            } else {
                this.font = new Font(fontName, Font.PLAIN, fontSize);
                int style = Font.PLAIN;
                if (fontFace.contains("bold")) {
                    style = style | Font.BOLD;
                } else if (fontFace.contains("italic")) {
                    style = style | Font.ITALIC;
                }
                if (style != Font.PLAIN) {
                    this.font = this.font.deriveFont(style);
                }
            }
        }
    }

    @XConfAttribute
    private static class GeneralEnvironment {

        public int code;
        public String format;
        public int width;

        private GeneralEnvironment() { }

        @XConfAttribute(attributes = {"fpe", "printing-format", "width"})
        private void set(String fpe, String format, int width) {
            if (fpe.equals("Produces an error")) {
                this.code = 0;
            } else if (fpe.equals("Produces a warning")) {
                this.code = 1;
            } else if (fpe.equals("Produces Inf or NaN")) {
                this.code = 2;
            }
            if (format.equals("Scientific format")) {
                this.format = "e";
            } else {
                this.format = "v";
            }
            this.width = Math.min(Math.max(0, width), 25);
        }
    }
}

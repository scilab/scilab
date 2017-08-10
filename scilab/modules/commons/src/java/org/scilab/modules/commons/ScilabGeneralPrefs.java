/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
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

package org.scilab.modules.commons;

import java.awt.Font;
import java.lang.reflect.Method;

import org.w3c.dom.Document;

import org.scilab.modules.commons.xml.XConfiguration;
import org.scilab.modules.commons.xml.XConfigurationEvent;
import org.scilab.modules.commons.xml.XConfigurationListener;
import static org.scilab.modules.commons.xml.XConfiguration.XConfAttribute;
import org.scilab.modules.localization.WindowsDefaultLanguage;

public class ScilabGeneralPrefs implements XConfigurationListener {

    private static final String ENV_PATH = "//general/body/environment";
    private static final String FONT_PATH = "//fonts/body/fonts";
    private static final String LANG_PATH = "//general/body/languages";
    private static final ScilabGeneralPrefs instance = new ScilabGeneralPrefs();
    private static Font desktopFont;

    private ScilabGeneralPrefs() { }

    public static ScilabGeneralPrefs getInstance() {
        return instance;
    }

    public static void openPreferences(String path) {
        try {
            Class prefs = ClassLoader.getSystemClassLoader().loadClass("org.scilab.modules.preferences.ScilabPreferences");
            Method open = prefs.getDeclaredMethod("openPreferences", String.class);
            open.invoke(null, path);
        } catch (ClassNotFoundException e) {
            // Nothing displayed (always occurs in MN mode)
        } catch (Exception e) {
            System.err.println(e);
        }
    }

    public void configurationChanged(XConfigurationEvent e) {
        boolean all = e.getModifiedPaths().contains("ALL");
        if (all || e.getModifiedPaths().contains(ENV_PATH)) {
            Document doc = XConfiguration.getXConfigurationDocument();
            GeneralEnvironment ge = XConfiguration.get(GeneralEnvironment.class, doc, ENV_PATH)[0];
            ScilabCommons.setieee(ge.code);
            ScilabCommons.setformat(ge.format, ge.width);
            ScilabCommons.setRecursionLimit(ge.recursionLimit);
        }

        if (e.getModifiedPaths().contains(LANG_PATH)) {
            Document doc = XConfiguration.getXConfigurationDocument();
            Language language = XConfiguration.get(Language.class, doc, LANG_PATH)[0];
            WindowsDefaultLanguage.setdefaultlanguage(language.lang);
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
        public int recursionLimit;

        private GeneralEnvironment() { }

        @XConfAttribute(attributes = {"fpe", "printing-format", "width", "recursion-limit"})
        private void set(int fpe, String format, int width, int recursionLimit) {
            this.code = fpe;
            this.format = format;
            this.width = Math.min(Math.max(0, width), 25);
            this.recursionLimit = recursionLimit;
        }
    }

    @XConfAttribute
    private static class Language {

        public String lang;

        private Language() { }

        @XConfAttribute(attributes = {"lang"})
        private void set(String lang) {
            this.lang = lang;
        }
    }
}

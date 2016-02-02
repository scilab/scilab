/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
 * Copyright (C) 2011 - Scilab Enterprises -Calixte DENIZET
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

package org.scilab.modules.scinotes;

import java.awt.Color;
import java.awt.Font;
import java.util.ArrayList;
import java.util.List;

import javax.swing.text.ViewFactory;
import javax.swing.text.View;
import javax.swing.text.Element;
import javax.xml.xpath.XPath;
import javax.xml.xpath.XPathConstants;
import javax.xml.xpath.XPathExpressionException;
import javax.xml.xpath.XPathFactory;

import org.w3c.dom.Document;
import org.w3c.dom.NodeList;

import static org.scilab.modules.commons.xml.XConfiguration.XConfAttribute;

import org.scilab.modules.commons.xml.ScilabXPathFactory;
import org.scilab.modules.commons.xml.XConfiguration;
import org.scilab.modules.gui.utils.ScilabFontUtils;

/**
 * The class ScilabContext provides a context to render a Scilab's document.
 * @author Calixte DENIZET
 */
public class ScilabContext implements ViewFactory {

    public static final String COLORSPATH = "//colors/body/syntax-highlighting-colors/item";
    public static final String SYSTEMFONTPATH = "//fonts/body/fonts";
    public static final String FONTPATH = "//fonts/body/fonts/item";
    public static final String XCONFFONTPATH = "//fonts/body/fonts/item[@xconf-uid=\"scinotes-font\"]";

    /**
     * Contains the colors of the different tokens
     */
    public Color[] tokenColors;

    /**
     * Contains the fonts of the different tokens
     */
    public Font[] tokenFonts;

    /**
     * Contains the attrib (underline or stroke) of the different tokens
     */
    public int[] tokenAttrib;

    private Font baseFont;
    private boolean isDesktop;
    private boolean compatible;
    private FontInfo[] scinotesInfos;
    private FontInfo[] desktopInfos;

    private View view;
    private boolean plain;
    private List<Integer> typeToDefault = new ArrayList<Integer>();

    /**
     * The constructor
     */
    public ScilabContext() {
        super();
        genAll();
    }

    /**
     * Constructor
     * @param plain boolean for a plain view
     */
    public ScilabContext(boolean plain) {
        this();
        this.plain = plain;
    }


    public static void saveFont(Font font) {
        XPathFactory xpathFactory = ScilabXPathFactory.newInstance();
        XPath xp = xpathFactory.newXPath();
        NodeList nodes;
        Document doc = XConfiguration.getXConfigurationDocument();

        try {
            nodes = (NodeList) xp.compile("//fonts/body/fonts/item[@xconf-uid='scinotes-font']").evaluate(doc, XPathConstants.NODESET);
        } catch (XPathExpressionException e) {
            System.err.println(e);
            return;
        }

        org.w3c.dom.Element e = (org.w3c.dom.Element) nodes.item(0);
        e.setAttribute("desktop", "false");
        e.setAttribute("font-name", font.getName());
        e.setAttribute("font-size", Integer.toString(font.getSize()));

        XConfiguration.save();
    }

    /**
     * @return the font used to render
     */
    public Font getBaseFont() {
        return baseFont;
    }

    public void changeBaseFontSize(int n) {
        float size = baseFont.getSize2D() + n;
        baseFont = baseFont.deriveFont(size);
        for (int i = 0; i < tokenFonts.length; i++) {
            tokenFonts[i] = tokenFonts[i].deriveFont(size);
        }
    }

    public void configurationChanged(SciNotesConfiguration.Conf conf) {
        boolean modified = false;
        FontInfo[] scinotes = null;
        FontInfo[] desktop = null;
        Document doc = null;
        if (conf.colors) {
            genAll();
            modified = true;
        }

        if (!modified && conf.font) {
            doc = XConfiguration.getXConfigurationDocument();
            scinotes = XConfiguration.get(FontInfo.class, doc, FONTPATH + "[@name='Scinotes']");
            if (!scinotes[0].equals(scinotesInfos)) {
                genAll();
                modified = true;
            }
        }

        if (!modified && conf.systemfont) {
            doc = XConfiguration.getXConfigurationDocument();
            desktop = XConfiguration.get(FontInfo.class, doc, SYSTEMFONTPATH);
            genAll();
        }
    }

    public void genAll() {
        tokenAttrib = new int[ScilabLexerConstants.NUMBEROFTOKENS];
        tokenColors = new Color[ScilabLexerConstants.NUMBEROFTOKENS];
        tokenFonts = new Font[ScilabLexerConstants.NUMBEROFTOKENS];

        Document doc = XConfiguration.getXConfigurationDocument();
        desktopInfos = XConfiguration.get(FontInfo.class, doc, SYSTEMFONTPATH);
        scinotesInfos = XConfiguration.get(FontInfo.class, doc, FONTPATH + "[@name='Scinotes']");
        Font font;
        if (scinotesInfos[0].isDesktop()) {
            font = desktopInfos[0].getFont();
            isDesktop = true;
        } else {
            font = scinotesInfos[0].getFont();
            isDesktop = false;
        }

        if (font != baseFont) {
            compatible = ScilabFontUtils.isAllStylesSameWidths(font);
            baseFont = font;
        }

        FontDecoration[] decorations = XConfiguration.get(FontDecoration.class, doc, COLORSPATH);
        doc = null;

        for (FontDecoration deco : decorations) {
            tokenAttrib[deco.getType()] = deco.getValue();
            tokenColors[deco.getType()] = deco.getColor();
            tokenFonts[deco.getType()] = compatible ? baseFont.deriveFont(deco.getFontFace()) : baseFont;
        }

        // Default color and special case
        typeToDefault.clear();
        for (int i = 0; i < tokenColors.length; i++) {
            if (tokenColors[i] == null) {
                tokenColors[i] = tokenColors[0];
                typeToDefault.add(i);
            }
        }

        /* Special case : Scilab's developers in comments */
        tokenColors[ScilabLexerConstants.AUTHORS] = tokenColors[ScilabLexerConstants.COMMENT];
        tokenColors[ScilabLexerConstants.OSKEYWORD] = tokenColors[ScilabLexerConstants.SKEYWORD];
        tokenColors[ScilabLexerConstants.ELSEIF] = tokenColors[ScilabLexerConstants.SKEYWORD];

        // Default attributes
        for (Integer i : typeToDefault) {
            tokenAttrib[i] = tokenAttrib[0];
        }
        tokenAttrib[ScilabLexerConstants.OSKEYWORD] = tokenAttrib[ScilabLexerConstants.SKEYWORD];
        tokenAttrib[ScilabLexerConstants.ELSEIF] = tokenAttrib[ScilabLexerConstants.SKEYWORD];

        // Default fonts
        for (int i = 0; i < tokenFonts.length; i++) {
            if (tokenFonts[i] == null) {
                tokenFonts[i] = tokenFonts[0];
            }
        }

        /* Special case : Scilab's developers in comments */
        Font c = tokenFonts[ScilabLexerConstants.COMMENT];
        int style = c.getStyle();
        if (compatible) {
            if (c.isBold()) {
                tokenFonts[ScilabLexerConstants.AUTHORS] = c.deriveFont(style ^ Font.BOLD);
            } else {
                tokenFonts[ScilabLexerConstants.AUTHORS] = c.deriveFont(style | Font.BOLD);
            }
        }

        tokenFonts[ScilabLexerConstants.OSKEYWORD] = tokenFonts[ScilabLexerConstants.SKEYWORD];
        tokenFonts[ScilabLexerConstants.ELSEIF] = tokenFonts[ScilabLexerConstants.SKEYWORD];
    }

    /**
     * Generate an attribute for a type of keyword
     * @param keyword the name can be found in scinotesConfiguration.xml
     * @param type the type to use
     */
    public void genAttribute(String keyword, int type) {
        tokenAttrib[ScilabLexerConstants.TOKENS.get(keyword)] = type;
        tokenAttrib[ScilabLexerConstants.OSKEYWORD] = tokenAttrib[ScilabLexerConstants.SKEYWORD];
        tokenAttrib[ScilabLexerConstants.ELSEIF] = tokenAttrib[ScilabLexerConstants.SKEYWORD];
        if (ScilabLexerConstants.TOKENS.get(keyword) == ScilabLexerConstants.DEFAULT) {
            for (Integer i : typeToDefault) {
                tokenAttrib[i] = tokenAttrib[0];
            }
        }
    }

    /**
     * Generate a color for a type of keyword
     * @param name the name can be found in scinotesConfiguration.xml
     * @param color the color to use
     */
    public void genColors(String name, Color color) {
        tokenColors[ScilabLexerConstants.TOKENS.get(name)] = color;
        tokenColors[ScilabLexerConstants.AUTHORS] = tokenColors[ScilabLexerConstants.COMMENT];
        tokenColors[ScilabLexerConstants.OSKEYWORD] = tokenColors[ScilabLexerConstants.SKEYWORD];
        tokenColors[ScilabLexerConstants.ELSEIF] = tokenColors[ScilabLexerConstants.SKEYWORD];

        if (ScilabLexerConstants.TOKENS.get(name) == ScilabLexerConstants.DEFAULT) {
            for (Integer i : typeToDefault) {
                tokenColors[i] = tokenColors[0];
            }
        }
    }

    /**
     * Generate a font for a type of keyword
     * @param name the name can be found in scinotesConfiguration.xml
     * @param type the type to use
     */
    public void genFont(String name, int type) {
        Font font = tokenFonts[ScilabLexerConstants.TOKENS.get(name)];
        int style = font.getStyle();
        switch (type) {
            case -2 :
                font = font.deriveFont(style & ~Font.ITALIC);
                break;
            case -1 :
                font = font.deriveFont(style & ~Font.BOLD);
                break;
            case 1 :
                font = font.deriveFont(style | Font.BOLD);
                break;
            case 2 :
                font = font.deriveFont(style | Font.ITALIC);
                break;
            default :
        }

        tokenFonts[ScilabLexerConstants.TOKENS.get(name)] = font;
        tokenFonts[ScilabLexerConstants.OSKEYWORD] = tokenFonts[ScilabLexerConstants.SKEYWORD];
        tokenFonts[ScilabLexerConstants.ELSEIF] = tokenFonts[ScilabLexerConstants.SKEYWORD];
        if (ScilabLexerConstants.TOKENS.get(name) == ScilabLexerConstants.DEFAULT) {
            for (Integer i : typeToDefault) {
                tokenFonts[i] = tokenFonts[0];
            }
        }
    }

    /**
     * @return the view to use to render the document
     */
    public View getCurrentView() {
        return view;
    }

    /**
     * Create a view with a given element
     * @param elem the Element to view
     * @return the view associated with the element
     */
    public View create(Element elem) {
        if (plain) {
            view = new ScilabPlainView(elem, this);
            ((ScilabPlainView) view).setDefaultTabRepresentation();
        } else {
            view = new ScilabView(elem, this);
            ((ScilabView) view).setDefaultTabRepresentation();
        }
        return view;
    }

    /**
     * Inner class to retrieve configuration elements from configuration file
     */
    @XConfAttribute
    private static class FontInfo {

        String fontname;
        int fontsize;
        boolean desktop;

        public FontInfo() { }

        @XConfAttribute(attributes = {"font-name", "font-size", "desktop"})
        public void set(String fontname, int fontsize, boolean desktop) {
            this.fontname = fontname;
            this.fontsize = fontsize;
            this.desktop = desktop;
        }

        public boolean isDesktop() {
            return desktop;
        }

        public Font getFont() {
            return new Font(fontname, Font.PLAIN, fontsize);
        }

        public boolean equals(Object o) {
            if (o instanceof FontInfo) {
                FontInfo f = (FontInfo) o;
                return fontname.equals(f.fontname) && fontsize == f.fontsize && desktop == f.desktop;
            }

            return false;
        }
    }

    /**
     * Inner class to retrieve configuration elements from configuration file
     */
    @XConfAttribute
    private static class FontDecoration {

        private boolean underline;
        private boolean strikeThrough;
        private boolean bold;
        private boolean italic;
        private Color color;
        private int type;

        public FontDecoration() { }

        @XConfAttribute(attributes = {"underline", "strike-through", "name", "italic", "color", "bold"})
        public void set(boolean underline, boolean strikeThrough, String name, boolean italic, Color color, boolean bold) {
            this.underline = underline;
            this.strikeThrough = strikeThrough;
            this.type = ScilabLexerConstants.TOKENS.get(name);
            this.color = color;
            this.italic = italic;
            this.bold = bold;
        }

        public int getFontFace() {
            int fontface = Font.PLAIN;
            if (bold) {
                fontface = fontface | Font.BOLD;
            }

            if (italic) {
                fontface = fontface | Font.ITALIC;
            }

            return fontface;
        }

        public Color getColor() {
            return color;
        }

        public int getValue() {
            int value = 0;
            if (underline) {
                value += 1;
            }
            if (strikeThrough) {
                value += 2;
            }

            return value;
        }

        public int getType() {
            return type;
        }

        public boolean equals(Object o) {
            if (o instanceof FontDecoration) {
                FontDecoration f = (FontDecoration) o;
                return underline == f.underline && strikeThrough == f.strikeThrough && bold == f.bold && italic == f.italic && color.equals(f.color) && type == f.type;
            }

            return false;
        }
    }
}

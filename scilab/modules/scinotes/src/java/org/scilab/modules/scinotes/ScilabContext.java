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

package org.scilab.modules.scinotes;

import java.util.Map;
import java.util.List;
import java.util.ArrayList;
import java.util.Iterator;
import java.awt.Color;
import java.awt.Font;

import javax.swing.text.ViewFactory;
import javax.swing.text.View;
import javax.swing.text.Element;

import org.w3c.dom.Document;


import org.scilab.modules.commons.xml.XConfiguration;
import org.scilab.modules.gui.utils.ScilabFontUtils;

/**
 * The class ScilabContext provides a context to render a Scilab's document.
 * @author Calixte DENIZET
 */
public class ScilabContext implements ViewFactory {

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

    public void genAll() {
        tokenAttrib = new int[ScilabLexerConstants.NUMBEROFTOKENS];
        tokenColors = new Color[ScilabLexerConstants.NUMBEROFTOKENS];
        tokenFonts = new Font[ScilabLexerConstants.NUMBEROFTOKENS];

        Document doc = XConfiguration.getXConfigurationDocument();
        FontInfo[] desktop = XConfiguration.get(FontInfo.class, doc, "//fonts/body/fonts");
        FontInfo[] scinotes = XConfiguration.get(FontInfo.class, doc, "//fonts/body/fonts/item[@name='Scinotes']");
        Font font;
        if (scinotes[0].isDesktop()) {
            font = desktop[0].getFont();
        } else {
            font = scinotes[0].getFont();
        }
        boolean compatible = ScilabFontUtils.isAllStylesSameWidths(font);

        FontDecoration[] decorations = XConfiguration.get(FontDecoration.class, doc, "//colors/body/syntax-highlighting-colors/item");
        doc = null;

        for (FontDecoration deco : decorations) {
            tokenAttrib[deco.getType()] = deco.getValue();
            tokenColors[deco.getType()] = deco.getColor();
            tokenFonts[deco.getType()] = compatible ? font.deriveFont(deco.getFontFace()) : font;
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
    private static class FontInfo {

        private String fontname;
        private int fontsize;
        private boolean desktop;

        public FontInfo() { }

        public void setFontName(String fontname) {
            this.fontname = fontname;
        }

        public void setFontSize(int fontsize) {
            this.fontsize = fontsize;
        }

        public void setDesktop(boolean desktop) {
            this.desktop = desktop;
        }

        public boolean isDesktop() {
            return desktop;
        }

        public Font getFont() {
            return new Font(fontname, Font.PLAIN, fontsize);
        }
    }

    /**
     * Inner class to retrieve configuration elements from configuration file
     */
    private static class FontDecoration {

        private boolean underline;
        private boolean strikeThrough;
        private boolean bold;
        private boolean italic;
        private Color color;
        private int type;

        public FontDecoration() { }

        public void setUnderline(boolean underline) {
            this.underline = underline;
        }

        public void setStrikeThrough(boolean strikeThrough) {
            this.strikeThrough = strikeThrough;
        }

        public void setName(String name) {
            type = ScilabLexerConstants.TOKENS.get(name);
        }

        public void setColor(Color color) {
            this.color = color;
        }

        public void setBold(boolean bold) {
            this.bold = bold;
        }

        public void setItalic(boolean italic) {
            this.italic = italic;
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
    }
}

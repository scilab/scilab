/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2008 - INRIA - Allan CORNET
 * desc : Singleton class used to set font of text objects
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


package org.scilab.modules.renderer.utils.textRendering;

import java.io.FileInputStream;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.awt.Font;
import java.awt.FontFormatException;
import java.awt.GraphicsEnvironment;
import java.util.ArrayList;

/**
 * Singleton class used to set font of text objects.
 * @author Jean-Baptiste Silvy
 */
public class FontManager {

    private static final String SCIDIR = System.getenv("SCI");
    private static final float[] INTERVALS = {3.0f, 4.0f, 5.0f};
    private static final float[] FIRST_POLY = {2.0f, 8.0f};
    private static final float[] SECOND_POLY = {4.0f, 2.0f};
    private static final float[] THIRD_POLY = {6.0f, -6.0f};
    private static final float[] FOURTH_POLY = {10.0f, -26.0f};

    private static final float SIX = 6.0f;

    private static final float[] INV_INTERVALS = {14.0f, 18.0f, 24.0f};
    private static final float[] INV_FIRST_POLY = {0.5f, -4.0f};
    private static final float[] INV_SECOND_POLY = {0.25f, -0.5f};
    private static final float[] INV_THIRD_POLY = {1.0f / SIX, 1.0f};
    private static final float[] INV_FOURTH_POLY = {0.1f, 2.6f};


    // logical awt fonts
    // JRE Java guaranteed: "Dialog", "DialogInput",
    // "Monospaced","Serif", "SansSerif", "Symbol", "Lucida"
    // Scilab 4.x fonts --> fonts with JRE 1.5 or more
    // Times            --> Serif
    // Helvetica        --> SansSerif
    // Courier          --> Monospaced
    // Symbol           --> Scilab Symbols font

    private static final String MONOSPACED = "Monospaced";
    private static final String SANSSERIF = "SansSerif";
    private static final String SERIF = "Serif";
    private static final String SCILABSYMBOLSFONT = "ScilabSymbols";
    private static final String SYMBOLSFONTPATH = "/thirdparty/fonts/scilabsymbols.ttf";
    private static final Font DEFAULT_FONT = new Font("Default", Font.PLAIN, 1);

    /** Singleton instance. */
    private static final FontManager sciFontManager = new FontManager();

    /**
     * A list of fonts.
     */
    private static class FontList extends ArrayList<Font> {

        private static final long serialVersionUID = 1L;

        /**
         * Default constructor.
         */
        public FontList() {
            super();
        }
    }

    private static FontList sciFonts;

    /**
     * Default constructor.
     * Should not be called.
     */
    private FontManager() {
        initializeFontManager();
    }

    /**
     * Get the font manager instance
     * @return the only instance of fontmanager.
     */
    public static FontManager getSciFontManager() {
        return sciFontManager;
    }

    /**
     * Convert sciab font size to awt font size.
     * We use a degree 2 polygon to compute this.
     * We use canonical form of the polygon so the inverse function is easy to compute
     * Tthe equivalence list is (0 => 8, 1 => 10, 2 => 12, 3 => 14, 4 => 18, 5 => 24).
     * @param sciSize scilab size
     * @return awt size
     */
    public static float scilabSizeToAwtSize(double sciSize) {
        float sciSizef = (float) sciSize;
        // f(x) = | 2x + 8 if x < 3
        //        | 4x + 2 if 3 < x < 4
        //        | 6x - 6 if 4 < x
        if (sciSizef < INTERVALS[0]) {
            return FIRST_POLY[0] * sciSizef + FIRST_POLY[1];
        } else if (sciSizef < INTERVALS[1]) {
            return SECOND_POLY[0] * sciSizef + SECOND_POLY[1];
        } else if (sciSizef < INTERVALS[2]) {
            return THIRD_POLY[0] * sciSizef + THIRD_POLY[1];
        } else {
            return FOURTH_POLY[0] * sciSizef + FOURTH_POLY[1];
        }
    }

    /**
     * Inverse of scilabSizeToAwtSize function
     * @param size size of a AWT font
     * @return corresponding size in Scilab
     */
    public static double awtSizeToScilabSize(float size) {
        // sqrt(|x - c| / a) - b
        if (size < INV_INTERVALS[0]) {
            return INV_FIRST_POLY[0] * size + INV_FIRST_POLY[1];
        } else if (size < INV_INTERVALS[1]) {
            return INV_SECOND_POLY[0] * size + INV_SECOND_POLY[1];
        } else if (size < INV_INTERVALS[2]) {
            return INV_THIRD_POLY[0] * size + INV_THIRD_POLY[1];
        } else {
            return INV_FOURTH_POLY[0] * size + INV_FOURTH_POLY[1];
        }
    }

    /**
     * Get a font from its index in Scilab.
     * @param fontIndex scilab index of the font.
     * @return font corresponding to the index with size 1.
     */
    public final Font getFontFromIndex(int fontIndex) {
        if (fontIndex >= 0 && fontIndex < sciFonts.size()) {
            return sciFonts.get(fontIndex);
        } else if (fontIndex < 0) {
            return sciFonts.get(0);
        } else {
            return sciFonts.get(sciFonts.size() - 1);
        }
    }

    /**
     * Get a font from its index in Scilab.
     * @param fontIndex scilab index of the font.
     * @param fontSize size of the font (scilab size).
     * @return font corresponding to the index.
     */
    public final Font getFontFromIndex(int fontIndex, double fontSize) {
        Font res = getFontFromIndex(fontIndex);
        return res.deriveFont(scilabSizeToAwtSize(fontSize));
    }
    /**
     * Add a new font in the font list.
     * @param newFont font to add to the list
     * @return index of added font.
     */
    public final int addFont(Font newFont) {
        sciFonts.add(newFont);
        return sciFonts.size() - 1;
    }

    /**
     * Replace a font in the font list by a new one.
     * @param index index of the font to replace
     * @param newFont font to add in the font list.
     * @return index of the added font or -1 if an error occurred.
     */
    public final int changeFont(int index, Font newFont) {
        int nbFonts = sciFonts.size();
        if (index > nbFonts) {
            // we need to add fonts until index
            for (int i = nbFonts; i < index; i++) {
                addFont(DEFAULT_FONT);
            }
            return addFont(newFont);
        } else if (index == nbFonts) {
            // add a new Font
            return addFont(newFont);
        } else {
            sciFonts.set(index, newFont);
            return index;
        }

    }

    /**
     * Create a new font knowing its name and size.
     * @param fontName Name of the font.
     * @return new font.
     */
    protected final Font createFont(String fontName) {
        // size must be applied after.
        // by default, we use PLAIN font.
        return new Font(fontName, Font.PLAIN, 1);
    }

    /**
     * Load scilab symbols font
     * @return font.
     */
    protected final Font loadScilabSymbolFont() {
        return loadFont(SCIDIR + SYMBOLSFONTPATH);
    }

    /**
     * load a font from filename
     * @param fontFileName filename of the font.
     * @return font.
     */
    protected final Font loadFont(String fontFileName) {
        Font loadedFont;
        FileInputStream in;
        File f = new File(fontFileName);
        try {
            in = new FileInputStream(f);
        } catch (FileNotFoundException e) {
            loadedFont = DEFAULT_FONT;
            return loadedFont;
        }

        try {
            loadedFont = Font.createFont(Font.TRUETYPE_FONT, in);
        } catch (IOException e) {
            loadedFont = DEFAULT_FONT;
        } catch (FontFormatException ffe) {
            loadedFont = DEFAULT_FONT;
        }

        return loadedFont;
    }

    /**
     * Create a new font knowing its name and size.
     * @param fontName Name of the font.
     * @param isBold whether the font is bold or not.
     * @param isItalic whether the font is in italic or not.
     * @return new font.
     */
    protected final Font createFont(String fontName, boolean isBold, boolean isItalic) {
        int style;
        if (isBold && isItalic) {
            style = Font.BOLD | Font.ITALIC;
        } else if (isBold) {
            style = Font.BOLD;
        } else if (isItalic) {
            style = Font.ITALIC;
        } else {
            style = Font.PLAIN;
        }
        return new Font(fontName, style, 1);
    }

    /**
     * Add a new font from its font name.
     * @param fontName name of the font to add.
     * @return index of the added font.
     */
    public final int addFont(String fontName) {
        return addFont(createFont(fontName));
    }

    /**
     * Add a new font from its font name.
     * @param fontName name of the font to add.
     * @param isBold whether the font is bold or not.
     * @param isItalic whether the font is in italic or not.
     * @return index of the added font.
     */
    public final int addFont(String fontName, boolean isBold, boolean isItalic) {
        return addFont(createFont(fontName, isBold, isItalic));
    }

    /**
     * Add a new font from its filename.
     * @param fontFilename  filename of the font to add.
     * @return index of the added font.
     */
    public final int addFontFromFilename(String fontFilename) {
        return addFont(loadFont(fontFilename));
    }

    /**
     * Replace a font in the font list by a new one.
     * @param index index of the font to replace
     * @param fontName Name of the font.
     * @return index of the added font or -1 if an error occurred.
     */

    public final int changeFont(int index, String fontName) {
        return changeFont(index, createFont(fontName));
    }

    /**
    * Replace a font in the font list by a new one loaded from a file.
    * @param index index of the font to replace
    * @param fontFilename  filename of the font
    * @return index of the added font or -1 if an error occurred.
    */
    public final int changeFontFromFilename(int index, String fontFilename) {
        return changeFont(index, loadFont(fontFilename));
    }

    /**
     * Replace a font in the font list by a new one.
     * @param index index of the font to replace
     * @param fontName Name of the font.
     * @param isBold whether the font is bold or not.
     * @param isItalic whether the font is in italic or not.
     * @return index of the added font or -1 if an error occurred.
     */
    public final int changeFont(int index, String fontName, boolean isBold, boolean isItalic) {
        return changeFont(index, createFont(fontName, isBold, isItalic));
    }

    /**
     * Get the list of all fonts available.
     * @return Names of the available fonts.
     */
    public final String[] getAvailableFontsName() {
        return GraphicsEnvironment.getLocalGraphicsEnvironment().getAvailableFontFamilyNames();
    }

    /**
     * Get the size of list of all fonts available.
     * @return size.
     */
    public final int getSizeAvailableFontsName() {
        return GraphicsEnvironment.getLocalGraphicsEnvironment().getAvailableFontFamilyNames().length;
    }

    /**
     * Check if fontname is available
     * @param fontname Name of the font.
     * @return true or false.
     */
    public final boolean isAvailableFontName(String fontname) {
        int i = getSizeAvailableFontsName();
        String[] availableFontsName = getAvailableFontsName();
        for (i = 0; i < availableFontsName.length; i++) {
            if (fontname.compareTo(availableFontsName[i]) == 0) {
                return true;
            }
        }
        return false;
    }

    /**
     * returns fontnames installed (used) by scilab
     * @return fonts name
     */
    public final String[] getInstalledFontsName() {
        int nbFonts = sciFonts.size();
        String[] fontnames = new String[nbFonts];

        for (int i = 0; i < nbFonts; i++) {
            Font res = getFontFromIndex(i);
            fontnames[i] = res.getFamily();
            if (res.isBold()) {
                fontnames[i] += " Bold";
            }
            if (res.isItalic()) {
                fontnames[i] += " Italic";
            }
        }

        return fontnames;
    }

    /**
     * Returns number of fonts installed.
     * @return number of fonts installed.
     */
    public final int getSizeInstalledFontsName() {
        return sciFonts.size();
    }

    /**
     * initializeFontManager
     */
    public final void initializeFontManager() {
        sciFonts = new FontList();
        // set default font
        /* Fonts order in Scilab 4.x for compatibility */
        /* Courrier --> Monospaced
          Symbol --> Symbol
          Times --> Serif
          Times Italic --> Serif Italic
          Times Bold --> Serif Bold
          Times Bold Italic --> Serif Bold Italic
          Helvetica --> SansSerif
          Helvetica Italic --> SansSerif Italic
          Helvetica Bold --> SansSerif Bold
          Helveticas Bold Italic --> SansSerif bold Italic
        */

        sciFonts.add(createFont(MONOSPACED));             /* scilab font_style 0 */
        /* Symbols font */                               /* scilab font_style 1 */
        /* on scilab 4.x a --> alpha (symbol) */
        /* with java , symbols are not ascii codes , but unicodes */
        /* if font exists on system, we use else we try to load scilab symbols font (truetype) */
        if (isAvailableFontName(SCILABSYMBOLSFONT)) {
            sciFonts.add(createFont(SCILABSYMBOLSFONT));
        } else {
            sciFonts.add(loadScilabSymbolFont());
        }
        sciFonts.add(createFont(SERIF));                  /* scilab font_style 2 */
        sciFonts.add(createFont(SERIF, false, true));     /* scilab font_style 3 */
        sciFonts.add(createFont(SERIF, true, false));     /* scilab font_style 4 */
        sciFonts.add(createFont(SERIF, true, true));      /* scilab font_style 5 */
        sciFonts.add(createFont(SANSSERIF));              /* scilab font_style 6 */
        sciFonts.add(createFont(SANSSERIF, false, true)); /* scilab font_style 7 */
        sciFonts.add(createFont(SANSSERIF, true, false)); /* scilab font_style 8 */
        sciFonts.add(createFont(SANSSERIF, true, true));  /* scilab font_style 9 */
        sciFonts.add(createFont(SANSSERIF, true, true));  /* scilab font_style 10 */
    }
}

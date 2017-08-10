/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) DIGITEO - 2010-2010 - Clement DAVID <clement.david@scilab.org>
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

package org.scilab.modules.graph.utils;

import java.awt.Color;

import com.mxgraph.util.mxConstants;

/**
 * Font list from Scilab xlfont().
 */
public enum Font {
    /** xlfont(0) */
    COURIER(java.awt.Font.MONOSPACED),
    /** xlfont(1) */
    SYMBOL(java.awt.Font.DIALOG),
    /** xlfont(2) */
    SERIF(java.awt.Font.SERIF),
    /** xlfont(3) */
    SERIF_ITALIC(java.awt.Font.SERIF, mxConstants.FONT_ITALIC),
    /** xlfont(4) */
    SERIF_BOLD(java.awt.Font.SERIF, mxConstants.FONT_BOLD),
    /** xlfont(5) */
    SERIF_BOLD_ITALIC(java.awt.Font.SERIF, mxConstants.FONT_BOLD
    + mxConstants.FONT_ITALIC),
    /** xlfont(6) */
    SANS_SERIF(java.awt.Font.SANS_SERIF),
    /** xlfont(7) */
    SANS_SERIF_ITALIC(java.awt.Font.SANS_SERIF, mxConstants.FONT_ITALIC),
    /** xlfont(8) */
    SANS_SERIF_BOLD(java.awt.Font.SANS_SERIF, mxConstants.FONT_BOLD),
    /** xlfont(9) */
    SANS_SERIF_BOLD_ITALIC(java.awt.Font.SANS_SERIF, mxConstants.FONT_BOLD
    + mxConstants.FONT_ITALIC);

    private static final Color[] COLORMAP = new Color[] {
        Color.BLACK,
        Color.BLUE, Color.GREEN, Color.CYAN, Color.RED, Color.MAGENTA,
        Color.YELLOW, Color.WHITE, new Color(0, 0, 144),
        new Color(0, 0, 176), new Color(0, 0, 208),
        new Color(135, 206, 255), new Color(0, 144, 0),
        new Color(0, 176, 0), new Color(0, 208, 0), new Color(0, 144, 0),
        new Color(0, 144, 144), new Color(0, 176, 176),
        new Color(0, 208, 208), new Color(144, 0, 0), new Color(176, 0, 0),
        new Color(208, 0, 0), new Color(144, 0, 144),
        new Color(176, 0, 176), new Color(208, 0, 208),
        new Color(128, 48, 0), new Color(160, 64, 0),
        new Color(192, 96, 0), new Color(255, 128, 128),
        new Color(255, 160, 160), new Color(255, 192, 192),
        new Color(255, 224, 224), new Color(255, 215, 0)
    };

    private final String name;
    private final int modifiers;

    /**
     * Default constructor.
     *
     * @param name
     *            the name of the font
     */
    private Font(String name) {
        this(name, 0);
    }

    /**
     * Cstr
     *
     * @param name
     *            the name of the font
     * @param modifiers
     *            the modifiers value
     */
    private Font(String name, int modifiers) {
        this.name = name;
        this.modifiers = modifiers;
    }

    /*
     * Static methods
     */

    /**
     * Return the Font for a given Scilab index
     *
     * @param index
     *            the index
     * @return the associated font
     */
    public static Font getFont(int index) {
        return Font.values()[index % Font.values().length];
    }

    /**
     * Map a Scilab size to a Java font size.
     *
     * @param scilabSize
     *            the size of the font (using getfont)
     * @return the corresponding java size
     */
    // CSOFF: MagicNumber
    public static int getSize(int scilabSize) {
        final int ret;
        if (scilabSize <= 0) {
            ret = 0;
        } else if (scilabSize < 1) {
            ret = (13 * scilabSize);
        } else if (scilabSize > 4) {
            ret = (7 * (scilabSize - 4) + 17);
        } else {
            ret = (int) (13 + 1.2 * (scilabSize - 1));
        }
        return ret;
    }

    // CSON: MagicNumber

    /**
     * Map a Scilab color to a Java color.
     *
     * @param scilabColor
     *            the color of the font (using getcolor)
     * @return the corresponding java color
     */
    // CSOFF: MagicNumber
    public static Color getColor(int scilabColor) {
        return COLORMAP[scilabColor % COLORMAP.length];
    }

    // CSON: MagicNumber

    /**
     * Map a Scilab size to a Java font size.
     *
     * @param scilabSize
     *            the size of the font (using getfont)
     * @return the corresponding java size
     */
    public static int getSize(String scilabSize) {
        return getSize(Integer.parseInt(scilabSize));
    }

    /**
     * Map a Scilab color to a Java color.
     *
     * @param scilabColor
     *            the color of the font (using getcolor)
     * @return the corresponding java color
     */
    public static Color getColor(String scilabColor) {
        return getColor(Integer.parseInt(scilabColor));
    }

    /*
     * Accessors
     */

    /**
     * @return the name of the font
     */
    public String getName() {
        return name;
    }

    /**
     * @return the modifiers
     */
    public int getModifiers() {
        return modifiers;
    }
}

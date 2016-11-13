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

package org.scilab.modules.gui.utils;

import java.awt.image.BufferedImage;
import java.awt.Font;
import java.awt.FontMetrics;
import java.awt.GraphicsEnvironment;
import java.awt.Graphics2D;

import java.util.ArrayList;
import java.util.List;

/**
 * Class to determinate if a font is monospaced or compatible
 * with SciNotes.
 * Take care: isAllStylesSameWidths can depend of the size of the font !
 * @author Calixte DENIZET
 */
public final class ScilabFontUtils {

    private static Graphics2D g2d;
    private static int defaultFontSize = 14;

    static {
        BufferedImage image = new BufferedImage(1, 1, BufferedImage.TYPE_INT_ARGB);
        g2d = image.createGraphics();
    }

    /**
     * Set the default font size to test.
     * @param size the size
     */
    public static void setDefaultFontSize(int size) {
        defaultFontSize = size;
    }

    /**
     * @return String[0] is the monospaced fonts and String[1] the non-monospaced
     */
    public static String[][] getMonospacedFontsFamillyName() {
        String[] fontFamilly = GraphicsEnvironment.getLocalGraphicsEnvironment().getAvailableFontFamilyNames() ;
        List<String> mono = new ArrayList();
        List<String> notMono = new ArrayList();
        for (int i = 0; i < fontFamilly.length; i++) {
            if (isMonospaced(fontFamilly[i])) {
                mono.add(fontFamilly[i]);
            } else {
                notMono.add(fontFamilly[i]);
            }
        }

        return new String[][] {mono.toArray(new String[0]), notMono.toArray(new String[0])};
    }

    /**
     * @return String[0] is the SciNotes fonts and String[1] the non-SciNotes
     */
    public static String[][] getAllStylesSameWidthsFontsFamillyName() {
        String[] fontFamilly = GraphicsEnvironment.getLocalGraphicsEnvironment().getAvailableFontFamilyNames() ;
        List<String> all = new ArrayList();
        List<String> notAll = new ArrayList();
        for (int i = 0; i < fontFamilly.length; i++) {
            if (isAllStylesSameWidths(fontFamilly[i])) {
                all.add(fontFamilly[i]);
            } else {
                notAll.add(fontFamilly[i]);
            }
        }

        return new String[][] {all.toArray(new String[0]), notAll.toArray(new String[0])};
    }

    /**
     * @param fontFamilly the font familly to test
     * @return true if each glyph has the same width in different styles
     */
    public static boolean isAllStylesSameWidths(String fontFamilly) {
        return isAllStylesSameWidths(new Font(fontFamilly, Font.PLAIN, defaultFontSize));
    }

    /**
     * @param font the font to test
     * @return true if each glyph has the same width in different styles
     */
    public static boolean isAllStylesSameWidths(Font font) {
        int[] style = new int[] {Font.PLAIN, Font.BOLD, Font.ITALIC, Font.BOLD | Font.ITALIC};
        FontMetrics fm = g2d.getFontMetrics(font.deriveFont(style[0]));
        int[] widths = fm.getWidths();

        for (int i = 1; i < style.length; i++) {
            fm = g2d.getFontMetrics(font.deriveFont(style[i]));
            int[] arr = fm.getWidths();
            // The range 33--126 corresponds to the usual characters in ASCII
            for (int j = 33; j < 126; j++) {
                if (arr[j] != widths[j]) {
                    return false;
                }
            }
        }

        return true;
    }

    /**
     * @param fontName the font name to test
     * @return true if the font is monospaced
     */
    public static boolean isMonospaced(String fontName) {
        return isMonospaced(new Font(fontName, Font.PLAIN, defaultFontSize));
    }

    /**
     * @param font the font to test
     * @return true if the font is monospaced
     */
    public static boolean isMonospaced(Font font) {
        int[] arr = g2d.getFontMetrics(font).getWidths();
        int w = arr['a'];
        // The range 33--126 corresponds to the usual characters in ASCII
        for (int i = 33; i < 127; i++) {
            if (arr[i] != w) {
                return false;
            }
        }

        return true;
    }
}

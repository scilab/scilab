/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Allan SIMON
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

package org.scilab.modules.ui_data.variableeditor.renderers;

import java.awt.Font;
import java.awt.FontMetrics;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Toolkit;
import java.util.Map;

import java.text.DecimalFormat;
import java.text.DecimalFormatSymbols;
import java.text.NumberFormat;

import javax.swing.SwingUtilities;
import javax.swing.table.DefaultTableCellRenderer;

/**
 * CellRenderer for complex type
 * @author Allan SIMON
 * @author Calixte DENIZET
 */
@SuppressWarnings(value = { "serial" })
public class ScilabComplexRenderer extends DefaultTableCellRenderer {

    public static final int SHORT = 0;
    public static final int SHORTE = 1;
    public static final int LONG = 2;
    public static final int LONGE = 3;

    private static final NumberFormat NUMBERFORMAT = NumberFormat.getInstance();
    private static final DecimalFormatSymbols SYMBOLS = DecimalFormatSymbols.getInstance();
    private static final DecimalFormat SHORTFORMAT = new DecimalFormat("#.####");
    private static final DecimalFormat SHORTEFORMAT = new DecimalFormat("#.####E00");
    private static final DecimalFormat LONGFORMAT = new DecimalFormat("#.###############");
    private static final DecimalFormat LONGEFORMAT = new DecimalFormat("#.###############E00");
    private static final String I = "i";
    private static final String PLUS = "+";

    static {
        SYMBOLS.setInfinity("\u221E");
        SYMBOLS.setNaN("Nan");
        SYMBOLS.setExponentSeparator("e");
        SHORTFORMAT.setDecimalFormatSymbols(SYMBOLS);
        SHORTEFORMAT.setDecimalFormatSymbols(SYMBOLS);
        LONGFORMAT.setDecimalFormatSymbols(SYMBOLS);
        LONGEFORMAT.setDecimalFormatSymbols(SYMBOLS);
    }

    private static NumberFormat numberFormat = SHORTFORMAT;
    private static NumberFormat numberFormatE = SHORTEFORMAT;

    private Map desktopFontHints = (Map) Toolkit.getDefaultToolkit().getDesktopProperty("awt.font.desktophints");
    private boolean areHintsSet = false;
    private int ascent = -1;
    private Font fontFori;

    /**
     * Constructor
     */
    public ScilabComplexRenderer() {
        super();
    }

    /**
     * Set the format.
     * @param format SHORT, SHORTE, LONG or LONGE.
     */
    public static void setFormat(int format) {
        switch (format) {
            case SHORT:
                numberFormat = SHORTFORMAT;
                numberFormatE = SHORTFORMAT;
                break;
            case SHORTE:
                numberFormat = SHORTEFORMAT;
                numberFormatE = SHORTEFORMAT;
                break;
            case LONG:
                numberFormat = LONGFORMAT;
                numberFormatE = LONGEFORMAT;
                break;
            case LONGE:
                numberFormat = LONGEFORMAT;
                numberFormatE = LONGEFORMAT;
                break;
        }
    }

    /**
     * {@inheritDoc}
     */
    protected void setValue(Object value) {
        if (value == null) {
            super.setValue("");
        } else if (value instanceof String) {
            super.setValue(value);
        } else {
            super.setValue(convertComplex((Double[]) value));
        }
    }

    /**
     * Convert a finite double, %nan or +/-%inf
     * @param d the double to convert
     * @return the double into String
     */
    protected static String convertDouble(Double d) {
        double dd = d.doubleValue();
        if ((Math.abs(dd) < 1000 && Math.abs(dd) > 0.001) || dd == 0) {
            return addSignToExponent(numberFormat.format(dd));
        } else {
            return addSignToExponent(numberFormatE.format(dd));
        }
    }

    /**
     * Add a + for the exponential notation
     */
    private static String addSignToExponent(String number) {
        return number.replaceAll("e([0-9])", "e+$1");
    }

    /**
     * @param comp a complex
     * @return the string representation of this complex.
     */
    private static String convertComplex(Double[] comp) {
        String re = "";
        String im = "";
        if (comp[0] == 0 && comp[1] == 0) {
            return "0";
        } else {
            StringBuilder buf = new StringBuilder();
            if (comp[0] != 0) {
                buf.append(convertDouble(comp[0]));
            }

            if (comp[1] != 0) {
                if (comp[1] == 1) {
                    if (comp[0] == 0) {
                        buf.append("i");
                    } else {
                        buf.append("+i");
                    }
                } else if (comp[1] == -1) {
                    buf.append("-i");
                } else if (comp[1] < 0) {
                    buf.append(convertDouble(comp[1]));
                    buf.append("i");
                } else {
                    if (comp[0] != 0) {
                        buf.append("+");
                    }
                    buf.append(convertDouble(comp[1]));
                    buf.append("i");
                }
            }
            return buf.toString();
        }
    }

    /**
     * {@inheritDoc}
     */
    public void setFont(Font f) {
        super.setFont(f);
        ascent = -1;
    }

    /**
     * {@inheritDoc}
     */
    protected void paintComponent(Graphics g) {
        String value = getText();
        if (!value.isEmpty() && value.charAt(value.length() - 1) == 'i') {
            Font font = getFont();
            FontMetrics fm = getFontMetrics(font);
            ((Graphics2D) g).addRenderingHints(desktopFontHints);
            if (ascent == -1) {
                ascent = fm.getAscent();
                fontFori = new Font(Font.SERIF, Font.ITALIC | Font.BOLD, font.getSize());
            }

            g.setColor(getBackground());
            g.fillRect(0, 0, getWidth(), getHeight());

            g.setColor(getForeground());
            g.setFont(font);

            String s = value.substring(0, value.length() - 1);
            int w = SwingUtilities.computeStringWidth(fm, s);
            g.drawString(s, 0, ascent);
            g.setFont(fontFori);
            g.drawString("i", w + 1, ascent);
        } else {
            super.paintComponent(g);
        }
    }

    /**
     * @param comp a complex
     * @return true if it is the null complex
     */
    protected static boolean isNull(Double[] comp) {
        return comp == null || (comp[0] == 0 && comp[1] == 0);
    }
}

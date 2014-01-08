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

package org.scilab.modules.graphic_objects.converters;

import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Component;
import java.awt.Composite;
import java.awt.CompositeContext;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Paint;
import java.awt.RenderingHints;
import java.awt.Toolkit;
import java.awt.font.TextAttribute;
import java.awt.image.ColorModel;
import java.awt.image.Raster;
import java.awt.image.WritableRaster;
import java.util.HashMap;
import java.util.Map;

import javax.swing.AbstractButton;
import javax.swing.Icon;
import javax.swing.UIManager;

public class UITools {

    public enum FontUnit {
        POINTS, PIXELS, NORMALIZED;

        public static String getAsString(FontUnit u) {
            switch (u) {
                case POINTS:
                    return "points";
                case PIXELS:
                    return "pixels";
                case NORMALIZED:
                    return "normalized";
                default:
                    return "points";
            }
        }

        public static FontUnit fromString(String s) {
            if (s == null || s.isEmpty()) {
                return POINTS;
            }

            if (s.equalsIgnoreCase("pixels")) {
                return PIXELS;
            }

            if (s.equalsIgnoreCase("normalized")) {
                return NORMALIZED;
            }

            return POINTS;
        }

        public static double getRatio(FontUnit unit) {
            switch (unit) {
                case POINTS:
                    return ((double) Toolkit.getDefaultToolkit().getScreenResolution()) / 72.;
                case PIXELS:
                    return 1.d;
                case NORMALIZED:
                    // Don't know what to return here... TODO: ask to Vincent
                    return 1.d;
                default:
                    return 1.d;
            }
        }

        public static double getRatio(FontUnit old, FontUnit niou) {
            if (old == niou) {
                return 1.d;
            }
            double oldR = getRatio(old);
            double niouR = getRatio(niou);

            return niouR / oldR;
        }
    }

    private static final Composite rolloverComposite = new Composite() {
        public CompositeContext createContext(ColorModel srcColorModel, ColorModel dstColorModel, RenderingHints hints) {
            return new CompositeContext() {
                public void dispose() { }

                public void compose(Raster src, Raster dstIn, WritableRaster dstOut) {
                    int[] srcPixels = new int[4];
                    for (int x = 0; x < dstOut.getWidth(); x++) {
                        for (int y = 0; y < dstOut.getHeight(); y++) {
                            src.getPixel(x, y, srcPixels);
                            // Ignore transparent pixels
                            if (srcPixels[3] != 0) {
                                // Lighten each color by 1/2, and increasing the blue
                                srcPixels[0] = (srcPixels[0] & 0xFF) / 2;
                                srcPixels[1] = (srcPixels[1] & 0xFF) / 2;
                                srcPixels[2] = (srcPixels[2] & 0xFF) / 2 + 68;
                                dstOut.setPixel(x, y, srcPixels);
                            }
                        }
                    }
                }
            };
        }
    };

    public static final Map<String, Color> mapColor = new HashMap<String, Color>();
    static {
        mapColor.put("blue", Color.BLUE);
        mapColor.put("black", Color.BLACK);
        mapColor.put("white", Color.WHITE);
        mapColor.put("orange", Color.ORANGE);
        mapColor.put("cyan", Color.CYAN);
        mapColor.put("green", Color.GREEN);
        mapColor.put("pink", Color.PINK);
        mapColor.put("gray", Color.GRAY);
        mapColor.put("magenta", Color.MAGENTA);
        mapColor.put("red", Color.RED);
        mapColor.put("yellow", Color.YELLOW);
        mapColor.put("lightgray", Color.LIGHT_GRAY);
        mapColor.put("darkgray", Color.DARK_GRAY);
    }

    public static final Color getColor(String s) {
        if (s == null || s.isEmpty()) {
            return null;
        }

        Color color = null;
        if (s.charAt(0) != '#') {
            color = mapColor.get(s);
        }

        if (color == null) {
            try {
                color = Color.decode(s);
            } catch (NumberFormatException e) {
            }
        }

        if (color == null) {
            String[] toks = s.split("[ ,;]");
            if (toks.length == 3) {
                int[] comp = new int[3];
                for (int i = 0; i < 3; i++) {
                    try {
                        comp[i] = Integer.parseInt(toks[i]);
                        comp[i] = comp[i] > 255 ? 255 : (comp[i] < 0 ? 0 : comp[i]);
                    } catch (NumberFormatException e) {
                        comp[i] = 0;
                    }
                }
                color = new Color(comp[0], comp[1], comp[2]);
            } else {
                color = Color.BLACK;
            }
        }

        return color;
    }

    public static final Map<String, FontWeight> mapWeight = new HashMap<String, FontWeight>();
    static {
        mapWeight.put("extra_light", FontWeight.EXTRA_LIGHT);
        mapWeight.put("extra-light", FontWeight.EXTRA_LIGHT);
        mapWeight.put("extralight", FontWeight.EXTRA_LIGHT);
        mapWeight.put("el", FontWeight.EXTRA_LIGHT);
        mapWeight.put("light", FontWeight.LIGHT);
        mapWeight.put("l", FontWeight.LIGHT);
        mapWeight.put("demi_light", FontWeight.DEMILIGHT);
        mapWeight.put("demi-light", FontWeight.DEMILIGHT);
        mapWeight.put("demilight", FontWeight.DEMILIGHT);
        mapWeight.put("dl", FontWeight.DEMILIGHT);
        mapWeight.put("regular", FontWeight.REGULAR);
        mapWeight.put("r", FontWeight.REGULAR);
        mapWeight.put("normal", FontWeight.REGULAR);
        mapWeight.put("n", FontWeight.REGULAR);
        mapWeight.put("semi_bold", FontWeight.SEMIBOLD);
        mapWeight.put("semi-bold", FontWeight.SEMIBOLD);
        mapWeight.put("semibold", FontWeight.SEMIBOLD);
        mapWeight.put("sb", FontWeight.SEMIBOLD);
        mapWeight.put("medium", FontWeight.MEDIUM);
        mapWeight.put("m", FontWeight.MEDIUM);
        mapWeight.put("demi_bold", FontWeight.DEMIBOLD);
        mapWeight.put("demi-bold", FontWeight.DEMIBOLD);
        mapWeight.put("demibold", FontWeight.DEMIBOLD);
        mapWeight.put("db", FontWeight.DEMIBOLD);
        mapWeight.put("demi", FontWeight.DEMIBOLD);
        mapWeight.put("d", FontWeight.DEMIBOLD);
        mapWeight.put("bold", FontWeight.BOLD);
        mapWeight.put("b", FontWeight.BOLD);
        mapWeight.put("heavy", FontWeight.HEAVY);
        mapWeight.put("h", FontWeight.HEAVY);
        mapWeight.put("extra_bold", FontWeight.EXTRABOLD);
        mapWeight.put("extra-bold", FontWeight.EXTRABOLD);
        mapWeight.put("extrabold", FontWeight.EXTRABOLD);
        mapWeight.put("eb", FontWeight.EXTRABOLD);
        mapWeight.put("ultra_bold", FontWeight.ULTRABOLD);
        mapWeight.put("ultra-bold", FontWeight.ULTRABOLD);
        mapWeight.put("ultrabold", FontWeight.ULTRABOLD);
        mapWeight.put("ub", FontWeight.ULTRABOLD);
    }

    public static final Map<Float, String> mapTextAttribute = new HashMap<Float, String>();
    static {
        mapTextAttribute.put(TextAttribute.WEIGHT_EXTRA_LIGHT, "extra-light");
        mapTextAttribute.put(TextAttribute.WEIGHT_LIGHT, "light");
        mapTextAttribute.put(TextAttribute.WEIGHT_DEMILIGHT, "demi-light");
        mapTextAttribute.put(TextAttribute.WEIGHT_REGULAR, "regular");
        mapTextAttribute.put(TextAttribute.WEIGHT_SEMIBOLD, "semi-bold");
        mapTextAttribute.put(TextAttribute.WEIGHT_MEDIUM, "medium");
        mapTextAttribute.put(TextAttribute.WEIGHT_DEMIBOLD, "demi-bold");
        mapTextAttribute.put(TextAttribute.WEIGHT_BOLD, "bold");
        mapTextAttribute.put(TextAttribute.WEIGHT_HEAVY, "heavy");
        mapTextAttribute.put(TextAttribute.WEIGHT_EXTRABOLD, "extra-bold");
        mapTextAttribute.put(TextAttribute.WEIGHT_ULTRABOLD, "ultra-bold");
        mapTextAttribute.put(TextAttribute.POSTURE_OBLIQUE, "italic");
        mapTextAttribute.put(TextAttribute.POSTURE_REGULAR, "regular");
        mapTextAttribute.put(null, "regular");
    }

    public enum FontWeight {
        EXTRA_LIGHT (TextAttribute.WEIGHT_EXTRA_LIGHT),
        LIGHT (TextAttribute.WEIGHT_LIGHT),
        DEMILIGHT (TextAttribute.WEIGHT_DEMILIGHT),
        REGULAR (TextAttribute.WEIGHT_REGULAR),
        SEMIBOLD (TextAttribute.WEIGHT_SEMIBOLD),
        MEDIUM (TextAttribute.WEIGHT_MEDIUM),
        DEMIBOLD (TextAttribute.WEIGHT_DEMIBOLD),
        BOLD (TextAttribute.WEIGHT_BOLD),
        HEAVY (TextAttribute.WEIGHT_HEAVY),
        EXTRABOLD (TextAttribute.WEIGHT_EXTRABOLD),
        ULTRABOLD (TextAttribute.WEIGHT_ULTRABOLD);

        private final Float value;

        FontWeight(Float value) {
            this.value = value;
        }

        public Float value() {
            return value;
        }
    }

    public static Font getFont(Map<String, String> style) {
        Font f = UIManager.getFont("Label.font");
        if (style == null || style.isEmpty()) {
            return f;
        } else {
            return getFont(f, style);
        }
    }

    public static Color getFontColor(Font font) {
        Map < TextAttribute, ? > map = (Map < TextAttribute, ? >) font.getAttributes();
        Paint p = (Paint) map.get(TextAttribute.FOREGROUND);
        if (p instanceof Color) {
            return (Color) p;
        }

        return Color.BLACK;
    }

    public static Font getFont(Font font, Map<String, String> style) {
        String name = style.get("font-family");
        Color bg = StringConverters.getObjectFromValue(Color.class, style.get("background"));
        Color fg = StringConverters.getObjectFromValue(Color.class, style.get("color"));
        double size = StringConverters.getObjectFromValue(double.class, style.get("font-size"));
        boolean underline = StringConverters.getObjectFromValue(boolean.class, style.get("underline"));
        boolean bold = StringConverters.getObjectFromValue(boolean.class, style.get("bold"));
        boolean italic = StringConverters.getObjectFromValue(boolean.class, style.get("italic"));
        boolean strikethrough = StringConverters.getObjectFromValue(boolean.class, style.get("strikethrough"));
        FontWeight weight = StringConverters.getObjectFromValue(FontWeight.class, style.get("font-weight"));

        style.remove("background");
        style.remove("color");
        style.remove("font-size");
        style.remove("underline");
        style.remove("bold");
        style.remove("italic");
        style.remove("strikethrough");
        style.remove("font-weight");

        if (name == null) {
            return getFont(bg, fg, font, size, underline, bold, italic, strikethrough, weight);
        } else {
            return getFont(bg, fg, name, size, underline, bold, italic, strikethrough, weight);
        }
    }

    public static Font getFont(Color bg, Color fg, String font, double size, boolean underline, boolean bold, boolean italic, boolean strikethrough, FontWeight weight) {
        Map<TextAttribute, Object> map = new HashMap<TextAttribute, Object>();
        map.put(TextAttribute.UNDERLINE, underline ? TextAttribute.UNDERLINE_ON : -1);
        map.put(TextAttribute.WEIGHT, bold ? TextAttribute.WEIGHT_BOLD : TextAttribute.WEIGHT_REGULAR);
        map.put(TextAttribute.POSTURE, italic ? TextAttribute.POSTURE_OBLIQUE : TextAttribute.POSTURE_REGULAR);
        map.put(TextAttribute.STRIKETHROUGH, new Boolean(strikethrough));
        map.put(TextAttribute.FOREGROUND, fg);
        map.put(TextAttribute.BACKGROUND, bg);
        map.put(TextAttribute.SIZE, Double.isNaN(size) ? new Double(12.0) : new Double(size));
        map.put(TextAttribute.FAMILY, font);

        if (weight != null) {
            map.put(TextAttribute.WEIGHT, weight);
        }

        return new Font(map);
    }

    @SuppressWarnings("unchecked")
    public static Font getFont(Color bg, Color fg, Font font, double size, boolean underline, boolean bold, boolean italic, boolean strikethrough, FontWeight weight) {
        Map<TextAttribute, Object> map = (Map<TextAttribute, Object>) font.getAttributes();
        map.put(TextAttribute.UNDERLINE, underline ? TextAttribute.UNDERLINE_ON : -1);
        map.put(TextAttribute.WEIGHT, bold ? TextAttribute.WEIGHT_BOLD : TextAttribute.WEIGHT_REGULAR);
        map.put(TextAttribute.POSTURE, italic ? TextAttribute.POSTURE_OBLIQUE : TextAttribute.POSTURE_REGULAR);
        map.put(TextAttribute.STRIKETHROUGH, new Boolean(strikethrough));

        if (weight != null) {
            map.put(TextAttribute.WEIGHT, weight);
        }

        if (fg != null) {
            map.put(TextAttribute.FOREGROUND, fg);
        }

        if (bg != null) {
            map.put(TextAttribute.BACKGROUND, bg);
        }

        if (!Double.isNaN(size)) {
            map.put(TextAttribute.SIZE, new Double(size));
        }

        return new Font(map);
    }

    public static boolean setTextAndMnemonic(String text, AbstractButton button) {
        if (text != null) {
            final int len = text.length();
            for (int i = 0; i < len; i++) {
                final char c = text.charAt(i);
                char n;
                if (c == '&' && i < len - 1 && (n = text.charAt(i + 1)) != '&') {
                    text = text.substring(0, i) + text.substring(i + 1);
                    text.replaceAll("&&", "&");
                    button.setText(text);
                    button.setMnemonic((int) n);

                    return true;
                }
            }

            button.setText(text);
        }

        return false;
    }

    public static Icon getColoredIcon(Dimension dim, Color color, Color border) {
        return new ColoredIcon(dim, color, border);
    }

    private static class ColoredIcon implements Icon {
        protected Color color;
        protected Color border;
        protected Dimension dim;

        public ColoredIcon(Dimension dim, Color color, Color border) {
            this.dim = dim;
            this.color = color;
            this.border = border;
        }

        public int getIconHeight() {
            return dim.height;
        }

        public int getIconWidth() {
            return dim.width;
        }

        public void paintIcon(Component c, Graphics g, int x, int y) {
            Graphics2D g2d = (Graphics2D) g;
            g2d.setColor(color);
            g2d.setStroke(new BasicStroke(1.0f));
            g2d.fillRect(x, y, dim.width, dim.height);
            if (border != null) {
                g2d.setColor(border);
                g2d.drawRect(x, y, dim.width, dim.height);
            }
        }
    }

    public static Icon getRolloverIcon(Icon icon) {
        return new RolloverIcon(icon);
    }

    private static class RolloverIcon implements Icon {
        protected Icon icon;

        public RolloverIcon(Icon icon) {
            this.icon = icon;
        }

        public int getIconHeight() {
            return this.icon.getIconHeight();
        }

        public int getIconWidth() {
            return this.icon.getIconWidth();
        }

        public void paintIcon(Component c, Graphics g, int x, int y) {
            Graphics2D g2d = (Graphics2D) g;
            Composite oldComposite = g2d.getComposite();
            g2d.setComposite(rolloverComposite);
            this.icon.paintIcon(c, g, x, y);
            g2d.setComposite(oldComposite);
        }
    }
}
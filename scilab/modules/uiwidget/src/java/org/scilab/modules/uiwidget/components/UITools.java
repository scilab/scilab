/*
 * Uicontrol2 ( http://forge.scilab.org/index.php/p/uicontrol2/ ) - This file is a part of Uicontrol2
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.uiwidget.components;

import java.awt.Color;
import java.awt.Component;
import java.awt.Composite;
import java.awt.CompositeContext;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Font;
import java.awt.RenderingHints;
import java.awt.font.TextAttribute;
import java.awt.image.ColorModel;
import java.awt.image.Raster;
import java.awt.image.WritableRaster;
import java.util.HashMap;
import java.util.Map;

import javax.swing.Icon;

import org.scilab.modules.uiwidget.StringConverters;

public class UITools {

    private static final Composite rolloverComposite = new Composite() {
        public CompositeContext createContext(ColorModel srcColorModel, ColorModel dstColorModel, RenderingHints hints) {
            return new CompositeContext() {
                public void dispose() { }

                public void compose(Raster src, Raster dstIn, WritableRaster dstOut) {
                    for (int x = 0; x < dstOut.getWidth(); x++) {
                        for (int y = 0; y < dstOut.getHeight(); y++) {
                            int[] srcPixels = new int[4];
                            src.getPixel(x, y, srcPixels);
                            // Ignore transparent pixels
                            if (srcPixels[3] != 0) {
                                // Lighten each color by 1/2, and increasing the blue
                                srcPixels[0] = srcPixels[0] / 2;
                                srcPixels[1] = srcPixels[1] / 2;
                                srcPixels[2] = srcPixels[2] / 2 + 68;
                                dstOut.setPixel(x, y, srcPixels);
                            }
                        }
                    }
                }
            };
        }
    };

    public static final Map<String, FontWeight> mapWeight = new HashMap<String, FontWeight>();
    static {
        mapWeight.put("extra_light", FontWeight.EXTRA_LIGHT);
        mapWeight.put("extra-light", FontWeight.EXTRA_LIGHT);
        mapWeight.put("extralight", FontWeight.EXTRA_LIGHT);
        mapWeight.put("light", FontWeight.LIGHT);
        mapWeight.put("demi_light", FontWeight.DEMILIGHT);
        mapWeight.put("demi-light", FontWeight.DEMILIGHT);
        mapWeight.put("demilight", FontWeight.DEMILIGHT);
        mapWeight.put("regular", FontWeight.REGULAR);
        mapWeight.put("normal", FontWeight.REGULAR);
        mapWeight.put("semi_bold", FontWeight.SEMIBOLD);
        mapWeight.put("semi-bold", FontWeight.SEMIBOLD);
        mapWeight.put("semibold", FontWeight.SEMIBOLD);
        mapWeight.put("medium", FontWeight.MEDIUM);
        mapWeight.put("demi_bold", FontWeight.DEMIBOLD);
        mapWeight.put("demi-bold", FontWeight.DEMIBOLD);
        mapWeight.put("demibold", FontWeight.DEMIBOLD);
        mapWeight.put("demi", FontWeight.DEMIBOLD);
        mapWeight.put("bold", FontWeight.BOLD);
        mapWeight.put("heavy", FontWeight.HEAVY);
        mapWeight.put("extra_bold", FontWeight.EXTRABOLD);
        mapWeight.put("extra-bold", FontWeight.EXTRABOLD);
        mapWeight.put("extrabold", FontWeight.EXTRABOLD);
        mapWeight.put("ultra_bold", FontWeight.ULTRABOLD);
        mapWeight.put("ultra-bold", FontWeight.ULTRABOLD);
        mapWeight.put("ultrabold", FontWeight.ULTRABOLD);
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

    public static Icon getRolloverIcon(Icon icon) {
        return new RolloverIcon(icon) {
        };
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
            Graphics2D graphics2d = (Graphics2D) g;
            Composite oldComposite = graphics2d.getComposite();
            graphics2d.setComposite(rolloverComposite);
            this.icon.paintIcon(c, g, x, y);
            graphics2d.setComposite(oldComposite);
        }
    }
}
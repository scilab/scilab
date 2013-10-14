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

package org.scilab.modules.gui.uiwidget;

import java.awt.Color;
import java.awt.Font;
import java.util.HashMap;
import java.util.Map;

import javax.swing.BorderFactory;
import javax.swing.Icon;
import javax.swing.UIManager;
import javax.swing.border.BevelBorder;
import javax.swing.border.Border;
import javax.swing.border.EtchedBorder;
import javax.swing.border.TitledBorder;

import org.scilab.modules.gui.uiwidget.components.UITools;

/**
 * Border Factory
 */
public final class UIBorderFactory {

    private static final Border defaultTitledBorder = BorderFactory.createTitledBorder("").getBorder();
    private static final Map<String, BorderCreator> creators = new HashMap<String, BorderCreator>();

    static {
        creators.put("bevel", new BorderCreator() {
            public Border create(Map<String, String> map) {
                String type = map.get("type");
                if (type == null || type.isEmpty()) {
                    return null;
                }
                int itype = type.equalsIgnoreCase("lowered") ? BevelBorder.LOWERED : BevelBorder.RAISED;
                String highlight = map.get("highlight");
                String shadow = map.get("shadow");
                if (highlight == null || shadow == null) {
                    return BorderFactory.createBevelBorder(itype);
                }

                return BorderFactory.createBevelBorder(itype, StringConverters.getObjectFromValue(Color.class, highlight), StringConverters.getObjectFromValue(Color.class, shadow));
            }
        });
        creators.put("etched", new BorderCreator() {
            public Border create(Map<String, String> map) {
                String type = map.get("type");
                String highlight = map.get("highlight");
                String shadow = map.get("shadow");
                if (type == null || type.isEmpty()) {
                    if (highlight == null || shadow == null) {
                        return BorderFactory.createEtchedBorder();
                    }
                    return BorderFactory.createEtchedBorder(StringConverters.getObjectFromValue(Color.class, highlight), StringConverters.getObjectFromValue(Color.class, shadow));
                }

                int itype = type.equalsIgnoreCase("lowered") ? EtchedBorder.LOWERED : EtchedBorder.RAISED;
                if (highlight == null || shadow == null) {
                    return BorderFactory.createEtchedBorder(itype);
                }
                return BorderFactory.createEtchedBorder(itype, StringConverters.getObjectFromValue(Color.class, highlight), StringConverters.getObjectFromValue(Color.class, shadow));
            }
        });
        creators.put("titled", new BorderCreator() {

            public Border create(Map<String, String> map) {
                String border = map.get("border");
                String title = map.get("title");
                Border b = defaultTitledBorder;
                title = title == null ? "" : title;

                if (border != null && !border.isEmpty()) {
                    border = border.toLowerCase();
                    if (border.equals("titled")) {
                        return null;
                    }

                    b = UIBorderFactory.getBorder(border, map);
                }

                if (b == null) {
                    b = defaultTitledBorder;
                }

                String justification = map.get("justification");
                String position = map.get("position");
                String font = map.get("font");
                String bold = map.get("bold");
                String italic = map.get("italic");
                String size = map.get("size");

                int ijustification = TitledBorder.DEFAULT_JUSTIFICATION;
                if (justification != null) {
                    justification = justification.toLowerCase();
                    if (justification.equals("left")) {
                        ijustification = TitledBorder.LEFT;
                    } else if (justification.equals("center")) {
                        ijustification = TitledBorder.CENTER;
                    } else if (justification.equals("right")) {
                        ijustification = TitledBorder.RIGHT;
                    } else if (justification.equals("trailing")) {
                        ijustification = TitledBorder.TRAILING;
                    }
                }

                int iposition = TitledBorder.DEFAULT_POSITION;
                if (position != null) {
                    position = position.toLowerCase();
                    if (position.equals("above_top")) {
                        iposition = TitledBorder.ABOVE_TOP;
                    } else if (position.equals("below_top")) {
                        iposition = TitledBorder.BELOW_TOP;
                    } else if (position.equals("above_bottom")) {
                        iposition = TitledBorder.ABOVE_BOTTOM;
                    } else if (position.equals("bottom")) {
                        iposition = TitledBorder.BOTTOM;
                    } else if (position.equals("below_bottom")) {
                        iposition = TitledBorder.BELOW_BOTTOM;
                    }
                }

                Font f;
                if (font == null || font.isEmpty()) {
                    TitledBorder tb = BorderFactory.createTitledBorder(b, title, ijustification, iposition);
                    f = UIManager.getFont("TitledBorder.font");
                    f = UITools.getFont(null, null, f, StringConverters.getObjectFromValue(double.class, size), false, StringConverters.getObjectFromValue(boolean.class, bold), StringConverters.getObjectFromValue(boolean.class, italic), false, null);
                } else {
                    f = UITools.getFont(null, null, font, StringConverters.getObjectFromValue(double.class, size), false, StringConverters.getObjectFromValue(boolean.class, bold), StringConverters.getObjectFromValue(boolean.class, italic), false, null);
                }

                Color color = StringConverters.getObjectFromValue(Color.class, map.get("color"));
                if (color == null) {
                    return BorderFactory.createTitledBorder(b, title, ijustification, iposition, f);
                }

                return BorderFactory.createTitledBorder(b, title, ijustification, iposition, f, color);
            }
        });
        creators.put("empty", new BorderCreator() {
            public Border create(Map<String, String> map) {
                int top = StringConverters.getObjectFromValue(int.class, map.get("top"));
                int left = StringConverters.getObjectFromValue(int.class, map.get("left"));
                int bottom = StringConverters.getObjectFromValue(int.class, map.get("bottom"));
                int right = StringConverters.getObjectFromValue(int.class, map.get("right"));

                if (top == Integer.MAX_VALUE || left == Integer.MAX_VALUE || bottom == Integer.MAX_VALUE || right == Integer.MAX_VALUE) {
                    return BorderFactory.createEmptyBorder();
                }

                return BorderFactory.createEmptyBorder(top, left, bottom, right);
            }
        });
        creators.put("matte", new BorderCreator() {
            public Border create(Map<String, String> map) {
                int top = StringConverters.getObjectFromValue(int.class, map.get("top"));
                int left = StringConverters.getObjectFromValue(int.class, map.get("left"));
                int bottom = StringConverters.getObjectFromValue(int.class, map.get("bottom"));
                int right = StringConverters.getObjectFromValue(int.class, map.get("right"));

                if (top == Integer.MAX_VALUE || left == Integer.MAX_VALUE || bottom == Integer.MAX_VALUE || right == Integer.MAX_VALUE) {
                    return null;
                }

                Color color = StringConverters.getObjectFromValue(Color.class, map.get("color"));
                Icon icon = StringConverters.getObjectFromValue(Icon.class, map.get("icon"));

                if (color == null && icon == null) {
                    return null;
                }

                if (color == null) {
                    return BorderFactory.createMatteBorder(top, left, bottom, right, icon);
                }

                return BorderFactory.createMatteBorder(top, left, bottom, right, color);
            }
        });
        creators.put("line", new BorderCreator() {
            public Border create(Map<String, String> map) {
                Color color = StringConverters.getObjectFromValue(Color.class, map.get("color"));
                int thickness = StringConverters.getObjectFromValue(int.class, map.get("thickness"));
                if (thickness == Integer.MAX_VALUE && color == null) {
                    return null;
                }

                if (thickness == Integer.MAX_VALUE) {
                    return BorderFactory.createLineBorder(color);
                }

                return BorderFactory.createLineBorder(color, thickness);
            }
        });
    }

    /**
     * Get a border from a CSS style string
     * @param str the CSS string
     * @return the corresponding Border
     */
    public static final Border getBorder(String str) {
        if (str == null || str.isEmpty()) {
            return null;
        }
        Map<String, String> map = StyleParser.parseLine(str);
        String name = map.get("name");
        if (name == null || name.isEmpty()) {
            return null;
        }
        name = name.toLowerCase();

        BorderCreator creator = creators.get(name);
        if (creator == null) {
            return null;
        }

        return creator.create(map);
    }

    /**
     * Get a border
     * @param name the border name
     * @param map the border properties
     * @return the corresponding Border
     */
    public static final Border getBorder(String name, Map<String, String> map) {
        BorderCreator creator = creators.get(name.toLowerCase());
        if (creator == null) {
            return null;
        }

        return creator.create(map);
    }

    /**
     * Inner interface to create a Border
     */
    private static interface BorderCreator {

        /**
         * Create a Border with the given properties in the map
         * @param map the properties
         * @return the Border
         */
        Border create(Map<String, String> map);
    }
}

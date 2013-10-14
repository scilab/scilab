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

import java.awt.BorderLayout;
import java.awt.CardLayout;
import java.awt.Container;
import java.awt.FlowLayout;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.GridLayout;
import java.awt.LayoutManager;
import java.util.HashMap;
import java.util.Map;

import javax.swing.BoxLayout;

import org.scilab.modules.gui.uiwidget.components.NoLayout;

/**
 * LayoutManager factory
 */
public final class UILayoutFactory {

    /**
     * Constants for gridbag layout
     */
    public static final Map<String, Integer> GridBagConstants = new HashMap<String, Integer>();

    static {
        GridBagConstants.put("northwest", GridBagConstraints.NORTHWEST);
        GridBagConstants.put("nw", GridBagConstraints.NORTHWEST);
        GridBagConstants.put("north", GridBagConstraints.NORTH);
        GridBagConstants.put("n", GridBagConstraints.NORTH);
        GridBagConstants.put("northeast", GridBagConstraints.NORTHEAST);
        GridBagConstants.put("ne", GridBagConstraints.NORTHEAST);
        GridBagConstants.put("west", GridBagConstraints.WEST);
        GridBagConstants.put("w", GridBagConstraints.WEST);
        GridBagConstants.put("center", GridBagConstraints.CENTER);
        GridBagConstants.put("c", GridBagConstraints.CENTER);
        GridBagConstants.put("east", GridBagConstraints.EAST);
        GridBagConstants.put("e", GridBagConstraints.EAST);
        GridBagConstants.put("southwest", GridBagConstraints.SOUTHWEST);
        GridBagConstants.put("sw", GridBagConstraints.SOUTHWEST);
        GridBagConstants.put("south", GridBagConstraints.SOUTH);
        GridBagConstants.put("s", GridBagConstraints.SOUTH);
        GridBagConstants.put("southeast", GridBagConstraints.SOUTHEAST);
        GridBagConstants.put("se", GridBagConstraints.SOUTHEAST);
        GridBagConstants.put("first_line_start", GridBagConstraints.FIRST_LINE_START);
        GridBagConstants.put("page_start", GridBagConstraints.PAGE_START);
        GridBagConstants.put("first_line_end", GridBagConstraints.FIRST_LINE_END);
        GridBagConstants.put("line_start", GridBagConstraints.LINE_START);
        GridBagConstants.put("line_end", GridBagConstraints.LINE_END);
        GridBagConstants.put("last_line_start", GridBagConstraints.LAST_LINE_START);
        GridBagConstants.put("page_end", GridBagConstraints.PAGE_END);
        GridBagConstants.put("last_line_end", GridBagConstraints.LAST_LINE_END);
        GridBagConstants.put("baseline", GridBagConstraints.BASELINE);
        GridBagConstants.put("baseline_leading", GridBagConstraints.BASELINE_LEADING);
        GridBagConstants.put("baseline_trailing", GridBagConstraints.BASELINE_TRAILING);
        GridBagConstants.put("above_baseline", GridBagConstraints.ABOVE_BASELINE);
        GridBagConstants.put("above_baseline_leading", GridBagConstraints.ABOVE_BASELINE_LEADING);
        GridBagConstants.put("above_baseline_trailing", GridBagConstraints.ABOVE_BASELINE_TRAILING);
        GridBagConstants.put("below_baseline", GridBagConstraints.BELOW_BASELINE);
        GridBagConstants.put("below_baseline_leading", GridBagConstraints.BELOW_BASELINE_LEADING);
        GridBagConstants.put("below_baseline_trailing", GridBagConstraints.BELOW_BASELINE_TRAILING);
        GridBagConstants.put("none", GridBagConstraints.NONE);
        GridBagConstants.put("horizontal", GridBagConstraints.HORIZONTAL);
        GridBagConstants.put("h", GridBagConstraints.HORIZONTAL);
        GridBagConstants.put("vertical", GridBagConstraints.VERTICAL);
        GridBagConstants.put("v", GridBagConstraints.VERTICAL);
        GridBagConstants.put("both", GridBagConstraints.BOTH);
        GridBagConstants.put("b", GridBagConstraints.BOTH);
    }

    /**
     * Constants for border layout
     */
    public static final Map<String, String> BorderConstants = new HashMap<String, String>();

    static {
        BorderConstants.put("north", BorderLayout.NORTH);
        BorderConstants.put("n", BorderLayout.NORTH);
        BorderConstants.put("west", BorderLayout.WEST);
        BorderConstants.put("w", BorderLayout.WEST);
        BorderConstants.put("center", BorderLayout.CENTER);
        BorderConstants.put("c", BorderLayout.CENTER);
        BorderConstants.put(null, BorderLayout.CENTER);
        BorderConstants.put("", BorderLayout.CENTER);
        BorderConstants.put("east", BorderLayout.EAST);
        BorderConstants.put("e", BorderLayout.EAST);
        BorderConstants.put("south", BorderLayout.SOUTH);
        BorderConstants.put("s", BorderLayout.SOUTH);
    }

    private static final Map<String, LayoutCreator> creators = new HashMap<String, LayoutCreator>();

    static {
        creators.put("nolayout", new LayoutCreator() {
            public LayoutManager create(Container c, Map<String, String> map) {
                return new NoLayout();
            }
        });
        creators.put("border", new LayoutCreator() {
            public LayoutManager create(Container c, Map<String, String> map) {
                BorderLayout layout = new BorderLayout();
                int hgap = StringConverters.getObjectFromValue(int.class, map.get("hgap"));
                int vgap = StringConverters.getObjectFromValue(int.class, map.get("vgap"));

                if (hgap != Integer.MAX_VALUE) {
                    layout.setHgap(hgap);
                }

                if (vgap != Integer.MAX_VALUE) {
                    layout.setVgap(vgap);
                }

                return layout;
            }
        });
        creators.put("box", new LayoutCreator() {
            public LayoutManager create(Container c, Map<String, String> map) {
                String axis = map.get("axis");
                int iaxis = BoxLayout.X_AXIS;
                if (axis != null && axis.equalsIgnoreCase("y")) {
                    iaxis = BoxLayout.Y_AXIS;
                }

                return new BoxLayout(c, iaxis);
            }
        });
        creators.put("card", new LayoutCreator() {
            public LayoutManager create(Container c, Map<String, String> map) {
                CardLayout layout = new CardLayout();
                int hgap = StringConverters.getObjectFromValue(int.class, map.get("hgap"));
                int vgap = StringConverters.getObjectFromValue(int.class, map.get("vgap"));

                if (hgap != Integer.MAX_VALUE) {
                    layout.setHgap(hgap);
                }

                if (vgap != Integer.MAX_VALUE) {
                    layout.setVgap(vgap);
                }

                return layout;
            }
        });
        creators.put("flow", new LayoutCreator() {
            public LayoutManager create(Container c, Map<String, String> map) {
                FlowLayout layout = new FlowLayout();
                int hgap = StringConverters.getObjectFromValue(int.class, map.get("hgap"));
                int vgap = StringConverters.getObjectFromValue(int.class, map.get("vgap"));
                String alignment = map.get("alignment");
                int ialignment = FlowLayout.CENTER;

                if (alignment != null) {
                    alignment = alignment.toLowerCase();
                    if (alignment.equals("leading")) {
                        ialignment = FlowLayout.LEADING;
                    } else if (alignment.equals("left")) {
                        ialignment = FlowLayout.LEFT;
                    } else if (alignment.equals("right")) {
                        ialignment = FlowLayout.RIGHT;
                    } else if (alignment.equals("trailing")) {
                        ialignment = FlowLayout.TRAILING;
                    }
                }

                if (hgap != Integer.MAX_VALUE) {
                    layout.setHgap(hgap);
                }

                if (vgap != Integer.MAX_VALUE) {
                    layout.setVgap(vgap);
                }

                layout.setAlignment(ialignment);

                return layout;
            }
        });
        creators.put("grid", new LayoutCreator() {
            public LayoutManager create(Container c, Map<String, String> map) {
                GridLayout layout = new GridLayout();
                int hgap = StringConverters.getObjectFromValue(int.class, map.get("hgap"));
                int vgap = StringConverters.getObjectFromValue(int.class, map.get("vgap"));
                int rows = StringConverters.getObjectFromValue(int.class, map.get("rows"));
                int cols = StringConverters.getObjectFromValue(int.class, map.get("cols"));

                if (rows != Integer.MAX_VALUE) {
                    layout.setRows(rows);
                }

                if (cols != Integer.MAX_VALUE) {
                    layout.setColumns(cols);
                }

                if (hgap != Integer.MAX_VALUE) {
                    layout.setHgap(hgap);
                }

                if (vgap != Integer.MAX_VALUE) {
                    layout.setVgap(vgap);
                }

                return layout;
            }
        });
        creators.put("gridbag", new LayoutCreator() {
            public LayoutManager create(Container c, Map<String, String> map) {
                return new GridBagLayout();
            }
        });
    }

    /**
     * Get a string representation of the given layout
     * @param layout the layout
     * @return the string representation
     */
    public static String getStringRepresentation(LayoutManager layout) {
        if (layout instanceof BorderLayout) {
            BorderLayout border = (BorderLayout) layout;
            int hgap = border.getHgap();
            int vgap = border.getVgap();

            String ret = "name:border";
            if (hgap != 0) {
                ret += "; hgap:" + Integer.toString(hgap);
            }
            if (vgap != 0) {
                ret += "; vgap:" + Integer.toString(vgap);
            }

            return ret;
        } else if (layout instanceof BoxLayout) {
            BoxLayout box = (BoxLayout) layout;
            int axis = box.getAxis();

            String ret = "name:box";
            if (axis == BoxLayout.X_AXIS) {
                ret += "; axis:x";
            } else {
                ret += "; axis:y";
            }

            return ret;
        } else if (layout instanceof CardLayout) {
            CardLayout card = (CardLayout) layout;
            int hgap = card.getHgap();
            int vgap = card.getVgap();

            String ret = "name:card";
            if (hgap != 0) {
                ret += "; hgap:" + Integer.toString(hgap);
            }
            if (vgap != 0) {
                ret += "; vgap:" + Integer.toString(vgap);
            }

            return ret;
        } else if (layout instanceof FlowLayout) {
            FlowLayout flow = (FlowLayout) layout;
            int hgap = flow.getHgap();
            int vgap = flow.getVgap();
            int alignment = flow.getAlignment();

            String ret = "name:flow";
            if (hgap != 0) {
                ret += "; hgap:" + Integer.toString(hgap);
            }
            if (vgap != 0) {
                ret += "; vgap:" + Integer.toString(vgap);
            }

            switch (alignment) {
                case FlowLayout.LEADING:
                    ret += "; alignment:leading";
                    break;
                case FlowLayout.LEFT:
                    ret += "; alignment:left";
                    break;
                case FlowLayout.RIGHT:
                    ret += "; alignment:right";
                    break;
                case FlowLayout.TRAILING:
                    ret += "; alignment:trailing";
                    break;
                default:
                    ret += "; alignment:center";
            }

            return ret;
        } else if (layout instanceof GridLayout) {
            GridLayout grid = (GridLayout) layout;
            int hgap = grid.getHgap();
            int vgap = grid.getVgap();
            int rows = grid.getRows();
            int columns = grid.getColumns();

            String ret = "name:grid";
            if (hgap != 0) {
                ret += "; hgap:" + Integer.toString(hgap);
            }
            if (vgap != 0) {
                ret += "; vgap:" + Integer.toString(vgap);
            }

            ret += "; rows:" + Integer.toString(rows);
            ret += "; cols:" + Integer.toString(columns);

            return ret;
        } else if (layout instanceof GridBagLayout) {
            return "name:gridbag";
        } else if (layout instanceof NoLayout) {
            return "name:nolayout";
        }

        return "unknown";
    }

    /**
     * Get the LayoutManager corresponding to the CSS string (e.g. str="name:grid; rows:2; cols:3")
     * @param c the container which will have the layout
     * @param str a CSS string
     * @return the LayoutManager
     */
    public static final LayoutManager getLayout(Container c, String str) {
        if (str == null || str.isEmpty()) {
            return null;
        }
        Map<String, String> map = StyleParser.parseLine(str);
        String name = map.get("name");
        if (name == null || name.isEmpty()) {
            return null;
        }
        name = name.toLowerCase();

        LayoutCreator creator = creators.get(name);
        if (creator == null) {
            return null;
        }

        return creator.create(c, map);
    }

    /**
     * Inner interface to create a LayoutManager
     */
    private static interface LayoutCreator {

        /**
         * Create a LayoutManager for a container c with the given properties in the map
         * @param c the container
         * @param map the properties
         * @return the LayoutManager
         */
        LayoutManager create(Container c, Map<String, String> map);
    }
}

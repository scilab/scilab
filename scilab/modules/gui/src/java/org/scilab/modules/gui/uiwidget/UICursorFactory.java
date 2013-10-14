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

import java.awt.Cursor;
import java.awt.Image;
import java.awt.Point;
import java.awt.Toolkit;
import java.util.HashMap;
import java.util.Map;

/**
 * Cursor factory
 */
public final class UICursorFactory {

    /**
     * Constants for gridbag layout
     */
    public static final Map<String, Integer> Cursors = new HashMap<String, Integer>();

    static {
        Cursors.put("crosshair", Cursor.CROSSHAIR_CURSOR);
        Cursors.put("default", Cursor.DEFAULT_CURSOR);
        Cursors.put("east-resize", Cursor.E_RESIZE_CURSOR);
        Cursors.put("e-resize", Cursor.E_RESIZE_CURSOR);
        Cursors.put("east_resize", Cursor.E_RESIZE_CURSOR);
        Cursors.put("e_resize", Cursor.E_RESIZE_CURSOR);
        Cursors.put("hand", Cursor.HAND_CURSOR);
        Cursors.put("move", Cursor.MOVE_CURSOR);
        Cursors.put("north-resize", Cursor.N_RESIZE_CURSOR);
        Cursors.put("n-resize", Cursor.N_RESIZE_CURSOR);
        Cursors.put("north_resize", Cursor.N_RESIZE_CURSOR);
        Cursors.put("n_resize", Cursor.N_RESIZE_CURSOR);
        Cursors.put("north-east-resize", Cursor.NE_RESIZE_CURSOR);
        Cursors.put("ne-resize", Cursor.NE_RESIZE_CURSOR);
        Cursors.put("north_east_resize", Cursor.NE_RESIZE_CURSOR);
        Cursors.put("ne_resize", Cursor.NE_RESIZE_CURSOR);
        Cursors.put("north-west-resize", Cursor.NW_RESIZE_CURSOR);
        Cursors.put("nw-resize", Cursor.NW_RESIZE_CURSOR);
        Cursors.put("north_west_resize", Cursor.NW_RESIZE_CURSOR);
        Cursors.put("nw_resize", Cursor.NW_RESIZE_CURSOR);
        Cursors.put("south-resize", Cursor.S_RESIZE_CURSOR);
        Cursors.put("s-resize", Cursor.S_RESIZE_CURSOR);
        Cursors.put("south_resize", Cursor.S_RESIZE_CURSOR);
        Cursors.put("s_resize", Cursor.S_RESIZE_CURSOR);
        Cursors.put("south-east-resize", Cursor.SE_RESIZE_CURSOR);
        Cursors.put("se-resize", Cursor.SE_RESIZE_CURSOR);
        Cursors.put("south_east_resize", Cursor.NE_RESIZE_CURSOR);
        Cursors.put("se_resize", Cursor.SE_RESIZE_CURSOR);
        Cursors.put("south-west-resize", Cursor.SW_RESIZE_CURSOR);
        Cursors.put("sw-resize", Cursor.SW_RESIZE_CURSOR);
        Cursors.put("south_west_resize", Cursor.SW_RESIZE_CURSOR);
        Cursors.put("sw_resize", Cursor.SW_RESIZE_CURSOR);
        Cursors.put("west-resize", Cursor.W_RESIZE_CURSOR);
        Cursors.put("w-resize", Cursor.W_RESIZE_CURSOR);
        Cursors.put("west_resize", Cursor.W_RESIZE_CURSOR);
        Cursors.put("w_resize", Cursor.W_RESIZE_CURSOR);
        Cursors.put("wait", Cursor.WAIT_CURSOR);
        Cursors.put("text", Cursor.TEXT_CURSOR);
    }

    public static final Map<Integer, String> CursorNames = new HashMap<Integer, String>();

    static {
        CursorNames.put(Cursor.CROSSHAIR_CURSOR, "crosshair");
        CursorNames.put(Cursor.DEFAULT_CURSOR, "default");
        CursorNames.put(Cursor.E_RESIZE_CURSOR, "east-resize");
        CursorNames.put(Cursor.HAND_CURSOR, "hand");
        CursorNames.put(Cursor.MOVE_CURSOR, "move");
        CursorNames.put(Cursor.N_RESIZE_CURSOR, "north-resize");
        CursorNames.put(Cursor.NE_RESIZE_CURSOR, "north-east-resize");
        CursorNames.put(Cursor.NW_RESIZE_CURSOR, "north-west-resize");
        CursorNames.put(Cursor.S_RESIZE_CURSOR, "south-resize");
        CursorNames.put(Cursor.SE_RESIZE_CURSOR, "south-east-resize");
        CursorNames.put(Cursor.SW_RESIZE_CURSOR, "south-west-resize");
        CursorNames.put(Cursor.W_RESIZE_CURSOR, "west-resize");
        CursorNames.put(Cursor.WAIT_CURSOR, "wait");
        CursorNames.put(Cursor.TEXT_CURSOR, "text");
    }

    /**
     * Get a string representation of the given cursor
     * @param c the cursor
     * @return string representation
     */
    public static final String getString(Cursor c) {
        int type = c.getType();
        if (type == Cursor.CUSTOM_CURSOR) {
            return c.getName();
        } else {
            String name = CursorNames.get(type);
            if (name == null) {
                return "default";
            }

            return name;
        }
    }

    /**
     * Get a Cursor from a name or a style
     * @param str a cursor name or a CSS string (e.g. "image:/tmp/foo.png; hotspot:0,0")
     * @return a cursor
     */
    public static final Cursor getCursor(String str) {
        if (str == null || str.isEmpty()) {
            return Cursor.getPredefinedCursor(Cursor.DEFAULT_CURSOR);
        }

        String name = str.toLowerCase();
        Integer id = Cursors.get(name);
        if (id == null) {
            // Not a predefined cursor so parse style
            Map<String, String> map = StyleParser.parseLine(str);
            String image = map.get("image");
            if (image == null || image.isEmpty()) {
                return Cursor.getPredefinedCursor(Cursor.DEFAULT_CURSOR);
            }
            Image im = StringConverters.getObjectFromValue(Image.class, image);
            if (im == null) {
                return Cursor.getPredefinedCursor(Cursor.DEFAULT_CURSOR);
            }
            String point = map.get("hotspot");
            Point p;
            if (point == null || point.isEmpty()) {
                p = new Point(0, 0);
            } else {
                p = StringConverters.getObjectFromValue(Point.class, point);
            }
            try {
                return Toolkit.getDefaultToolkit().createCustomCursor(im, p, str);
            } catch (IndexOutOfBoundsException e) {
                return Toolkit.getDefaultToolkit().createCustomCursor(im, new Point(0, 0), str);
            }
        } else {
            return Cursor.getPredefinedCursor(id);
        }
    }
}

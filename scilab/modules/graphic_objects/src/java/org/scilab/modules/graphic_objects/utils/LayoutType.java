/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
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

package org.scilab.modules.graphic_objects.utils;


public enum LayoutType {
    NONE, GRIDBAG, GRID, BORDER;

    public static LayoutType intToEnum(Integer value) {
        switch (value) {
            default:
            case 0:
                return NONE;
            case 1:
                return GRIDBAG;
            case 2:
                return GRID;
            case 3:
                return BORDER;
        }
    }

    public static LayoutType stringToEnum(String value) {

        if (value == null || value.equals("")) {
            return NONE;
        }

        char[] chars = value.toCharArray();
        if (chars[0] == 'g' || chars[0] == 'G') {
            if (value.equalsIgnoreCase("grid")) {
                return GRID;
            } else if (value.equalsIgnoreCase("gridbag")) {
                return GRIDBAG;
            }
        }

        if (chars[0] == 'b' || chars[0] == 'B') {
            return BORDER;
        }

        return NONE;
    }

    public static String enumToString(LayoutType value) {
        switch (value) {
            default:
            case NONE:
                return "none";
            case GRIDBAG:
                return "gridbag";
            case GRID:
                return "grid";
            case BORDER:
                return "border";
        }
    }
}

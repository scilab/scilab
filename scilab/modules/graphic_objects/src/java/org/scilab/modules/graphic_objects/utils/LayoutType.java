/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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

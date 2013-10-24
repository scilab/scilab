package org.scilab.modules.graphic_objects.utils;

public enum ArcDrawingMethod {
    NURBS, LINES;

    /**
     * Converts an integer to the corresponding enum
     * @param intValue the integer value
     * @return the arc drawing method enum
     */
    public static ArcDrawingMethod intToEnum(Integer intValue) {
        switch (intValue) {
            case 0:
                return ArcDrawingMethod.NURBS;
            case 1:
                return ArcDrawingMethod.LINES;
            default:
                return null;
        }
    }

    public static Integer enumToInt(ArcDrawingMethod val) {
        switch (val) {
            case NURBS:
                return 0;
            case LINES:
                return 1;
        }
        return null;
    }

    public static ArcDrawingMethod stringToEnum(String val) {
        char[] chars = val.toCharArray();

        if (chars[0] == 'l') {
            return LINES;
        }

        if (chars[0] == 'n') {
            return NURBS;
        }

        return null;
    }

    public static String enumToString(ArcDrawingMethod val) {
        switch (val) {
            case NURBS:
                return "nurbs";
            case LINES:
                return "lines";
        }
        return null;

    }
}

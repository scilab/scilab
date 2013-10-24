package org.scilab.modules.graphic_objects.utils;

/** Specifies whether rotation applies to a single subwindow or to all the figure's subwindows */
public enum RotationType {
    UNARY, MULTIPLE;

    /**
     * Converts an integer to the corresponding enum
     * @param intValue the integer value
     * @return the rotation type enum
     */
    public static RotationType intToEnum(Integer intValue) {
        switch (intValue) {
            case 0:
                return UNARY;
            case 1:
                return MULTIPLE;
            default:
                return null;
        }
    }

    public static String enumToString(RotationType val) {
        switch (val) {
            case MULTIPLE:
                return "multiple";
            case UNARY:
                return "unary";
            default:
                return "";
        }
    }

    public static RotationType stringToEnum(String val) {
        char[] chars = val.toCharArray();

        if (chars[0] == 'u') {
            return UNARY;
        }

        if (chars[0] == 'm') {
            return MULTIPLE;
        }

        return null;
    }
}

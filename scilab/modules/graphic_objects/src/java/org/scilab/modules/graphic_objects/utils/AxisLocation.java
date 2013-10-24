package org.scilab.modules.graphic_objects.utils;

/**
 * Axis location type
 * BOTTOM, TOP, MIDDLE, ORIGIN are allowed for an x-axis,
 * LEFT, RIGHT, MIDDLE or ORIGIN are allowed for a y-axis
 */
public enum AxisLocation {
    BOTTOM, TOP, MIDDLE, ORIGIN, LEFT, RIGHT;


    /**
     * Converts an integer to the corresponding enum
     * @param intValue the integer value
     * @return the axis location enum
     */
    public static AxisLocation intToEnum(Integer val) {
        switch (val) {
            case 0:
                return BOTTOM;
            case 1:
                return TOP;
            case 2:
                return MIDDLE;
            case 3:
                return ORIGIN;
            case 4:
                return LEFT;
            case 5:
                return RIGHT;
            default:
                return null;
        }
    }

    public static Integer enumToInt(AxisLocation val) {
        switch (val) {
            case BOTTOM:
                return 0;
            case LEFT:
                return 4;
            case MIDDLE:
                return 2;
            case ORIGIN:
                return 3;
            case RIGHT:
                return 5;
            case TOP:
                return 1;
            default:
                return null;
        }
    }

    public static String enumToString(AxisLocation val) {
        switch (val) {
            case BOTTOM:
                return "bottom";
            case LEFT:
                return "left";
            case MIDDLE:
                return "middle";
            case ORIGIN:
                return "origin";
            case RIGHT:
                return "right";
            case TOP:
                return "top";
            default:
                return null;
        }
    }

    public static AxisLocation stringToEnum(String val) {
        char[] chars = val.toCharArray();

        if (chars[0] == 'b') {
            return BOTTOM;
        }

        if (chars[0] == 'l') {
            return LEFT;
        }

        if (chars[0] == 'm') {
            return MIDDLE;
        }

        if (chars[0] == 'o') {
            return ORIGIN;
        }

        if (chars[0] == 'r') {
            return RIGHT;
        }

        if (chars[0] == 't') {
            return TOP;
        }

        return null;
    }
}

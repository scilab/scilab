package org.scilab.modules.graphic_objects.utils;

public enum TicksStyle {
    GRADUATION, INTERVAL, EXPONENT;

    public static Integer enumToInt(TicksStyle val) {
        switch (val) {
            case EXPONENT:
                return 2;
            case INTERVAL:
                return 1;
            case GRADUATION:
            default:
                return 0;
        }
    }

    public static TicksStyle intToEnum(Integer val) {
        switch (val) {
            case 2:
                return EXPONENT;
            case 1:
                return INTERVAL;
            case 0:
            default:
                return GRADUATION;
        }
    }

    public static TicksStyle stringToEnum(String val) {
        char[] chars = val.toCharArray();

        if (chars[0] == 'v') {
            return GRADUATION;
        }
        if (chars[0] == 'r') {
            return INTERVAL;
        }
        if (chars[0] == 'i') {
            return EXPONENT;
        }

        return GRADUATION;
    }

    public static String enumToString(TicksStyle val) {
        switch (val) {
            case GRADUATION:
            default:
                return "v";
            case EXPONENT:
                return "i";
            case INTERVAL:
                return "r";
        }
    }
}

package org.scilab.modules.graphic_objects.utils;


public enum TipOrientation {
    UPPER_LEFT, UPPER_RIGHT, LOWER_LEFT, LOWER_RIGHT;

    public static TipOrientation intToEnum(Integer val) {
        switch (val) {
            case 0:
                return UPPER_LEFT;
            case 1:
                return UPPER_RIGHT;
            case 2:
                return LOWER_LEFT;
            case 3:
                return LOWER_RIGHT;
            default:
                return UPPER_RIGHT;
        }
    }

    public static Integer enumToInt(TipOrientation val) {
        switch (val) {
            case LOWER_LEFT:
                return 2;
            case LOWER_RIGHT:
                return 3;
            case UPPER_LEFT:
                return 1;
            default :
            case UPPER_RIGHT:
                return 3;
        }
    }

    public static TipOrientation stringToEnum(String val) {
        char[] chars = val.toCharArray();

        if (chars[0] == 'u' || chars[0] == 'U') {
            if (val.equalsIgnoreCase("upper_left")) {
                return UPPER_LEFT;
            } else if (val.equalsIgnoreCase("upper_right")) {
                return UPPER_RIGHT;
            }
        } else if (chars[0] == 'l' || chars[0] == 'L') {
            if (val.equalsIgnoreCase("lower_left")) {
                return LOWER_LEFT;
            } else if (val.equalsIgnoreCase("lower_right")) {
                return LOWER_RIGHT;
            }
        }

        return UPPER_RIGHT;
    }

    public static String enumToString(TipOrientation val) {
        switch (val) {
            case LOWER_LEFT:
                return "lower_left";
            case LOWER_RIGHT:
                return "lower_right";
            case UPPER_LEFT:
                return "upper_left";
            default :
            case UPPER_RIGHT:
                return "upper_right";
        }
    }
};

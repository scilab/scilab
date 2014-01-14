package org.scilab.modules.graphic_objects.utils;


public enum TipOrientation {
    TOP_LEFT, TOP_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT;

    public static TipOrientation intToEnum(Integer val) {
        switch (val) {
            case 0:
                return TOP_LEFT;
            case 1:
                return TOP_RIGHT;
            case 2:
                return BOTTOM_LEFT;
            case 3:
                return BOTTOM_RIGHT;
            default:
                return TOP_RIGHT;
        }
    }

    public static Integer enumToInt(TipOrientation val) {
        switch (val) {
            case BOTTOM_LEFT:
                return 2;
            case BOTTOM_RIGHT:
                return 3;
            case TOP_LEFT:
                return 1;
            default :
            case TOP_RIGHT:
                return 3;
        }
    }

    public static TipOrientation stringToEnum(String val) {
        char[] chars = val.toCharArray();

        if (chars[0] == 't' || chars[0] == 'T') {
            if (val.equalsIgnoreCase("top_left")) {
                return TOP_LEFT;
            } else if (val.equalsIgnoreCase("top_right")) {
                return TOP_RIGHT;
            }
        } else if (chars[0] == 'b' || chars[0] == 'B') {
            if (val.equalsIgnoreCase("bottom_left")) {
                return BOTTOM_LEFT;
            } else if (val.equalsIgnoreCase("bottom_right")) {
                return BOTTOM_RIGHT;
            }
        }

        return TOP_RIGHT;
    }

    public static String enumToString(TipOrientation val) {
        switch (val) {
            case BOTTOM_LEFT:
                return "bottom_left";
            case BOTTOM_RIGHT:
                return "bottom_right";
            case TOP_LEFT:
                return "top_left";
            default :
            case TOP_RIGHT:
                return "top_right";
        }
    }
};

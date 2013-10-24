package org.scilab.modules.graphic_objects.utils;

public enum TicksDirection {
    TOP, BOTTOM, LEFT, RIGHT;

    public static int enumToInt(TicksDirection val) {
        switch (val) {
            default :
            case TOP:
                return 0;
            case LEFT:
                return 2;
            case BOTTOM:
                return 1;
            case RIGHT:
                return 3;
        }
    }

    public static TicksDirection intToEnum(int val) {
        switch (val) {
            default :
            case 0:
                return TOP;
            case 2:
                return LEFT;
            case 1:
                return BOTTOM;
            case 3:
                return RIGHT;
        }
    }

    public static String enumToString(TicksDirection val) {
        switch (val) {
            default :
            case TOP:
                return "top";
            case LEFT:
                return "left";
            case BOTTOM:
                return "bottom";
            case RIGHT:
                return "right";
        }
    }

    public static TicksDirection stringToEnum(String val) {
        char[] chars = val.toCharArray();

        if (chars[0] == 't') {
            return TOP;
        }
        if (chars[0] == 'l') {
            return LEFT;
        }
        if (chars[0] == 'b') {
            return BOTTOM;
        }
        if (chars[0] == 'r') {
            return RIGHT;
        }

        return null;
    }
}

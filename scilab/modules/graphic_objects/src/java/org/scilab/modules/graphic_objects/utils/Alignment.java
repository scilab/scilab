package org.scilab.modules.graphic_objects.utils;

/** Alignment */
public enum Alignment {
    LEFT, CENTER, RIGHT;

    public static Integer enumToInt(Alignment value) {
        switch (value) {
            default:
            case LEFT:
                return 0;
            case CENTER:
                return 1;
            case RIGHT:
                return 2;
        }
    }

    public static Alignment intToEnum(Integer value) {
        switch (value) {
            case 0:
                return LEFT;
            case 1:
                return CENTER;
            case 2:
                return RIGHT;
            default:
                return null;
        }
    }

    public static String enumToString(Alignment value) {
        switch (value) {
            default:
            case LEFT:
                return "left";
            case CENTER:
                return "center";
            case RIGHT:
                return "right";
        }
    }

    public static Alignment stringToEnum(String value) {
        if (value.equalsIgnoreCase("left")) {
            return LEFT;
        }

        if (value.equalsIgnoreCase("center")) {
            return CENTER;
        }

        if (value.equalsIgnoreCase("right")) {
            return RIGHT;
        }

        return null;
    }
};

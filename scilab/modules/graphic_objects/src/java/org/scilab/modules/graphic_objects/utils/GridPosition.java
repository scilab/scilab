package org.scilab.modules.graphic_objects.utils;

public enum GridPosition {
    BACKGROUND, FOREGROUND;

    /**
     * Converts an integer to the corresponding enum
     * @param intValue the integer value
     * @return the grid position enum
     */
    public static GridPosition intToEnum(Integer val) {
        switch (val) {
            case 0:
                return GridPosition.BACKGROUND;
            case 1:
                return GridPosition.FOREGROUND;
            default:
                return null;
        }
    }

    public static Integer enumToInt(GridPosition val) {
        switch (val) {
            case BACKGROUND:
                return 0;
            case FOREGROUND:
                return 1;
        }
        return null;
    }

    public static String enumToString(GridPosition val) {
        switch (val) {
            case BACKGROUND:
                return "background";
            case FOREGROUND:
                return "foreground";
        }
        return null;
    }

    public static GridPosition stringToEnum(String val) {
        char[] chars = val.toCharArray();

        if (chars[0] == 'b') {
            return BACKGROUND;
        }
        if (chars[0] == 'f') {
            return FOREGROUND;
        }

        return null;
    }
};


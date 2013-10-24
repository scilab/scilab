package org.scilab.modules.graphic_objects.utils;

public enum BoxType {
    OFF, ON, HIDDEN_AXES, BACK_HALF;

    /**
     * Converts an integer to the corresponding enum
     * @param intValue the integer value
     * @return the box type enum
     */
    public static BoxType intToEnum(Integer val) {
        switch (val) {
            case 0:
                return BoxType.OFF;
            case 1:
                return BoxType.ON;
            case 2:
                return BoxType.HIDDEN_AXES;
            case 3:
                return BoxType.BACK_HALF;
            default:
                return null;
        }
    }

    public static Integer enumToInt(BoxType val) {
        switch (val) {
            case BACK_HALF:
                return 3;
            case HIDDEN_AXES:
                return 2;
            case ON:
                return 1;
            case OFF:
                return 0;
        }
        return null;
    }

    public static String enumToString(BoxType val) {
        switch (val) {
            case BACK_HALF:
                return "back_half";
            case HIDDEN_AXES:
                return "hidden_axes";
            case ON:
                return "on";
            case OFF:
                return "off";
        }
        return null;
    }

    public static BoxType stringToEnum(String val) {
        char[] chars = val.toCharArray();

        if (chars[0] == 'o') {
            if (chars.length == 2 && chars[1] == 'n') {
                return ON;
            }

            if (chars.length > 2 && chars[1] == 'f') {
                return OFF;
            }

            return null;
        }

        if (chars[0] == 'b') {
            return BACK_HALF;
        }

        if (chars[0] == 'h') {
            return HIDDEN_AXES;
        }

        return null;
    }
}

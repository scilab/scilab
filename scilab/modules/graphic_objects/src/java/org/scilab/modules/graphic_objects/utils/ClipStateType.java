package org.scilab.modules.graphic_objects.utils;

/** Indicates how clipping is performed */
public enum ClipStateType {
    OFF, CLIPGRF, ON;

    public static ClipStateType intToEnum(Integer val) {
        switch (val) {
            case 0:
                return ClipStateType.OFF;
            case 1:
                return ClipStateType.CLIPGRF;
            case 2:
                return ClipStateType.ON;
            default:
                return null;
        }
    }

    public static Integer enumToInt(ClipStateType val) {
        switch (val) {
            case CLIPGRF:
                return 1;
            case OFF:
                return 0;
            case ON:
                return 2;
            default:
                return null;
        }
    }

    public static ClipStateType stringToEnum(String val) {
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

        if (chars[0] == 'c') {
            return CLIPGRF;
        }

        return null;
    }

    public static String enumToString(ClipStateType val) {
        switch (val) {
            case CLIPGRF:
                return "clipgrf";
            case OFF:
                return "off";
            case ON:
                return "on";
            default:
                return null;
        }
    }
}

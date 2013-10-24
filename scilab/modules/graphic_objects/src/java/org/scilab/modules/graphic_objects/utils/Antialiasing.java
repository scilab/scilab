package org.scilab.modules.graphic_objects.utils;

public enum Antialiasing {
    OFF, TWO, FOUR, EIGHT, SIXTEEN;

    public static Integer enumToInt(Antialiasing val) {
        switch (val) {
            case EIGHT:
                return 3;
            case FOUR:
                return 2;
            case SIXTEEN:
                return 4;
            case TWO:
                return 1;
            default:
                return 0;
        }
    }

    public static Antialiasing intToEnum(Integer val) {
        switch (val) {
            case 1 :
                return TWO;
            case 2 :
                return FOUR;
            case 3 :
                return EIGHT;
            case 4 :
                return SIXTEEN;
            default :
                return OFF;
        }
    }

    public static Antialiasing stringToEnum(String val) {
        char[] chars = val.toCharArray();

        if (chars[0] == '2') {
            return TWO;
        }

        if (chars[0] == '4') {
            return FOUR;
        }

        if (chars[0] == '8') {
            return EIGHT;
        }

        if (chars[0] == '1') {
            return SIXTEEN;
        }

        return OFF;
    }

    public static String enumToString(Antialiasing val) {
        switch (val) {
            case EIGHT:
                return "8x";
            case FOUR:
                return "4x";
            case SIXTEEN:
                return "16x";
            case TWO:
                return "2x";
            default:
                return "off";
        }
    }
}

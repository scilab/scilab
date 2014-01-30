package org.scilab.modules.graphic_objects.uicontrol.frame.border;

public enum FrameBorderType {
    NONE, LINE, BEVEL, SOFTBEVEL, ETCHED, TITLED, EMPTY, COMPOUND, MATTE;

    public static FrameBorderType intToEnum(Integer value) {
        if (value == null) {
            return null;
        }

        switch (value) {
            default :
            case 0 :
                return NONE;
            case 1 :
                return LINE;
            case 2 :
                return BEVEL;
            case 3 :
                return SOFTBEVEL;
            case 4 :
                return ETCHED;
            case 5 :
                return TITLED;
            case 6 :
                return EMPTY;
            case 7 :
                return COMPOUND;
            case 8 :
                return MATTE;
        }
    }

    public static FrameBorderType stringToEnum(String value) {
        if (value == null || value.equals("")) {
            return null;
        }

        char[] chars = value.toCharArray();
        if (chars[0] == 'l' || chars[0] == 'L') {
            return LINE;
        }

        if (chars[0] == 'b' || chars[0] == 'B') {
            return BEVEL;
        }

        if (chars[0] == 's' || chars[0] == 'S') {
            return SOFTBEVEL;
        }

        if (chars[0] == 'e' || chars[0] == 'E') {
            if (chars[1] == 'm' || chars[1] == 'M') {
                return EMPTY;
            }
            return ETCHED;
        }

        if (chars[0] == 't' || chars[0] == 'T') {
            return TITLED;
        }

        if (chars[0] == 'c' || chars[0] == 'C') {
            return COMPOUND;
        }

        if (chars[0] == 'm' || chars[0] == 'M') {
            return MATTE;
        }

        return NONE;
    }
}

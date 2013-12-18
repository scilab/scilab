package org.scilab.modules.graphic_objects.utils;

/** Legend location */
public enum LegendLocation {
    IN_UPPER_RIGHT, IN_UPPER_LEFT, IN_LOWER_RIGHT, IN_LOWER_LEFT, OUT_UPPER_RIGHT, OUT_UPPER_LEFT, OUT_LOWER_RIGHT, OUT_LOWER_LEFT, UPPER_CAPTION, LOWER_CAPTION, BY_COORDINATES;

    public static LegendLocation intToEnum(Integer value) {
        switch (value) {
            case 0:
                return IN_UPPER_RIGHT;
            case 1:
                return IN_UPPER_LEFT;
            case 2:
                return IN_LOWER_RIGHT;
            case 3:
                return IN_LOWER_LEFT;
            case 4:
                return OUT_UPPER_RIGHT;
            case 5:
                return OUT_UPPER_LEFT;
            case 6:
                return OUT_LOWER_RIGHT;
            case 7:
                return OUT_LOWER_LEFT;
            case 8:
                return UPPER_CAPTION;
            case 9:
                return LOWER_CAPTION;
            case 10:
                return BY_COORDINATES;
            default:
                return null;
        }
    }

    public static Integer enumToInt(LegendLocation value) {
        switch (value) {
            default:
            case IN_UPPER_RIGHT:
                return 0;
            case IN_UPPER_LEFT:
                return 1;
            case IN_LOWER_RIGHT:
                return 2;
            case IN_LOWER_LEFT:
                return 3;
            case OUT_UPPER_RIGHT:
                return 4;
            case OUT_UPPER_LEFT:
                return 5;
            case OUT_LOWER_RIGHT:
                return 6;
            case OUT_LOWER_LEFT:
                return 7;
            case UPPER_CAPTION:
                return 8;
            case LOWER_CAPTION:
                return 9;
            case BY_COORDINATES:
                return 10;
        }
    }

    public static String enumToString(LegendLocation value) {
        switch (value) {
            default:
            case IN_UPPER_RIGHT:
                return "in_upper_right";
            case IN_UPPER_LEFT:
                return "in_upper_left";
            case IN_LOWER_RIGHT:
                return "in_lower_right";
            case IN_LOWER_LEFT:
                return "in_lower_left";
            case OUT_UPPER_RIGHT:
                return "out_upper_right";
            case OUT_UPPER_LEFT:
                return "out_upper_left";
            case OUT_LOWER_RIGHT:
                return "out_lower_right";
            case OUT_LOWER_LEFT:
                return "out_lower_left";
            case UPPER_CAPTION:
                return "upper_caption";
            case LOWER_CAPTION:
                return "lower_caption";
            case BY_COORDINATES:
                return "by_coordinates";
        }
    }

    public static LegendLocation stringToEnum(String value) {
        char[] chars = value.toCharArray();

        switch (chars[0]) {
            case 'i' :
                if (value.equals("in_lower_right")) {
                    return IN_LOWER_RIGHT;
                }

                if (value.equals("in_lower_left")) {
                    return IN_LOWER_LEFT;
                }

                if (value.equals("in_upper_right")) {
                    return IN_UPPER_RIGHT;
                }

                if (value.equals("in_upper_left")) {
                    return IN_UPPER_LEFT;
                }
            case 'o' :
                if (value.equals("out_lower_right")) {
                    return OUT_LOWER_RIGHT;
                }

                if (value.equals("out_lower_left")) {
                    return OUT_LOWER_LEFT;
                }

                if (value.equals("out_upper_right")) {
                    return OUT_UPPER_RIGHT;
                }

                if (value.equals("out_upper_left")) {
                    return OUT_UPPER_LEFT;
                }
            case 'b' :
                if (value.equals("by_coordinates")) {
                    return BY_COORDINATES;
                }
            case 'l' :
                if (value.equals("lower_caption")) {
                    return LOWER_CAPTION;
                }
            case 'u' :
                if (value.equals("upper_caption")) {
                    return UPPER_CAPTION;
                }
        }

        return IN_UPPER_RIGHT;
    }
};

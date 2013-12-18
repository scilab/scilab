package org.scilab.modules.graphic_objects.utils;

/** Text box mode */
public enum TextBoxMode {
    OFF, CENTERED, FILLED;

    public static Integer enumToInt(TextBoxMode value) {
        switch (value) {
            default:
            case OFF:
                return 0;
            case CENTERED:
                return 1;
            case FILLED:
                return 2;
        }
    }

    public static TextBoxMode intToEnum(Integer value) {
        switch (value) {
            case 0:
                return OFF;
            case 1:
                return CENTERED;
            case 2:
                return FILLED;
            default:
                return null;
        }
    }

    public static String enumToString(TextBoxMode value) {
        switch (value) {
            default:
            case OFF:
                return "off";
            case CENTERED:
                return "centered";
            case FILLED:
                return "filled";
        }
    }

    public static TextBoxMode stringToEnum(String value) {
        if (value.equalsIgnoreCase("off")) {
            return OFF;
        }

        if (value.equalsIgnoreCase("centered")) {
            return CENTERED;
        }

        if (value.equalsIgnoreCase("filled")) {
            return FILLED;
        }

        return null;
    }
};

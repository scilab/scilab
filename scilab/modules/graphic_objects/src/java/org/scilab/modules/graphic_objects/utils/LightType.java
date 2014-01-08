package org.scilab.modules.graphic_objects.utils;

public enum LightType {
    DIRECTIONAL, POINT;

    public static Integer enumToInt(LightType value) {
        switch (value) {
            case DIRECTIONAL:
                return 0;
            default :
            case POINT:
                return 1;
        }
    }

    public static LightType intToEnum(Integer value) {
        if (value == 1) {
            return DIRECTIONAL;
        }

        return POINT;
    }

    public static String enumToString(LightType value) {
        switch (value) {
            case DIRECTIONAL:
                return "directional";
            default:
            case POINT:
                return "point";
        }
    }

    public static LightType stringToEnum(String value) {
        if (value.equalsIgnoreCase("directional")) {
            return DIRECTIONAL;
        }

        return POINT;
    }
}

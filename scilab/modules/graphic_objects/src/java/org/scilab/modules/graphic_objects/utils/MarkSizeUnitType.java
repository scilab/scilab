package org.scilab.modules.graphic_objects.utils;

public enum MarkSizeUnitType {
    POINT, TABULATED;

    public static MarkSizeUnitType intToEnum(Integer val) {
        switch (val) {
            case 0:
                return POINT;
            case 1:
                return TABULATED;
        }
        return null;
    }

    public static Integer enumToInt(MarkSizeUnitType val) {
        switch (val) {
            case POINT:
                return 0;
            case TABULATED:
                return 1;
        }
        return null;
    }

    public static String enumToString(MarkSizeUnitType val) {
        switch (val) {
            case POINT:
                return "point";
            case TABULATED:
                return "tabulated";
        }
        return null;
    }

    public static MarkSizeUnitType stringToEnum(String val) {
        char[] chars = val.toCharArray();

        if (chars[0] == 'p') {
            return POINT;
        }

        if (chars[0] == 't') {
            return TABULATED;
        }

        return null;
    }
}

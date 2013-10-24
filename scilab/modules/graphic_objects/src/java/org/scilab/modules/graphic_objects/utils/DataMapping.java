package org.scilab.modules.graphic_objects.utils;

/** Data mapping */
public enum DataMapping {
    SCALED, DIRECT;

    public static DataMapping intToEnum(Integer val) {
        switch (val) {
            case 0:
                return DataMapping.SCALED;
            case 1:
                return DataMapping.DIRECT;
            default:
                return null;
        }
    }

    public static Integer enumToInt(DataMapping val) {
        switch (val) {
            case DIRECT:
                return 1;
            case SCALED:
                return 0;
            default:
                return null;
        }
    }

    public static String enumToString(DataMapping val) {
        switch (val) {
            case DIRECT:
                return "direct";
            case SCALED:
                return "scaled";
            default:
                return null;
        }
    }

    public static DataMapping stringToEnum(String val) {
        char[] chars = val.toCharArray();

        if (chars[0] == 's') {
            return DataMapping.SCALED;
        }
        if (chars[0] == 'd') {
            return DataMapping.DIRECT;
        }

        return null;
    }
}

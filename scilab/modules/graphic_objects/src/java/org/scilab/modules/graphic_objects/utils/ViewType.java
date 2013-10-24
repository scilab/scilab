package org.scilab.modules.graphic_objects.utils;

public enum ViewType {
    VIEW_2D, VIEW_3D;

    public static ViewType intToEnum(Integer val) {
        switch (val) {
            case 0:
                return ViewType.VIEW_2D;
            case 1:
                return ViewType.VIEW_3D;
            default:
                return null;
        }
    }

    public static Integer enumToInt(ViewType val) {
        switch (val) {
            case VIEW_2D:
                return 0;
            case VIEW_3D:
                return 1;
            default:
                return null;
        }
    }

    public static String enumToString(ViewType val) {
        switch (val) {
            case VIEW_2D:
                return "2d";
            case VIEW_3D:
                return "3d";
            default:
                return null;
        }
    }

    public static ViewType stringToEnum(String val) {
        char[] chars = val.toCharArray();

        if (chars[0] == '2') {
            return VIEW_2D;
        }

        if (chars[0] == '3') {
            return VIEW_3D;
        }

        return null;
    }
}

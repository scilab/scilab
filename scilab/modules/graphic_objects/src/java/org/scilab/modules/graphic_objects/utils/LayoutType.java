package org.scilab.modules.graphic_objects.utils;


public enum LayoutType {
    NONE, GRID, BORDER;

    public static LayoutType intToEnum(Integer intValue) {
        switch (intValue) {
            default:
            case 0:
                return LayoutType.NONE;
            case 1:
                return LayoutType.GRID;
            case 2:
                return LayoutType.BORDER;
        }
    }
}

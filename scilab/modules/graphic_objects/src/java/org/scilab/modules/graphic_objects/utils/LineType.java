package org.scilab.modules.graphic_objects.utils;

public enum LineType {
    SOLID, DASH, DASH_DOT, LONG_DASH_DOT, BIG_DASH_DOT, BIG_DASH_LONG_DASH, DOT, DOUBLE_DOT, LONG_BLANK_DOT, BIG_BLANK_DOT;

    /**
     * Converts a scilab line style index to the corresponding line type.
     * @param sciIndex the scilab index.
     * @return the line type as enum.
     */
    public static LineType intToEnum(Integer val) {
        switch (val) {
            default :
            case 1:
                return SOLID;
            case 2:
                return DASH;
            case 3:
                return DASH_DOT;
            case 4:
                return LONG_DASH_DOT;
            case 5:
                return BIG_DASH_DOT;
            case 6:
                return BIG_DASH_LONG_DASH;
            case 7:
                return DOT;
            case 8:
                return DOUBLE_DOT;
            case 9:
                return LONG_BLANK_DOT;
            case 10:
                return BIG_BLANK_DOT;
        }
    }

    /**
     * Converts the line type to the corresponding scilab line style index.
     * @return  the scilab line style index corresponding to this line type.
     */
    public static Integer enumToInt(LineType val) {
        switch (val) {
            case SOLID:
            default:
                return 1;
            case DASH:
                return 2;
            case DASH_DOT:
                return 3;
            case LONG_DASH_DOT:
                return 4;
            case BIG_DASH_DOT:
                return 5;
            case BIG_DASH_LONG_DASH:
                return 6;
            case DOT:
                return 7;
            case DOUBLE_DOT:
                return 8;
            case LONG_BLANK_DOT:
                return 9;
            case BIG_BLANK_DOT:
                return 10;
        }
    }

    /**
     * Converts the line type to a 16-bit pattern.
     * @return the 16-bit pattern corresponding to the line type.
     */
    public short asPattern() {
        switch (this) {
            case DASH:
                return (short) 0x07FF; // 5 blanks, 11 solids
            case DASH_DOT:
                return (short) 0x0F0F; // 4 blanks, 4 solids, 4 blanks, 4 solids
            case LONG_DASH_DOT:
                return (short) 0x1FC2; // 3 blanks, 3 solids, 3 blanks, 7 solids
            case BIG_DASH_DOT:
                return (short) 0x3FC9; // 2 blanks, 8 solids, 2 blanks, 1 solid, 2 blanks, 1 solid
            case BIG_DASH_LONG_DASH:
                return (short) 0x3FC6; // 3 blanks, 8 solids, 3 blanks, 2 solids
            case DOT:
                return (short) 0x5555; // (1 blank, 1 solid) x 8
            case DOUBLE_DOT:
                return (short) 0x3333; // (2 blanks, 2 solids) x 4
            case LONG_BLANK_DOT:
                return (short) 0x1111; // (3 blanks, 1 solids) x 4
            case BIG_BLANK_DOT:
                return (short) 0x0101; // (7 blanks, 1 solids) x 2
            default:
            case SOLID:
                return (short) 0xFFFF; // 16 solids, unused equivalent to no stipple
        }
    }
}


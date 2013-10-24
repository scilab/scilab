package org.scilab.modules.graphic_objects.utils;

public enum PixelDrawingMode {
    CLEAR, AND, ANDREVERSE, COPY, ANDINVERTED, NOOP, XOR, OR, NOR,
    EQUIV, INVERT, ORREVERSE, COPYINVERTED, ORINVERTED, NAND, SET;

    /**
     * Converts an integer to the corresponding enum
     * @param intValue the integer value
     * @return the pixel drawing mode enum
     */
    public static PixelDrawingMode intToEnum(Integer val) {
        switch (val) {
            case 0:
                return PixelDrawingMode.CLEAR;
            case 1:
                return PixelDrawingMode.AND;
            case 2:
                return PixelDrawingMode.ANDREVERSE;
            case 3:
                return PixelDrawingMode.COPY;
            case 4:
                return PixelDrawingMode.ANDINVERTED;
            case 5:
                return PixelDrawingMode.NOOP;
            case 6:
                return PixelDrawingMode.XOR;
            case 7:
                return PixelDrawingMode.OR;
            case 8:
                return PixelDrawingMode.NOR;
            case 9:
                return PixelDrawingMode.EQUIV;
            case 10:
                return PixelDrawingMode.INVERT;
            case 11:
                return PixelDrawingMode.ORREVERSE;
            case 12:
                return PixelDrawingMode.COPYINVERTED;
            case 13:
                return PixelDrawingMode.ORINVERTED;
            case 14:
                return PixelDrawingMode.NAND;
            case 15:
                return PixelDrawingMode.SET;
            default:
                return null;
        }
    }

    public static PixelDrawingMode stringToEnum(String val) {
        char[] chars = val.toCharArray();

        switch (chars[0]) {
            case 'a':
                //check input is longer than the shorter string
                if (chars.length < 3) {
                    break;
                }

                if (chars.length == 3) {
                    return AND;
                }

                if (chars[3] == 'R') {
                    return ANDREVERSE;
                }

                if (chars[3] == 'I') {
                    return ANDINVERTED;
                }
                break;
            case 'c':
                if (chars.length < 4) {
                    break;
                }

                if (chars.length == 4) {
                    return COPY;
                }

                if (chars[4] == 'r') {
                    return CLEAR;
                }

                if (chars[4] == 'I') {
                    return COPYINVERTED;
                }
                break;
            case 'e':
                if (chars.length < 5) {
                    break;
                }
                return EQUIV;
            case 'i':
                if (chars.length < 6) {
                    break;
                }
                return INVERT;
            case 'n':
                if (chars.length < 3) {
                    break;
                }

                if (chars[4] == 'o') {
                    return NOOP;
                }

                if (chars[4] == 'r') {
                    return NOR;
                }

                if (chars[4] == 'n') {
                    return NAND;
                }

                break;
            case 'o':
                if (chars.length < 2) {
                    break;
                }

                if (chars.length == 2) {
                    return OR;
                }

                if (chars[4] == 'R') {
                    return ORREVERSE;
                }

                if (chars[4] == 'I') {
                    return ORINVERTED;
                }

                break;
            case 's':
                if (chars.length < 3) {
                    break;
                }
                return SET;
            case 'x':
                if (chars.length < 3) {
                    break;
                }
                return XOR;
            default:
                break;
        }

        return null;
    }

    public static String enumToString(PixelDrawingMode val) {
        switch (val) {
            case AND:
                return("and");
            case ANDINVERTED:
                return("andInverted");
            case ANDREVERSE:
                return("andReverse");
            case CLEAR:
                return("clear");
            case COPY:
                return("copy");
            case COPYINVERTED:
                return("copyInverted");
            case EQUIV:
                return("equiv");
            case INVERT:
                return("invert");
            case NAND:
                return("nand");
            case NOOP:
                return("noop");
            case NOR:
                return("nor");
            case OR:
                return("or");
            case ORINVERTED:
                return("orInverted");
            case ORREVERSE:
                return("orReverse");
            case SET:
                return("set");
            case XOR:
                return("xor");
            default:
                return("");
        }
    }
};

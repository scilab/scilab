/*
 * Scilab (http://www.scilab.org/) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.types;

/**
 * Class to put Java data on Scilab stack at the given stack position.
 * There is a lot of convenient methods to put directly Java data on Scilab stack.
 * Since Scilab is single threaded, all these methods are not thread safe and could lead
 * to errors with multi-threaded use.
 *
 */
public final class ScilabStackPutter {

    private final static double[][][] singleDouble = new double[][][] {{{ 0 }}};
    private final static double[][][] rowDouble = new double[][][] {{ null }};
    private final static double[][][] matDouble = new double[][][] { null };
    private final static double[][][] singleComplex = new double[][][] {{{ 0 }}, {{ 0 }}};
    private final static double[][][] rowComplex = new double[][][] {{ null }, { null }};
    private final static double[][][] matComplex = new double[][][] { null, null };
    private final static Object[] singleByte = new Object[] {new int[]{ 0 }, new byte[][]{{ (byte)0 }}};
    private final static Object[] rowByte = new Object[] {new int[]{ 0 }, new byte[][]{ null }};
    private final static Object[] matByte = new Object[] {new int[]{ 0 }, null};
    private final static Object[] singleShort = new Object[] {new int[]{ 0 }, new short[][]{{ (short)0 }}};
    private final static Object[] rowShort = new Object[] {new int[]{ 0 }, new short[][]{ null }};
    private final static Object[] matShort = new Object[] {new int[]{ 0 }, null};
    private final static Object[] singleInt = new Object[] {new int[]{ 0 }, new int[][]{{ 0 }}};
    private final static Object[] rowInt = new Object[] {new int[]{ 0 }, new int[][]{ null }};
    private final static Object[] matInt = new Object[] {new int[]{ 0 }, null};
    private final static Object[] singleLong = new Object[] {new int[]{ 0 }, new long[][]{{ (long)0 }}};
    private final static Object[] rowLong = new Object[] {new int[]{ 0 }, new long[][]{ null }};
    private final static Object[] matLong = new Object[] {new int[]{ 0 }, null};
    private final static String[][] singleString = new String[][] {{ "" }};
    private final static String[][] rowString = new String[][] { null };
    private final static boolean[][] singleBoolean = new boolean[][] {{ true }};
    private final static boolean[][] rowBoolean = new boolean[][] { null };

    /**
     * Put an empty matrix to given stack position
     * @param stackPos position
     * @return true if the operation succeeded.
     */
    public static final boolean putEmpty(final int stackPos) {
        return put(stackPos, new double[0], false);
    }

    /**
     * Put a ScilabType at the given position in the Scilab stack
     * @param stackPos position
     * @param var the variable
     * @return true if the operation succeeded.
     */
    public static final boolean put(final int stackPos, final ScilabType var) {
        return var == null ? putEmpty(stackPos) : GetScilabVariableJNI.putVarOnStack(stackPos, var.getSerializedObject(), var.getType().swigValue(), var.isSwaped()) == 0;
    }

    /**
     * Put a double at the given position in the Scilab stack
     * @param stackPos position
     * @param d the variable
     * @return true if the operation succeeded
     */
    public static final boolean put(final int stackPos, final double d) {
        singleDouble[0][0][0] = d;
        return GetScilabVariableJNI.putVarOnStack(stackPos, singleDouble, ScilabTypeEnum.sci_matrix.swigValue(), false) == 0;
    }

    /**
     * Put a double array at the given position in the Scilab stack
     * @param stackPos position
     * @param d the variable
     * @param swap if true d will be transposed
     * @return true if the operation succeeded
     */
    public static final boolean put(final int stackPos, final double[] d, final boolean swap) {
        rowDouble[0][0] = d;
        return d == null ? putEmpty(stackPos) : GetScilabVariableJNI.putVarOnStack(stackPos, rowDouble, ScilabTypeEnum.sci_matrix.swigValue(), swap) == 0;
    }

    /**
     * Put a double matrix at the given position in the Scilab stack
     * @param stackPos position
     * @param d the variable
     * @param swap if true d will be transposed
     * @return true if the operation succeeded
     */
    public static final boolean put(final int stackPos, final double[][] d, final boolean swap) {
        matDouble[0] = d;
        return d == null ? putEmpty(stackPos) : GetScilabVariableJNI.putVarOnStack(stackPos, matDouble, ScilabTypeEnum.sci_matrix.swigValue(), swap) == 0;
    }

    /**
     * Put a complex at the given position in the Scilab stack
     * @param stackPos position
     * @param r the real part
     * @param c the imag part
     * @return true if the operation succeeded
     */
    public static final boolean put(final int stackPos, final double r, final double c) {
        singleComplex[0][0][0] = r;
        singleComplex[0][0][1] = c;
        return GetScilabVariableJNI.putVarOnStack(stackPos, singleComplex, ScilabTypeEnum.sci_matrix.swigValue(), false) == 0;
    }

    /**
     * Put a complex array at the given position in the Scilab stack
     * @param stackPos position
     * @param r the real part
     * @param c the imag part
     * @param swap if true [r+%i*c] will be transposed
     * @return true if the operation succeeded
     */
    public static final boolean put(final int stackPos, final double[] r, final double[] c, final boolean swap) {
        rowComplex[0][0] = r;
        rowComplex[0][1] = c;
        return (r == null || c == null) ? putEmpty(stackPos) : GetScilabVariableJNI.putVarOnStack(stackPos, rowComplex, ScilabTypeEnum.sci_matrix.swigValue(), swap) == 0;
    }

    /**
     * Put a complex matrix at the given position in the Scilab stack
     * @param stackPos position
     * @param r the real part
     * @param c the imag part
     * @param swap if true [r+%i*c] will be transposed
     * @return true if the operation succeeded
     */
    public static final boolean put(final int stackPos, final double[][] r, final double[][] c, final boolean swap) {
        matComplex[0] = r;
        matComplex[1] = c;
        return (r == null || c == null) ? putEmpty(stackPos) : GetScilabVariableJNI.putVarOnStack(stackPos, matComplex, ScilabTypeEnum.sci_matrix.swigValue(), swap) == 0;
    }

    /**
     * Put a byte at the given position in the Scilab stack
     * @param stackPos position
     * @param b the variable
     * @return true if the operation succeeded
     */
    public static final boolean put(final int stackPos, final byte b, final boolean unsigned) {
        ((int[]) singleByte[0])[0] = unsigned ? ScilabIntegerTypeEnum.sci_uint8.swigValue() : ScilabIntegerTypeEnum.sci_int8.swigValue();
        ((byte[][]) singleByte[1])[0][0] = b;
        return GetScilabVariableJNI.putVarOnStack(stackPos, singleByte, ScilabTypeEnum.sci_ints.swigValue(), false) == 0;
    }

    /**
     * Put a byte array at the given position in the Scilab stack
     * @param stackPos position
     * @param b the variable
     * @param swap if true d will be transposed
     * @return true if the operation succeeded
     */
    public static final boolean put(final int stackPos, final byte[] b, final boolean unsigned, final boolean swap) {
        ((int[]) rowByte[0])[0] = unsigned ? ScilabIntegerTypeEnum.sci_uint8.swigValue() : ScilabIntegerTypeEnum.sci_int8.swigValue();
        ((byte[][]) rowByte[1])[0] = b;
        return b == null ? putEmpty(stackPos) : GetScilabVariableJNI.putVarOnStack(stackPos, rowByte, ScilabTypeEnum.sci_ints.swigValue(), swap) == 0;
    }

    /**
     * Put a byte matrix at the given position in the Scilab stack
     * @param stackPos position
     * @param b the variable
     * @param swap if true d will be transposed
     * @return true if the operation succeeded
     */
    public static final boolean put(final int stackPos, final byte[][] b, final boolean unsigned, final boolean swap) {
        ((int[]) matByte[0])[0] = unsigned ? ScilabIntegerTypeEnum.sci_uint8.swigValue() : ScilabIntegerTypeEnum.sci_int8.swigValue();
        matByte[1] = b;
        return b == null ? putEmpty(stackPos) : GetScilabVariableJNI.putVarOnStack(stackPos, matByte, ScilabTypeEnum.sci_ints.swigValue(), swap) == 0;
    }

    /**
     * Put a short at the given position in the Scilab stack
     * @param stackPos position
     * @param s the variable
     * @return true if the operation succeeded
     */
    public static final boolean put(final int stackPos, final short s, final boolean unsigned) {
        ((int[]) singleShort[0])[0] = unsigned ? ScilabIntegerTypeEnum.sci_uint16.swigValue() : ScilabIntegerTypeEnum.sci_int16.swigValue();
        ((short[][]) singleShort[1])[0][0] = s;
        return GetScilabVariableJNI.putVarOnStack(stackPos, singleShort, ScilabTypeEnum.sci_ints.swigValue(), false) == 0;
    }

    /**
     * Put a short array at the given position in the Scilab stack
     * @param stackPos position
     * @param s the variable
     * @param swap if true d will be transposed
     * @return true if the operation succeeded
     */
    public static final boolean put(final int stackPos, final short[] s, final boolean unsigned, final boolean swap) {
        ((int[]) rowShort[0])[0] = unsigned ? ScilabIntegerTypeEnum.sci_uint16.swigValue() : ScilabIntegerTypeEnum.sci_int16.swigValue();
        ((short[][]) rowShort[1])[0] = s;
        return s == null ? putEmpty(stackPos) : GetScilabVariableJNI.putVarOnStack(stackPos, rowShort, ScilabTypeEnum.sci_ints.swigValue(), swap) == 0;
    }

    /**
     * Put a short matrix at the given position in the Scilab stack
     * @param stackPos position
     * @param s the variable
     * @param swap if true d will be transposed
     * @return true if the operation succeeded
     */
    public static final boolean put(final int stackPos, final short[][] s, final boolean unsigned, final boolean swap) {
        ((int[]) matShort[0])[0] = unsigned ? ScilabIntegerTypeEnum.sci_uint16.swigValue() : ScilabIntegerTypeEnum.sci_int16.swigValue();
        matShort[1] = s;
        return s == null ? putEmpty(stackPos) : GetScilabVariableJNI.putVarOnStack(stackPos, matShort, ScilabTypeEnum.sci_ints.swigValue(), swap) == 0;
    }

    /**
     * Put a int at the given position in the Scilab stack
     * @param stackPos position
     * @param i the variable
     * @return true if the operation succeeded
     */
    public static final boolean put(final int stackPos, final int i, final boolean unsigned) {
        ((int[]) singleInt[0])[0] = unsigned ? ScilabIntegerTypeEnum.sci_uint32.swigValue() : ScilabIntegerTypeEnum.sci_int32.swigValue();
        ((int[][]) singleInt[1])[0][0] = i;
        return GetScilabVariableJNI.putVarOnStack(stackPos, singleInt, ScilabTypeEnum.sci_ints.swigValue(), false) == 0;
    }

    /**
     * Put a int array at the given position in the Scilab stack
     * @param stackPos position
     * @param i the variable
     * @param swap if true d will be transposed
     * @return true if the operation succeeded
     */
    public static final boolean put(final int stackPos, final int[] i, final boolean unsigned, final boolean swap) {
        ((int[]) rowInt[0])[0] = unsigned ? ScilabIntegerTypeEnum.sci_uint32.swigValue() : ScilabIntegerTypeEnum.sci_int32.swigValue();
        ((int[][]) rowInt[1])[0] = i;
        return i == null ? putEmpty(stackPos) : GetScilabVariableJNI.putVarOnStack(stackPos, rowInt, ScilabTypeEnum.sci_ints.swigValue(), swap) == 0;
    }

    /**
     * Put a int matrix at the given position in the Scilab stack
     * @param stackPos position
     * @param i the variable
     * @param swap if true d will be transposed
     * @return true if the operation succeeded
     */
    public static final boolean put(final int stackPos, final int[][] i, final boolean unsigned, final boolean swap) {
        ((int[]) matInt[0])[0] = unsigned ? ScilabIntegerTypeEnum.sci_uint32.swigValue() : ScilabIntegerTypeEnum.sci_int32.swigValue();
        matInt[1] = i;
        return i == null ? putEmpty(stackPos) : GetScilabVariableJNI.putVarOnStack(stackPos, matInt, ScilabTypeEnum.sci_ints.swigValue(), swap) == 0;
    }

    /**
     * Put a long at the given position in the Scilab stack
     * @param stackPos position
     * @param l the variable
     * @return true if the operation succeeded
     */
    public static final boolean put(final int stackPos, final long l, final boolean unsigned) {
        ((int[]) singleLong[0])[0] = unsigned ? ScilabIntegerTypeEnum.sci_uint64.swigValue() : ScilabIntegerTypeEnum.sci_int64.swigValue();
        ((long[][]) singleLong[1])[0][0] = l;
        return GetScilabVariableJNI.putVarOnStack(stackPos, singleLong, ScilabTypeEnum.sci_ints.swigValue(), false) == 0;
    }

    /**
     * Put a long array at the given position in the Scilab stack
     * @param stackPos position
     * @param l the variable
     * @param swap if true d will be transposed
     * @return true if the operation succeeded
     */
    public static final boolean put(final int stackPos, final long[] l, final boolean unsigned, final boolean swap) {
        ((int[]) rowLong[0])[0] = unsigned ? ScilabIntegerTypeEnum.sci_uint64.swigValue() : ScilabIntegerTypeEnum.sci_int64.swigValue();
        ((long[][]) rowLong[1])[0] = l;
        return l == null ? putEmpty(stackPos) : GetScilabVariableJNI.putVarOnStack(stackPos, rowLong, ScilabTypeEnum.sci_ints.swigValue(), swap) == 0;
    }

    /**
     * Put a long matrix at the given position in the Scilab stack
     * @param stackPos position
     * @param l the variable
     * @param swap if true d will be transposed
     * @return true if the operation succeeded
     */
    public static final boolean put(final int stackPos, final long[][] l, final boolean unsigned, final boolean swap) {
        ((int[]) matLong[0])[0] = unsigned ? ScilabIntegerTypeEnum.sci_uint64.swigValue() : ScilabIntegerTypeEnum.sci_int64.swigValue();
        matLong[1] = l;
        return l == null ? putEmpty(stackPos) : GetScilabVariableJNI.putVarOnStack(stackPos, matLong, ScilabTypeEnum.sci_ints.swigValue(), swap) == 0;
    }

    /**
     * Put a string at the given position in the Scilab stack
     * @param stackPos position
     * @param s the variable
     * @return true if the operation succeeded
     */
    public static final boolean put(final int stackPos, final String s) {
        singleString[0][0] = s;
        return s == null ? putEmpty(stackPos) : GetScilabVariableJNI.putVarOnStack(stackPos, singleString, ScilabTypeEnum.sci_strings.swigValue(), false) == 0;
    }

    /**
     * Put a string array at the given position in the Scilab stack
     * @param stackPos position
     * @param s the variable
     * @param swap if true d will be transposed
     * @return true if the operation succeeded
     */
    public static final boolean put(final int stackPos, final String[] s, final boolean swap) {
        rowString[0] = s;
        return s == null ? putEmpty(stackPos) : GetScilabVariableJNI.putVarOnStack(stackPos, rowString, ScilabTypeEnum.sci_strings.swigValue(), swap) == 0;
    }

    /**
     * Put a string matrix at the given position in the Scilab stack
     * @param stackPos position
     * @param s the variable
     * @param swap if true d will be transposed
     * @return true if the operation succeeded
     */
    public static final boolean put(final int stackPos, final String[][] s, final boolean swap) {
        return s == null ? putEmpty(stackPos) : GetScilabVariableJNI.putVarOnStack(stackPos, s, ScilabTypeEnum.sci_strings.swigValue(), swap) == 0;
    }

    /**
     * Put a boolean at the given position in the Scilab stack
     * @param stackPos position
     * @param b the variable
     * @return true if the operation succeeded
     */
    public static final boolean put(final int stackPos, final boolean b) {
        singleBoolean[0][0] = b;
        return GetScilabVariableJNI.putVarOnStack(stackPos, singleBoolean, ScilabTypeEnum.sci_boolean.swigValue(), false) == 0;
    }

    /**
     * Put a boolean array at the given position in the Scilab stack
     * @param stackPos position
     * @param b the variable
     * @param swap if true d will be transposed
     * @return true if the operation succeeded
     */
    public static final boolean put(final int stackPos, final boolean[] b, final boolean swap) {
        rowBoolean[0] = b;
        return b == null ? putEmpty(stackPos) : GetScilabVariableJNI.putVarOnStack(stackPos, rowBoolean, ScilabTypeEnum.sci_boolean.swigValue(), swap) == 0;
    }

    /**
     * Put a boolean matrix at the given position in the Scilab stack
     * @param stackPos position
     * @param b the variable
     * @param swap if true d will be transposed
     * @return true if the operation succeeded
     */
    public static final boolean put(final int stackPos, final boolean[][] b, final boolean swap) {
        return b == null ? putEmpty(stackPos) : GetScilabVariableJNI.putVarOnStack(stackPos, b, ScilabTypeEnum.sci_boolean.swigValue(), swap) == 0;
    }
}

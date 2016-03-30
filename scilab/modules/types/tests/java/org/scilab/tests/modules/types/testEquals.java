/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Sylvestre LEDRU
 * Copyright (C) 2011 - Scilab Enterprises - Sylvestre LEDRU
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */
package org.scilab.tests.modules.types;

import static org.junit.Assert.assertTrue;

import java.util.Arrays;

import org.junit.Test;
import org.scilab.modules.types.ScilabBoolean;
import org.scilab.modules.types.ScilabDouble;
import org.scilab.modules.types.ScilabInteger;
import org.scilab.modules.types.ScilabIntegerTypeEnum;
import org.scilab.modules.types.ScilabList;
import org.scilab.modules.types.ScilabMList;
import org.scilab.modules.types.ScilabSparse;
import org.scilab.modules.types.ScilabString;
import org.scilab.modules.types.ScilabTList;
import org.scilab.modules.types.ScilabTypeEnum;

public class testEquals {

    @Test
    public void compareDoubleTest() throws NullPointerException {
        double[][] a = { { 21.2, 22.0, 42.0, 39.0 }, { 23.2, 24.0, 44.0, 40.0 } };
        double[][] b = { { 21.2, 22.0, 42.0, 39.0 }, { 23.2, 24.0, 44.0, 40.0 } };
        double[][] c = { { 42, 43 }, { 21, 22 } };
        ScilabDouble aMatrix = new ScilabDouble(a);
        ScilabDouble bMatrix = new ScilabDouble(b);
        ScilabDouble cMatrix = new ScilabDouble(c);
        assertTrue(aMatrix.equals(bMatrix) == true);
        assertTrue(bMatrix.equals(aMatrix) == true);
        assertTrue(cMatrix.equals(aMatrix) == false);
        assertTrue(Arrays.deepEquals(aMatrix.getRealPart(), bMatrix.getRealPart()) == true);
        assertTrue(aMatrix.toString().equals("[21.2, 22.0, 42.0, 39.0 ; 23.2, 24.0, 44.0, 40.0]") == true);
        assertTrue(cMatrix.toString().equals("[42.0, 43.0 ; 21.0, 22.0]") == true);
        assertTrue(aMatrix.getType() == ScilabTypeEnum.sci_matrix);
        assertTrue(bMatrix.getType() == ScilabTypeEnum.sci_matrix);
        assertTrue(cMatrix.getType() == ScilabTypeEnum.sci_matrix);
    }

    @Test
    public void compareSparseTest() throws NullPointerException {
        double[][] a = { { 21.2, 22.0, 42.0, 39.0 }, { 23.2, 24.0, 44.0, 40.0 } };
        double[][] b = { { 21.2, 22.0, 42.0, 39.0 }, { 23.2, 24.0, 44.0, 40.0 } };
        double[][] c = { { 42, 43 }, { 21, 22 } };
        ScilabSparse aMatrix = new ScilabSparse(a);
        ScilabSparse bMatrix = new ScilabSparse(b);
        ScilabSparse cMatrix = new ScilabSparse(c);
        ScilabSparse matrixComplex = new ScilabSparse(a, b);
        assertTrue(aMatrix.equals(bMatrix) == true);
        assertTrue(bMatrix.equals(aMatrix) == true);
        assertTrue(cMatrix.equals(aMatrix) == false);
        assertTrue(Arrays.deepEquals(aMatrix.getFullRealPart(), bMatrix.getFullRealPart()) == true);
        assertTrue(aMatrix.toString().equals(
                       "sparse([1, 1 ; 1, 2 ; 1, 3 ; 1, 4 ; 2, 1 ; 2, 2 ; 2, 3 ; 2, 4], [21.2 ; 22.0 ; 42.0 ; 39.0 ; 23.2 ; 24.0 ; 44.0 ; 40.0], [2, 4])") == true);
        assertTrue(cMatrix.toString().equals("sparse([1, 1 ; 1, 2 ; 2, 1 ; 2, 2], [42.0 ; 43.0 ; 21.0 ; 22.0], [2, 2])") == true);
        assertTrue(matrixComplex
                   .toString()
                   .equals("sparse([1, 1 ; 1, 2 ; 1, 3 ; 1, 4 ; 2, 1 ; 2, 2 ; 2, 3 ; 2, 4], [21.2+21.2*%i ; 22.0+22.0*%i ; 42.0+42.0*%i ; 39.0+39.0*%i ; 23.2+23.2*%i ; 24.0+24.0*%i ; 44.0+44.0*%i ; 40.0+40.0*%i], [2, 4])"));
        assertTrue(aMatrix.getType() == ScilabTypeEnum.sci_sparse);
        assertTrue(bMatrix.getType() == ScilabTypeEnum.sci_sparse);
        assertTrue(cMatrix.getType() == ScilabTypeEnum.sci_sparse);
    }

    @Test
    public void doubleMatrixTests() {
        double[][] a = { { 21.2, 22.0, 42.0, 39.0 }, { 23.2, 24.0, 44.0, 40.0 } };
        ScilabDouble emptyMatrix = new ScilabDouble();
        assertTrue(emptyMatrix.getHeight() == 0);
        assertTrue(emptyMatrix.getWidth() == 0);
        assertTrue(emptyMatrix.isEmpty() == true);
        assertTrue(emptyMatrix.toString().equals("[]") == true);
        emptyMatrix.setRealPart(a);
        emptyMatrix.setImaginaryPart(a);
        assertTrue(emptyMatrix.getType() == ScilabTypeEnum.sci_matrix);

        ScilabBoolean aMatrix = new ScilabBoolean(true);
        assertTrue(aMatrix.equals(emptyMatrix) == false);
        assertTrue(emptyMatrix.equals(aMatrix) == false);
        assertTrue(aMatrix.getType() != ScilabTypeEnum.sci_matrix);
    }

    @Test
    public void compareDoubleComplexTest() throws NullPointerException {
        double[][] a = { { 21.2, 22.0, 42.0, 39.0 }, { 23.2, 24.0, 44.0, 40.0 } };
        double[][] aImg = { { 210.2, 220.0, 420.0, 390.0 }, { 230.2, 240.0, 440.0, 400.0 } };
        double[][] b = { { 21.2, 22.0, 42.0, 39.0 }, { 23.2, 24.0, 44.0, 40.0 } };
        double[][] bImg = { { 210.2, 220.0, 420.0, 390.0 }, { 230.2, 240.0, 440.0, 400.0 } };
        double[][] c = { { 42, 43 }, { 21, 22 } };
        double[][] cImg = { { 420, 430 }, { 210, 220 } };
        ScilabDouble aMatrix = new ScilabDouble(a, aImg);
        ScilabDouble bMatrix = new ScilabDouble(b, bImg);
        ScilabDouble cMatrix = new ScilabDouble(c, cImg);
        assertTrue(aMatrix.equals(bMatrix) == true);
        assertTrue(bMatrix.equals(aMatrix) == true);
        assertTrue(cMatrix.equals(aMatrix) == false);
        assertTrue(aMatrix.getType() == ScilabTypeEnum.sci_matrix);
        assertTrue(bMatrix.getType() == ScilabTypeEnum.sci_matrix);
        assertTrue(cMatrix.getType() == ScilabTypeEnum.sci_matrix);

        assertTrue(Arrays.deepEquals(aMatrix.getRealPart(), bMatrix.getRealPart()) == true);
        assertTrue(Arrays.deepEquals(aMatrix.getImaginaryPart(), bMatrix.getImaginaryPart()) == true);
        assertTrue(aMatrix
                   .toString()
                   .equals("[21.2 + 210.2 * %i, 22.0 + 220.0 * %i, 42.0 + 420.0 * %i, 39.0 + 390.0 * %i ; 23.2 + 230.2 * %i, 24.0 + 240.0 * %i, 44.0 + 440.0 * %i, 40.0 + 400.0 * %i]") == true);
        assertTrue(cMatrix.toString().equals("[42.0 + 420.0 * %i, 43.0 + 430.0 * %i ; 21.0 + 210.0 * %i, 22.0 + 220.0 * %i]") == true);

        double[] result = new double[] { 21.2, 23.2, 22.0, 24.0, 42.0, 44.0, 39.0, 40.0, 210.2, 230.2, 220.0, 240.0, 420.0, 440.0, 390.0, 400.0 };
        assertTrue(aMatrix.getSerializedComplexMatrix().length == result.length);
        assertTrue(Arrays.equals(aMatrix.getSerializedComplexMatrix(), result) == true);
        ScilabDouble scalarComplex = new ScilabDouble(1, 2);
        assertTrue(aMatrix.equals(scalarComplex) == false);
        assertTrue(aMatrix.equals(bMatrix) == true);
    }

    @Test
    public void sparseTest() throws NullPointerException {
        double[][] a = { { 21.2, 22.0, 42.0, 39.0 }, { 23.2, 24.0, 44.0, 40.0 } };
        double[][] aImg = { { 210.2, 220.0, 420.0, 390.0 }, { 230.2, 240.0, 440.0, 400.0 } };
        double[][] b = { { 21.2, 22.0, 42.0, 39.0 }, { 23.2, 24.0, 44.0, 40.0 } };
        double[][] bImg = { { 210.2, 220.0, 420.0, 390.0 }, { 230.2, 240.0, 440.0, 400.0 } };
        double[][] c = { { 42, 43 }, { 21, 22 } };
        double[][] cImg = { { 420, 430 }, { 210, 220 } };
        ScilabSparse aMatrix = new ScilabSparse(a, aImg);
        ScilabSparse bMatrix = new ScilabSparse(b, bImg);
        ScilabSparse cMatrix = new ScilabSparse(c, cImg);
        assertTrue(aMatrix.equals(bMatrix) == true);
        assertTrue(bMatrix.equals(aMatrix) == true);
        assertTrue(cMatrix.equals(aMatrix) == false);
        assertTrue(aMatrix.getType() == ScilabTypeEnum.sci_sparse);
        assertTrue(bMatrix.getType() == ScilabTypeEnum.sci_sparse);
        assertTrue(cMatrix.getType() == ScilabTypeEnum.sci_sparse);

        assertTrue(Arrays.deepEquals(aMatrix.getFullRealPart(), bMatrix.getFullRealPart()) == true);
        assertTrue(Arrays.deepEquals(aMatrix.getFullImaginaryPart(), bMatrix.getFullImaginaryPart()) == true);
        assertTrue(aMatrix
                   .toString()
                   .equals("sparse([1, 1 ; 1, 2 ; 1, 3 ; 1, 4 ; 2, 1 ; 2, 2 ; 2, 3 ; 2, 4], [21.2+210.2*%i ; 22.0+220.0*%i ; 42.0+420.0*%i ; 39.0+390.0*%i ; 23.2+230.2*%i ; 24.0+240.0*%i ; 44.0+440.0*%i ; 40.0+400.0*%i], [2, 4])"));
        assertTrue(cMatrix.toString().equals("sparse([1, 1 ; 1, 2 ; 2, 1 ; 2, 2], [42.0+420.0*%i ; 43.0+430.0*%i ; 21.0+210.0*%i ; 22.0+220.0*%i], [2, 2])"));

        ScilabSparse mySparse = new ScilabSparse(100, 100, 5, new int[] { 1, 1, 1, 1, 1 }, new int[] { 1, 25, 50, 75, 99 }, new double[] { 1.0, 2.0, 3.0, 4.0,
                5.0
                                                                                                                                         });
        String ref = "sparse([1, 2 ; 2, 26 ; 3, 51 ; 4, 76 ; 5, 100], [1.0 ; 2.0 ; 3.0 ; 4.0 ; 5.0], [100, 100])";
        assertTrue(mySparse.toString().equals(ref));
    }

    @Test
    public void compareInteger8Test() throws NullPointerException {
        byte[][] a = { { 32, 42, 41 }, { 12, 13, 32 } };
        byte[][] b = { { 32, 42, 41 }, { 12, 13, 32 } };
        byte[][] c = { { 42, 43 }, { 21, 22 } };
        ScilabInteger aMatrix = new ScilabInteger(a, true);
        ScilabInteger bMatrix = new ScilabInteger(b, true);
        ScilabInteger cMatrix = new ScilabInteger(c, true);
        ScilabInteger dMatrix = new ScilabInteger(c, false);
        assertTrue(aMatrix.equals(bMatrix) == true);
        assertTrue(bMatrix.equals(aMatrix) == true);
        assertTrue(cMatrix.equals(aMatrix) == false);
        assertTrue(cMatrix.equals(dMatrix) == true);
        assertTrue(aMatrix.getType() == ScilabTypeEnum.sci_ints);
        assertTrue(bMatrix.getType() == ScilabTypeEnum.sci_ints);
        assertTrue(cMatrix.getType() == ScilabTypeEnum.sci_ints);

        assertTrue(Arrays.deepEquals(aMatrix.getData(), bMatrix.getData()) == true);
        assertTrue(aMatrix.toString().equals("uint8([32, 42, 41 ; 12, 13, 32])") == true);
        assertTrue(cMatrix.toString().equals("uint8([42, 43 ; 21, 22])") == true);
        assertTrue(dMatrix.toString().equals("int8([42, 43 ; 21, 22])") == true);
        assertTrue(aMatrix.getDataAsByte().length == a.length);
        assertTrue(aMatrix.getDataAsByte()[0].length == a[0].length);
        byte[][] d = aMatrix.getDataAsByte();
        long[][] d2 = aMatrix.getData();
        assertTrue(d[0][0] == 32);
        assertTrue(d2[0][0] == 32);
        assertTrue(Arrays.deepEquals(a, d));
        assertTrue(aMatrix.getPrec() == ScilabIntegerTypeEnum.sci_uint8);
        assertTrue(dMatrix.getPrec() == ScilabIntegerTypeEnum.sci_int8);
        assertTrue(aMatrix.isUnsigned() == true);
        assertTrue(dMatrix.isUnsigned() == false);
        assertTrue(ScilabInteger.convertOldType("TYPE8", true) == ScilabIntegerTypeEnum.sci_uint8);
        assertTrue(ScilabInteger.convertOldType("TYPE8", false) == ScilabIntegerTypeEnum.sci_int8);
    }

    @Test
    public void integer8MatrixTests() {
        byte[][] a = { { 21, 22, 42, 39 }, { 23, 24, 44, 40 } };
        ScilabInteger emptyMatrix = new ScilabInteger();
        assertTrue(emptyMatrix.getHeight() == 0);
        assertTrue(emptyMatrix.getWidth() == 0);
        assertTrue(emptyMatrix.isEmpty() == true);
        assertTrue(emptyMatrix.toString().equals("[]") == true);
        assertTrue(emptyMatrix.getType() == ScilabTypeEnum.sci_ints);
        emptyMatrix.setData(a, true);
        assertTrue(emptyMatrix.getType() == ScilabTypeEnum.sci_ints);

        ScilabBoolean aMatrix = new ScilabBoolean(true);
        assertTrue(aMatrix.equals(emptyMatrix) == false);
        assertTrue(emptyMatrix.equals(aMatrix) == false);
        ScilabInteger scalarInteger = new ScilabInteger((byte) 2);
    }

    @Test
    public void compareInteger16Test() throws NullPointerException {
        short[][] a = { { 32, 42, 41 }, { 12, 13, 32 } };
        short[][] b = { { 32, 42, 41 }, { 12, 13, 32 } };
        short[][] c = { { 42, 43 }, { 21, 22 } };
        ScilabInteger aMatrix = new ScilabInteger(a, true);
        ScilabInteger bMatrix = new ScilabInteger(b, true);
        ScilabInteger cMatrix = new ScilabInteger(c, true);
        ScilabInteger dMatrix = new ScilabInteger(c, false);
        assertTrue(aMatrix.equals(bMatrix) == true);
        assertTrue(bMatrix.equals(aMatrix) == true);
        assertTrue(cMatrix.equals(aMatrix) == false);
        assertTrue(cMatrix.equals(dMatrix) == true);
        assertTrue(aMatrix.getType() == ScilabTypeEnum.sci_ints);
        assertTrue(bMatrix.getType() == ScilabTypeEnum.sci_ints);
        assertTrue(cMatrix.getType() == ScilabTypeEnum.sci_ints);

        assertTrue(Arrays.deepEquals(aMatrix.getData(), bMatrix.getData()) == true);
        assertTrue(aMatrix.toString().equals("uint16([32, 42, 41 ; 12, 13, 32])") == true);
        assertTrue(cMatrix.toString().equals("uint16([42, 43 ; 21, 22])") == true);
        assertTrue(dMatrix.toString().equals("int16([42, 43 ; 21, 22])") == true);
        assertTrue(aMatrix.getDataAsShort().length == a.length);
        assertTrue(aMatrix.getDataAsShort()[0].length == a[0].length);
        short[][] d = aMatrix.getDataAsShort();
        long[][] d2 = aMatrix.getData();
        assertTrue(d[0][0] == 32);
        assertTrue(d2[0][0] == 32);
        assertTrue(Arrays.deepEquals(a, d));
        assertTrue(aMatrix.getPrec() == ScilabIntegerTypeEnum.sci_uint16);
        assertTrue(dMatrix.getPrec() == ScilabIntegerTypeEnum.sci_int16);
        assertTrue(aMatrix.isUnsigned() == true);
        assertTrue(dMatrix.isUnsigned() == false);
        assertTrue(ScilabInteger.convertOldType("TYPE16", true) == ScilabIntegerTypeEnum.sci_uint16);
        assertTrue(ScilabInteger.convertOldType("TYPE16", false) == ScilabIntegerTypeEnum.sci_int16);
    }

    @Test
    public void integer16MatrixTests() {
        short[][] a = { { 21, 22, 42, 39 }, { 23, 24, 44, 40 } };
        ScilabInteger emptyMatrix = new ScilabInteger();
        assertTrue(emptyMatrix.getType() == ScilabTypeEnum.sci_ints);
        emptyMatrix.setData(a, true);
        assertTrue(emptyMatrix.getType() == ScilabTypeEnum.sci_ints);

        ScilabBoolean aMatrix = new ScilabBoolean(true);
        assertTrue(aMatrix.equals(emptyMatrix) == false);
        assertTrue(emptyMatrix.equals(aMatrix) == false);
        ScilabInteger scalarInteger = new ScilabInteger((short) 2);
    }

    @Test
    public void compareInteger32Test() throws NullPointerException {
        int[][] a = { { 32, 42, 41 }, { 12, 13, 32 } };
        int[][] b = { { 32, 42, 41 }, { 12, 13, 32 } };
        int[][] c = { { 42, 43 }, { 21, 22 } };
        ScilabInteger aMatrix = new ScilabInteger(a, true);
        ScilabInteger bMatrix = new ScilabInteger(b, true);
        ScilabInteger cMatrix = new ScilabInteger(c, true);
        ScilabInteger dMatrix = new ScilabInteger(c, false);
        assertTrue(aMatrix.equals(bMatrix) == true);
        assertTrue(bMatrix.equals(aMatrix) == true);
        assertTrue(cMatrix.equals(aMatrix) == false);
        assertTrue(cMatrix.equals(dMatrix) == true);
        assertTrue(aMatrix.getType() == ScilabTypeEnum.sci_ints);
        assertTrue(bMatrix.getType() == ScilabTypeEnum.sci_ints);
        assertTrue(cMatrix.getType() == ScilabTypeEnum.sci_ints);

        assertTrue(Arrays.deepEquals(aMatrix.getData(), bMatrix.getData()) == true);
        assertTrue(aMatrix.toString().equals("uint32([32, 42, 41 ; 12, 13, 32])") == true);
        assertTrue(cMatrix.toString().equals("uint32([42, 43 ; 21, 22])") == true);
        assertTrue(dMatrix.toString().equals("int32([42, 43 ; 21, 22])") == true);
        assertTrue(aMatrix.getDataAsInt().length == a.length);
        assertTrue(aMatrix.getDataAsInt()[0].length == a[0].length);
        int[][] d = aMatrix.getDataAsInt();
        long[][] d2 = aMatrix.getData();
        assertTrue(d[0][0] == 32);
        assertTrue(d2[0][0] == 32);
        assertTrue(Arrays.deepEquals(a, d));
        assertTrue(aMatrix.getPrec() == ScilabIntegerTypeEnum.sci_uint32);
        assertTrue(dMatrix.getPrec() == ScilabIntegerTypeEnum.sci_int32);
        assertTrue(aMatrix.isUnsigned() == true);
        assertTrue(dMatrix.isUnsigned() == false);
        assertTrue(ScilabInteger.convertOldType("TYPE32", true) == ScilabIntegerTypeEnum.sci_uint32);
        assertTrue(ScilabInteger.convertOldType("TYPE32", false) == ScilabIntegerTypeEnum.sci_int32);
    }

    @Test
    public void integer32MatrixTests() {
        int[][] a = { { 21, 22, 42, 39 }, { 23, 24, 44, 40 } };
        ScilabInteger emptyMatrix = new ScilabInteger();
        assertTrue(emptyMatrix.getType() == ScilabTypeEnum.sci_ints);
        emptyMatrix.setData(a, true);
        assertTrue(emptyMatrix.getType() == ScilabTypeEnum.sci_ints);

        ScilabBoolean aMatrix = new ScilabBoolean(true);
        assertTrue(aMatrix.equals(emptyMatrix) == false);
        assertTrue(emptyMatrix.equals(aMatrix) == false);
        ScilabInteger scalarInteger = new ScilabInteger((int) 2);
    }

    @Test
    public void compareInteger64Test() throws NullPointerException {
        long[][] a = { { 32, 42, 41 }, { 12, 13, 32 } };
        long[][] b = { { 32, 42, 41 }, { 12, 13, 32 } };
        long[][] c = { { 42, 43 }, { 21, 22 } };
        ScilabInteger aMatrix = new ScilabInteger(a, true);
        ScilabInteger bMatrix = new ScilabInteger(b, true);
        ScilabInteger cMatrix = new ScilabInteger(c, true);
        ScilabInteger dMatrix = new ScilabInteger(c, false);
        assertTrue(aMatrix.equals(bMatrix) == true);
        assertTrue(bMatrix.equals(aMatrix) == true);
        assertTrue(cMatrix.equals(aMatrix) == false);
        assertTrue(cMatrix.equals(dMatrix) == true);
        assertTrue(aMatrix.getType() == ScilabTypeEnum.sci_ints);
        assertTrue(bMatrix.getType() == ScilabTypeEnum.sci_ints);
        assertTrue(cMatrix.getType() == ScilabTypeEnum.sci_ints);

        assertTrue(Arrays.deepEquals(aMatrix.getData(), bMatrix.getData()) == true);
        assertTrue(aMatrix.toString().equals("uint64([32, 42, 41 ; 12, 13, 32])") == true);
        assertTrue(cMatrix.toString().equals("uint64([42, 43 ; 21, 22])") == true);
        assertTrue(dMatrix.toString().equals("int64([42, 43 ; 21, 22])") == true);
        assertTrue(aMatrix.getDataAsLong().length == a.length);
        assertTrue(aMatrix.getDataAsLong()[0].length == a[0].length);
        long[][] d = aMatrix.getDataAsLong();
        long[][] d2 = aMatrix.getData();
        assertTrue(d[0][0] == 32);
        assertTrue(d2[0][0] == 32);
        assertTrue(Arrays.deepEquals(a, d));
        assertTrue(aMatrix.getPrec() == ScilabIntegerTypeEnum.sci_uint64);
        assertTrue(dMatrix.getPrec() == ScilabIntegerTypeEnum.sci_int64);
        assertTrue(aMatrix.isUnsigned() == true);
        assertTrue(dMatrix.isUnsigned() == false);
        assertTrue(ScilabInteger.convertOldType("TYPE64", true) == ScilabIntegerTypeEnum.sci_uint64);
        assertTrue(ScilabInteger.convertOldType("TYPE64", false) == ScilabIntegerTypeEnum.sci_int64);
    }

    @Test
    public void integer64MatrixTests() {
        long[][] a = { { 21, 22, 42, 39 }, { 23, 24, 44, 40 } };
        ScilabInteger emptyMatrix = new ScilabInteger();
        assertTrue(emptyMatrix.getType() == ScilabTypeEnum.sci_ints);
        emptyMatrix.setData(a, true);
        assertTrue(emptyMatrix.getType() == ScilabTypeEnum.sci_ints);

        ScilabBoolean aMatrix = new ScilabBoolean(true);
        assertTrue(aMatrix.equals(emptyMatrix) == false);
        assertTrue(emptyMatrix.equals(aMatrix) == false);
        ScilabInteger scalarInteger = new ScilabInteger((long) 2);
    }

    @Test
    public void compareBooleanTest() throws NullPointerException {
        boolean[][] a = { { true, false, true }, { true, true, true } };
        boolean[][] b = { { true, false, true }, { true, true, true } };
        boolean[][] c = { { true, false }, { false, true } };
        ScilabBoolean aMatrix = new ScilabBoolean(a);
        ScilabBoolean bMatrix = new ScilabBoolean(b);
        ScilabBoolean cMatrix = new ScilabBoolean(c);
        assertTrue(aMatrix.equals(bMatrix) == true);
        assertTrue(bMatrix.equals(aMatrix) == true);
        assertTrue(cMatrix.equals(aMatrix) == false);
        assertTrue(aMatrix.getType() == ScilabTypeEnum.sci_boolean);
        assertTrue(bMatrix.getType() == ScilabTypeEnum.sci_boolean);
        assertTrue(cMatrix.getType() == ScilabTypeEnum.sci_boolean);

        assertTrue(Arrays.deepEquals(aMatrix.getData(), bMatrix.getData()) == true);
        assertTrue(aMatrix.toString().equals("[%t, %f, %t ; %t, %t, %t]") == true);
        assertTrue(cMatrix.toString().equals("[%t, %f ; %f, %t]") == true);
    }

    @Test
    public void booleanMatrixTests() {
        boolean[][] a = { { true, false, true }, { true, true, true } };
        ScilabBoolean emptyMatrix = new ScilabBoolean();
        assertTrue(emptyMatrix.getHeight() == 0);
        assertTrue(emptyMatrix.getWidth() == 0);
        assertTrue(emptyMatrix.isEmpty() == true);
        assertTrue(emptyMatrix.toString().equals("[]") == true);
        assertTrue(emptyMatrix.getType() == ScilabTypeEnum.sci_boolean);
        emptyMatrix.setData(a);
        assertTrue(emptyMatrix.getType() == ScilabTypeEnum.sci_boolean);

        ScilabDouble aMatrix = new ScilabDouble(2);
        assertTrue(aMatrix.equals(emptyMatrix) == false);
        assertTrue(emptyMatrix.equals(aMatrix) == false);
    }

    @Test
    public void compareStringTest() throws NullPointerException {
        String[][] a = { { "This", "is", "my", "string" }, { "and", "I want to", "compare", " them" } };
        String[][] b = { { "This", "is", "my", "string" }, { "and", "I want to", "compare", " them" } };
        String[][] c = { { "Wrong", "string" }, { "right", "string" } };
        ScilabString aMatrix = new ScilabString(a);
        ScilabString bMatrix = new ScilabString(b);
        ScilabString cMatrix = new ScilabString(c);
        assertTrue(aMatrix.equals(bMatrix) == true);
        assertTrue(bMatrix.equals(aMatrix) == true);
        assertTrue(cMatrix.equals(aMatrix) == false);
        assertTrue(aMatrix.getType() == ScilabTypeEnum.sci_strings);
        assertTrue(bMatrix.getType() == ScilabTypeEnum.sci_strings);
        assertTrue(cMatrix.getType() == ScilabTypeEnum.sci_strings);

        assertTrue(Arrays.deepEquals(aMatrix.getData(), bMatrix.getData()) == true);
        assertTrue(aMatrix.toString().equals("[\"This\", \"is\", \"my\", \"string\" ; \"and\", \"I want to\", \"compare\", \" them\"]") == true);
        assertTrue(cMatrix.toString().equals("[\"Wrong\", \"string\" ; \"right\", \"string\"]") == true);

    }

    @Test(expected = IllegalArgumentException.class)
    public void stringMatrixTests() {
        String[][] a = { { "This", "is", "my", "string" }, { "and", "I want to", "compare", " them" } };
        ScilabString emptyMatrix = new ScilabString();
        assertTrue(emptyMatrix.getHeight() == 0);
        assertTrue(emptyMatrix.getWidth() == 0);
        assertTrue(emptyMatrix.isEmpty() == true);
        assertTrue(emptyMatrix.toString().equals("[]") == true);
        assertTrue(emptyMatrix.getType() == ScilabTypeEnum.sci_strings);
        emptyMatrix.setData(a);
        assertTrue(emptyMatrix.getType() == ScilabTypeEnum.sci_strings);
        ScilabDouble aMatrix = new ScilabDouble(2);
        assertTrue(aMatrix.equals(emptyMatrix) == false);
        assertTrue(emptyMatrix.equals(aMatrix) == false);
        String[] b = { "This", "is", "my", "string" };
        ScilabString vectorString = new ScilabString(b);
        String[] c = null;
        ScilabString emptyString = new ScilabString(c);
        ScilabString nullString = new ScilabString((String) null);

    }

    @Test
    public void compareListTest() throws NullPointerException {
        ScilabList data = new ScilabList();
        assertTrue(data.getType() == ScilabTypeEnum.sci_list);
        assertTrue(data.getHeight() == 0);
        assertTrue(data.getWidth() == 0);
        assertTrue(data.toString().equals("list()"));
        data.add(new ScilabDouble(2));
        assertTrue(data.getHeight() == 1);
        assertTrue(data.getWidth() == 1);
        data.add(new ScilabDouble(51));
        assertTrue(data.getHeight() == 1);
        assertTrue(data.getWidth() == 2);
        data.add(new ScilabString("hello"));
        ScilabList data2 = new ScilabList();
        data2.add(new ScilabDouble(2));
        data2.add(new ScilabDouble(51));
        data2.add(new ScilabString("hello"));
        ScilabList data3 = new ScilabList();
        data3.add(new ScilabDouble(2));
        data3.add(new ScilabDouble(42));
        data3.add(new ScilabBoolean(true));
        data3.add(new ScilabString("hello"));
        assertTrue(data.equals(data2) == true);
        assertTrue(data.equals(data3) == false);
        assertTrue(data.toString().equals("list([2.0], [51.0], [\"hello\"])") == true);
        assertTrue(data3.toString().equals("list([2.0], [42.0], [%t], [\"hello\"])") == true);

        ScilabList data4 = new ScilabList(data3);
        assertTrue(data4.equals(data3) == true);
        assertTrue(data4.getType() == ScilabTypeEnum.sci_list);

    }

    @Test
    public void compareTListTest() throws NullPointerException {
        ScilabTList data = new ScilabTList();
        assertTrue(data.getType() == ScilabTypeEnum.sci_tlist);
        assertTrue(data.getHeight() == 0);
        assertTrue(data.getWidth() == 0);
        assertTrue(data.toString().equals("tlist()"));
        data.add(new ScilabString("hello"));
        assertTrue(data.getHeight() == 1);
        assertTrue(data.getWidth() == 1);
        data.add(new ScilabDouble(2));
        assertTrue(data.getHeight() == 1);
        assertTrue(data.getWidth() == 2);
        data.add(new ScilabDouble(51));
        assertTrue(data.getHeight() == 1);
        assertTrue(data.getWidth() == 3);
        ScilabTList data2 = new ScilabTList();
        data2.add(new ScilabString("hello"));
        data2.add(new ScilabDouble(2));
        data2.add(new ScilabDouble(51));
        ScilabTList data3 = new ScilabTList();
        data3.add(new ScilabString("hello"));
        data3.add(new ScilabDouble(2));
        data3.add(new ScilabDouble(42));
        data3.add(new ScilabBoolean(true));
        assertTrue(data.equals(data2) == true);
        assertTrue(data.equals(data3) == false);

        assertTrue(data.toString().equals("tlist([\"hello\"], [2.0], [51.0])") == true);
        assertTrue(data3.toString().equals("tlist([\"hello\"], [2.0], [42.0], [%t])") == true);
        String[] b = { "a", "b", "c" };

        ScilabTList tlist = new ScilabTList(b);
        tlist.add(new ScilabDouble(2));
        tlist.add(new ScilabDouble(3));
        assertTrue(tlist.toString().equals("tlist([\"a\", \"b\", \"c\"], [2.0], [3.0])") == true);
        ScilabTList tlistFromAnOther = new ScilabTList(b, data2);
        assertTrue(tlistFromAnOther.getHeight() == 1);
        assertTrue(tlistFromAnOther.getWidth() == 4);
        assertTrue(tlistFromAnOther.toString().equals("tlist([\"a\", \"b\", \"c\"], [\"hello\"], [2.0], [51.0])"));
        assertTrue(tlist.getType() == ScilabTypeEnum.sci_tlist);
    }

    @Test
    public void compareMListTest() throws NullPointerException {
        ScilabMList data = new ScilabMList();
        assertTrue(data.getType() == ScilabTypeEnum.sci_mlist);
        assertTrue(data.getHeight() == 0);
        assertTrue(data.getWidth() == 0);
        assertTrue(data.toString().equals("mlist()"));
        data.add(new ScilabString("hello"));
        assertTrue(data.getHeight() == 1);
        assertTrue(data.getWidth() == 1);
        data.add(new ScilabDouble(2));
        assertTrue(data.getHeight() == 1);
        assertTrue(data.getWidth() == 2);
        data.add(new ScilabDouble(51));
        assertTrue(data.getHeight() == 1);
        assertTrue(data.getWidth() == 3);
        ScilabMList data2 = new ScilabMList();
        data2.add(new ScilabString("hello"));
        data2.add(new ScilabDouble(2));
        data2.add(new ScilabDouble(51));
        ScilabMList data3 = new ScilabMList();
        data3.add(new ScilabString("hello"));
        data3.add(new ScilabDouble(2));
        data3.add(new ScilabDouble(42));
        data3.add(new ScilabBoolean(true));
        assertTrue(data3.getType() == ScilabTypeEnum.sci_mlist);
        assertTrue(data.equals(data2) == true);
        assertTrue(data.equals(data3) == false);
        assertTrue(data.toString().equals("mlist([\"hello\"], [2.0], [51.0])") == true);
        assertTrue(data3.toString().equals("mlist([\"hello\"], [2.0], [42.0], [%t])") == true);
        String[] b = { "a", "b", "c" };

        ScilabMList mlist = new ScilabMList(b);
        mlist.add(new ScilabDouble(2));
        mlist.add(new ScilabDouble(3));
        assertTrue(mlist.toString().equals("mlist([\"a\", \"b\", \"c\"], [2.0], [3.0])") == true);
        ScilabMList mlistFromAnOther = new ScilabMList(b, data2);
        assertTrue(mlistFromAnOther.getHeight() == 1);
        assertTrue(mlistFromAnOther.getWidth() == 4);
        assertTrue(mlistFromAnOther.toString().equals("mlist([\"a\", \"b\", \"c\"], [\"hello\"], [2.0], [51.0])"));

    }

}

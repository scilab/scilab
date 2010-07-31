/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Sylvestre LEDRU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.tests.modules.javasci;

import org.testng.annotations.*;

import java.util.Arrays;

import org.scilab.modules.types.scilabTypes.ScilabType;
import org.scilab.modules.types.scilabTypes.ScilabDouble;
import org.scilab.modules.types.scilabTypes.ScilabInteger;
import org.scilab.modules.types.scilabTypes.ScilabBoolean;
import org.scilab.modules.types.scilabTypes.ScilabString;
import org.scilab.modules.types.scilabTypes.ScilabList;

public class testEquals {

	@Test
    public void compareDoubleTest() throws NullPointerException {
        double [][]a={{21.2, 22.0, 42.0, 39.0},{23.2, 24.0, 44.0, 40.0}};
        double [][]b={{21.2, 22.0, 42.0, 39.0},{23.2, 24.0, 44.0, 40.0}};
        double [][]c={{42,43},{21,22}};
        ScilabDouble aMatrix = new ScilabDouble(a);
        ScilabDouble bMatrix = new ScilabDouble(b);
        ScilabDouble cMatrix = new ScilabDouble(c);
        assert aMatrix.equals(bMatrix) == true;
        assert bMatrix.equals(aMatrix) == true;
        assert cMatrix.equals(aMatrix) == false;
        assert Arrays.deepEquals(aMatrix.getRealPart(), bMatrix.getRealPart()) == true;
		assert aMatrix.toString().equals("[21.2, 22.0, 42.0, 39.0 ; 23.2, 24.0, 44.0, 40.0]") == true;
		assert cMatrix.toString().equals("[42.0, 43.0 ; 21.0, 22.0]") == true;
    }


	@Test
    public void compareDoubleComplexTest() throws NullPointerException {
        double [][]a={{21.2, 22.0, 42.0, 39.0},{23.2, 24.0, 44.0, 40.0}};
        double [][]aImg={{210.2, 220.0, 420.0, 390.0},{230.2, 240.0, 440.0, 400.0}};
        double [][]b={{21.2, 22.0, 42.0, 39.0},{23.2, 24.0, 44.0, 40.0}};
        double [][]bImg={{210.2, 220.0, 420.0, 390.0},{230.2, 240.0, 440.0, 400.0}};
        double [][]c={{42,43},{21,22}};
        double [][]cImg={{420,430},{210,220}};
        ScilabDouble aMatrix = new ScilabDouble(a, aImg);
        ScilabDouble bMatrix = new ScilabDouble(b, bImg);
        ScilabDouble cMatrix = new ScilabDouble(c, cImg);
        assert aMatrix.equals(bMatrix) == true;
        assert bMatrix.equals(aMatrix) == true;
        assert cMatrix.equals(aMatrix) == false;
        assert Arrays.deepEquals(aMatrix.getRealPart(), bMatrix.getRealPart()) == true;
        assert Arrays.deepEquals(aMatrix.getImaginaryPart(), bMatrix.getImaginaryPart()) == true;
		assert aMatrix.toString().equals("[21.2 + 210.2 * %i, 22.0 + 220.0 * %i, 42.0 + 420.0 * %i, 39.0 + 390.0 * %i ; 23.2 + 230.2 * %i, 24.0 + 240.0 * %i, 44.0 + 440.0 * %i, 40.0 + 400.0 * %i]") == true;
		assert cMatrix.toString().equals("[42.0 + 420.0 * %i, 43.0 + 430.0 * %i ; 21.0 + 210.0 * %i, 22.0 + 220.0 * %i]") == true;

    }


	@Test
    public void compareInteger8Test() throws NullPointerException {
        byte [][]a={{32,42,41}, {12,13,32}};
        byte [][]b={{32,42,41}, {12,13,32}};
        byte [][]c={{42,43},{21,22}};
        ScilabInteger aMatrix = new ScilabInteger(a, true);
        ScilabInteger bMatrix = new ScilabInteger(b, true);
        ScilabInteger cMatrix = new ScilabInteger(c, true);
        ScilabInteger dMatrix = new ScilabInteger(c, false);
        assert aMatrix.equals(bMatrix) == true;
        assert bMatrix.equals(aMatrix) == true;
        assert cMatrix.equals(aMatrix) == false;
        assert cMatrix.equals(dMatrix) == true;
        assert Arrays.deepEquals(aMatrix.getData(), bMatrix.getData()) == true;
		assert aMatrix.toString().equals("uint8([32, 42, 41 ; 12, 13, 32])") == true;
		assert cMatrix.toString().equals("uint8([42, 43 ; 21, 22])") == true;
		assert dMatrix.toString().equals("int8([42, 43 ; 21, 22])") == true;
    }


	@Test
    public void compareInteger16Test() throws NullPointerException {
        short [][]a={{32,42,41}, {12,13,32}};
        short [][]b={{32,42,41}, {12,13,32}};
        short [][]c={{42,43},{21,22}};
        ScilabInteger aMatrix = new ScilabInteger(a, true);
        ScilabInteger bMatrix = new ScilabInteger(b, true);
        ScilabInteger cMatrix = new ScilabInteger(c, true);
        ScilabInteger dMatrix = new ScilabInteger(c, false);
        assert aMatrix.equals(bMatrix) == true;
        assert bMatrix.equals(aMatrix) == true;
        assert cMatrix.equals(aMatrix) == false;
        assert cMatrix.equals(dMatrix) == true;
        assert Arrays.deepEquals(aMatrix.getData(), bMatrix.getData()) == true;
		assert aMatrix.toString().equals("uint16([32, 42, 41 ; 12, 13, 32])") == true;
		assert cMatrix.toString().equals("uint16([42, 43 ; 21, 22])") == true;
		assert dMatrix.toString().equals("int16([42, 43 ; 21, 22])") == true;
    }

	@Test
    public void compareInteger32Test() throws NullPointerException {
        int [][]a={{32,42,41}, {12,13,32}};
        int [][]b={{32,42,41}, {12,13,32}};
        int [][]c={{42,43},{21,22}};
        ScilabInteger aMatrix = new ScilabInteger(a, true);
        ScilabInteger bMatrix = new ScilabInteger(b, true);
        ScilabInteger cMatrix = new ScilabInteger(c, true);
        ScilabInteger dMatrix = new ScilabInteger(c, false);
        assert aMatrix.equals(bMatrix) == true;
        assert bMatrix.equals(aMatrix) == true;
        assert cMatrix.equals(aMatrix) == false;
        assert cMatrix.equals(dMatrix) == true;
        assert Arrays.deepEquals(aMatrix.getData(), bMatrix.getData()) == true;
		assert aMatrix.toString().equals("uint32([32, 42, 41 ; 12, 13, 32])") == true;
		assert cMatrix.toString().equals("uint32([42, 43 ; 21, 22])") == true;
		assert dMatrix.toString().equals("int32([42, 43 ; 21, 22])") == true;
    }

	@Test
    public void compareInteger64Test() throws NullPointerException {
        long [][]a={{32,42,41}, {12,13,32}};
        long [][]b={{32,42,41}, {12,13,32}};
        long [][]c={{42,43},{21,22}};
        ScilabInteger aMatrix = new ScilabInteger(a, true);
        ScilabInteger bMatrix = new ScilabInteger(b, true);
        ScilabInteger cMatrix = new ScilabInteger(c, true);
        ScilabInteger dMatrix = new ScilabInteger(c, false);
        assert aMatrix.equals(bMatrix) == true;
        assert bMatrix.equals(aMatrix) == true;
        assert cMatrix.equals(aMatrix) == false;
        assert cMatrix.equals(dMatrix) == true;
        assert Arrays.deepEquals(aMatrix.getData(), bMatrix.getData()) == true;
		assert aMatrix.toString().equals("uint64([32, 42, 41 ; 12, 13, 32])") == true;
		assert cMatrix.toString().equals("uint64([42, 43 ; 21, 22])") == true;
		assert dMatrix.toString().equals("int64([42, 43 ; 21, 22])") == true;
    }


	@Test
    public void compareBooleanTest() throws NullPointerException {
        boolean [][]a={{true,false,true}, {true,true,true}};
        boolean [][]b={{true,false,true}, {true,true,true}};
        boolean [][]c={{true,false},{false,true}};
        ScilabBoolean aMatrix = new ScilabBoolean(a);
        ScilabBoolean bMatrix = new ScilabBoolean(b);
        ScilabBoolean cMatrix = new ScilabBoolean(c);
        assert aMatrix.equals(bMatrix) == true;
        assert bMatrix.equals(aMatrix) == true;
        assert cMatrix.equals(aMatrix) == false;
        assert Arrays.deepEquals(aMatrix.getData(), bMatrix.getData()) == true;
        assert aMatrix.toString().equals("[%t, %f, %t ; %t, %t, %t]") == true;
        assert cMatrix.toString().equals("[%t, %f ; %f, %t]") == true;
    }

	@Test
    public void compareStringTest() throws NullPointerException {
        String [][]a={{"This","is","my","string"},{"and","I want to", "compare"," them"}};
        String [][]b={{"This","is","my","string"},{"and","I want to", "compare"," them"}};
        String [][]c={{"Wrong","string"},{"right","string"}};
        ScilabString aMatrix = new ScilabString(a);
        ScilabString bMatrix = new ScilabString(b);
        ScilabString cMatrix = new ScilabString(c);
        assert aMatrix.equals(bMatrix) == true;
        assert bMatrix.equals(aMatrix) == true;
        assert cMatrix.equals(aMatrix) == false;
        assert Arrays.deepEquals(aMatrix.getData(), bMatrix.getData()) == true;
        assert aMatrix.toString().equals("[\"This\", \"is\", \"my\", \"string\" ; \"and\", \"I want to\", \"compare\", \" them\"]") == true;
		assert cMatrix.toString().equals("[\"Wrong\", \"string\" ; \"right\", \"string\"]") == true;

    }

	@Test
    public void compareListTest() throws NullPointerException {
        ScilabList data = new ScilabList();
        data.add(new ScilabDouble(2));
        data.add(new ScilabDouble(51));
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
        assert data.equals(data2) == true;
        assert data.equals(data3) == false;
		assert data.toString().equals("list([2.0],\n[51.0],\n[\"hello\"])") == true;
		assert data3.toString().equals("list([2.0],\n[42.0],\n[%t],\n[\"hello\"])") == true;

    }
}

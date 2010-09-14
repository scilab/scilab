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
package org.scilab.tests.modules.types;

import org.testng.annotations.*;

import java.util.Arrays;

import org.scilab.modules.types.ScilabType;
import org.scilab.modules.types.ScilabDouble;
import org.scilab.modules.types.ScilabInteger;
import org.scilab.modules.types.ScilabIntegerTypeEnum;
import org.scilab.modules.types.ScilabBoolean;
import org.scilab.modules.types.ScilabString;
import org.scilab.modules.types.ScilabList;
import org.scilab.modules.types.ScilabTList;
import org.scilab.modules.types.ScilabMList;

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
	public void doubleMatrixTests() {
		double [][]a={{21.2, 22.0, 42.0, 39.0},{23.2, 24.0, 44.0, 40.0}};
		ScilabDouble emptyMatrix = new ScilabDouble();
		assert emptyMatrix.getHeight() == 0;
		assert emptyMatrix.getWidth() == 0;
		assert emptyMatrix.isEmpty() == true;
		assert emptyMatrix.toString().equals("[]") == true;
		emptyMatrix.setRealPart(a);
		emptyMatrix.setImaginaryPart(a);

		ScilabBoolean aMatrix = new ScilabBoolean(true);
		assert aMatrix.equals(emptyMatrix) == false;
		assert emptyMatrix.equals(aMatrix) == false;
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

		double [] result = new double[]{21.2, 23.2, 22.0, 24.0, 42.0, 44.0, 39.0, 40.0, 210.2, 230.2, 220.0, 240.0, 420.0, 440.0, 390.0, 400.0};
		assert aMatrix.getSerializedComplexMatrix().length == result.length;
		assert Arrays.equals(aMatrix.getSerializedComplexMatrix(),result) == true;
		ScilabDouble scalarComplex = new ScilabDouble(1,2);
		assert aMatrix.equals(scalarComplex) == false;
		assert aMatrix.equals(bMatrix) == true;
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
		assert aMatrix.getDataAsByte().length == a.length;
		assert aMatrix.getDataAsByte()[0].length == a[0].length;
		byte [][]d = aMatrix.getDataAsByte();
		long [][]d2 = aMatrix.getData();
		assert d[0][0] == 32;
		assert d2[0][0] == 32;
		assert Arrays.deepEquals(a,d);
		assert aMatrix.getPrec() == ScilabIntegerTypeEnum.sci_uint8;
		assert dMatrix.getPrec() == ScilabIntegerTypeEnum.sci_int8;
		assert aMatrix.isUnsigned() == true;
		assert dMatrix.isUnsigned() == false;
		assert ScilabInteger.convertOldType("TYPE8", true) == ScilabIntegerTypeEnum.sci_uint8;
		assert ScilabInteger.convertOldType("TYPE8", false) == ScilabIntegerTypeEnum.sci_int8;
	}


	@Test
	public void integer8MatrixTests() {
		byte [][]a={{21, 22, 42, 39},{23, 24, 44, 40}};
		ScilabInteger emptyMatrix = new ScilabInteger();
		assert emptyMatrix.getHeight() == 0;
		assert emptyMatrix.getWidth() == 0;
		assert emptyMatrix.isEmpty() == true;
		assert emptyMatrix.toString().equals("int([])") == true;
		emptyMatrix.setData(a, true);

		ScilabBoolean aMatrix = new ScilabBoolean(true);
		assert aMatrix.equals(emptyMatrix) == false;
		assert emptyMatrix.equals(aMatrix) == false;
		ScilabInteger scalarInteger = new ScilabInteger((byte)2);
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
		assert aMatrix.getDataAsShort().length == a.length;
		assert aMatrix.getDataAsShort()[0].length == a[0].length;
		short [][]d = aMatrix.getDataAsShort();
		long [][]d2 = aMatrix.getData();
		assert d[0][0] == 32;
		assert d2[0][0] == 32;
		assert Arrays.deepEquals(a,d);
		assert aMatrix.getPrec() == ScilabIntegerTypeEnum.sci_uint16;
		assert dMatrix.getPrec() == ScilabIntegerTypeEnum.sci_int16;
		assert aMatrix.isUnsigned() == true;
		assert dMatrix.isUnsigned() == false;
		assert ScilabInteger.convertOldType("TYPE16", true) == ScilabIntegerTypeEnum.sci_uint16;
		assert ScilabInteger.convertOldType("TYPE16", false) == ScilabIntegerTypeEnum.sci_int16;
	}

	@Test
	public void integer16MatrixTests() {
		short [][]a={{21, 22, 42, 39},{23, 24, 44, 40}};
		ScilabInteger emptyMatrix = new ScilabInteger();
		emptyMatrix.setData(a, true);

		ScilabBoolean aMatrix = new ScilabBoolean(true);
		assert aMatrix.equals(emptyMatrix) == false;
		assert emptyMatrix.equals(aMatrix) == false;
		ScilabInteger scalarInteger = new ScilabInteger((short)2);
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
		assert aMatrix.getDataAsInt().length == a.length;
		assert aMatrix.getDataAsInt()[0].length == a[0].length;
		int [][]d = aMatrix.getDataAsInt();
		long [][]d2 = aMatrix.getData();
		assert d[0][0] == 32;
		assert d2[0][0] == 32;
		assert Arrays.deepEquals(a,d);
		assert aMatrix.getPrec() == ScilabIntegerTypeEnum.sci_uint32;
		assert dMatrix.getPrec() == ScilabIntegerTypeEnum.sci_int32;
		assert aMatrix.isUnsigned() == true;
		assert dMatrix.isUnsigned() == false;
		assert ScilabInteger.convertOldType("TYPE32", true) == ScilabIntegerTypeEnum.sci_uint32;
		assert ScilabInteger.convertOldType("TYPE32", false) == ScilabIntegerTypeEnum.sci_int32;
	}

	@Test
	public void integer32MatrixTests() {
		int [][]a={{21, 22, 42, 39},{23, 24, 44, 40}};
		ScilabInteger emptyMatrix = new ScilabInteger();
		emptyMatrix.setData(a, true);

		ScilabBoolean aMatrix = new ScilabBoolean(true);
		assert aMatrix.equals(emptyMatrix) == false;
		assert emptyMatrix.equals(aMatrix) == false;
		ScilabInteger scalarInteger = new ScilabInteger((int)2);
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
		assert aMatrix.getDataAsLong().length == a.length;
		assert aMatrix.getDataAsLong()[0].length == a[0].length;
		long [][]d = aMatrix.getDataAsLong();
		long [][]d2 = aMatrix.getData();
		assert d[0][0] == 32;
		assert d2[0][0] == 32;
		assert Arrays.deepEquals(a,d);
		assert aMatrix.getPrec() == ScilabIntegerTypeEnum.sci_uint64;
		assert dMatrix.getPrec() == ScilabIntegerTypeEnum.sci_int64;
		assert aMatrix.isUnsigned() == true;
		assert dMatrix.isUnsigned() == false;
		assert ScilabInteger.convertOldType("TYPE64", true) == ScilabIntegerTypeEnum.sci_uint64;
		assert ScilabInteger.convertOldType("TYPE64", false) == ScilabIntegerTypeEnum.sci_int64;
	}

	@Test
	public void integer64MatrixTests() {
		long [][]a={{21, 22, 42, 39},{23, 24, 44, 40}};
		ScilabInteger emptyMatrix = new ScilabInteger();
		emptyMatrix.setData(a, true);

		ScilabBoolean aMatrix = new ScilabBoolean(true);
		assert aMatrix.equals(emptyMatrix) == false;
		assert emptyMatrix.equals(aMatrix) == false;
		ScilabInteger scalarInteger = new ScilabInteger((long)2);
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
	public void booleanMatrixTests() {
		boolean [][]a={{true,false,true}, {true,true,true}};
		ScilabBoolean emptyMatrix = new ScilabBoolean();
		assert emptyMatrix.getHeight() == 0;
		assert emptyMatrix.getWidth() == 0;
		assert emptyMatrix.isEmpty() == true;
		assert emptyMatrix.toString().equals("[]") == true;
		emptyMatrix.setData(a);
		ScilabDouble aMatrix = new ScilabDouble(2);
		assert aMatrix.equals(emptyMatrix) == false;
		assert emptyMatrix.equals(aMatrix) == false;
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

	@Test(expectedExceptions = IllegalArgumentException.class)
	public void stringMatrixTests() {
		String [][]a={{"This","is","my","string"},{"and","I want to", "compare"," them"}};
		ScilabString emptyMatrix = new ScilabString();
		assert emptyMatrix.getHeight() == 0;
		assert emptyMatrix.getWidth() == 0;
		assert emptyMatrix.isEmpty() == true;
		assert emptyMatrix.toString().equals("[]") == true;
		emptyMatrix.setData(a);
		ScilabDouble aMatrix = new ScilabDouble(2);
		assert aMatrix.equals(emptyMatrix) == false;
		assert emptyMatrix.equals(aMatrix) == false;
		String []b={"This","is","my","string"};
		ScilabString vectorString = new ScilabString(b);
		String []c = null;
		ScilabString emptyString = new ScilabString(c);
		ScilabString nullString = new ScilabString((String)null);

	}

	@Test
	public void compareListTest() throws NullPointerException {
		ScilabList data = new ScilabList();
		assert data.getHeight() == 0;
		assert data.getWidth() == 0;
		assert data.toString().equals("list()");
		data.add(new ScilabDouble(2));
		assert data.getHeight() == 1;
		assert data.getWidth() == 1;
		data.add(new ScilabDouble(51));
		assert data.getHeight() == 1;
		assert data.getWidth() == 2;
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
		assert data.toString().equals("list([2.0], [51.0], [\"hello\"])") == true;
		assert data3.toString().equals("list([2.0], [42.0], [%t], [\"hello\"])") == true;

		ScilabList data4 = new ScilabList(data3);
		assert data4.equals(data3) == true;

	}


	@Test
	public void compareTListTest() throws NullPointerException {
		ScilabTList data = new ScilabTList();
		assert data.getHeight() == 0;
		assert data.getWidth() == 0;
		assert data.toString().equals("tlist()");
		data.add(new ScilabString("hello"));
		assert data.getHeight() == 1;
		assert data.getWidth() == 1;
		data.add(new ScilabDouble(2));
		assert data.getHeight() == 1;
		assert data.getWidth() == 2;
		data.add(new ScilabDouble(51));
		assert data.getHeight() == 1;
		assert data.getWidth() == 3;
		ScilabTList data2 = new ScilabTList();
		data2.add(new ScilabString("hello"));
		data2.add(new ScilabDouble(2));
		data2.add(new ScilabDouble(51));
		ScilabTList data3 = new ScilabTList();
		data3.add(new ScilabString("hello"));
		data3.add(new ScilabDouble(2));
		data3.add(new ScilabDouble(42));
		data3.add(new ScilabBoolean(true));
		assert data.equals(data2) == true;
		assert data.equals(data3) == false;

		assert data.toString().equals("tlist([\"hello\"], [2.0], [51.0])") == true;
		assert data3.toString().equals("tlist([\"hello\"], [2.0], [42.0], [%t])") == true;
		String []b={"a","b","c"};

		ScilabTList tlist = new ScilabTList(b);
		tlist.add(new ScilabDouble(2));
		tlist.add(new ScilabDouble(3));
		assert tlist.toString().equals("tlist([\"a\", \"b\", \"c\"], [2.0], [3.0])") == true;
		ScilabTList tlistFromAnOther = new ScilabTList(b, data2);
		assert tlistFromAnOther.getHeight() == 1;
		assert tlistFromAnOther.getWidth() == 4;
		assert tlistFromAnOther.toString().equals("tlist([\"a\", \"b\", \"c\"], [\"hello\"], [2.0], [51.0])");
	}


	@Test
	public void compareMListTest() throws NullPointerException {
		ScilabMList data = new ScilabMList();
		assert data.getHeight() == 0;
		assert data.getWidth() == 0;
		assert data.toString().equals("mlist()");
		data.add(new ScilabString("hello"));
		assert data.getHeight() == 1;
		assert data.getWidth() == 1;
		data.add(new ScilabDouble(2));
		assert data.getHeight() == 1;
		assert data.getWidth() == 2;
		data.add(new ScilabDouble(51));
		assert data.getHeight() == 1;
		assert data.getWidth() == 3;
		ScilabMList data2 = new ScilabMList();
		data2.add(new ScilabString("hello"));
		data2.add(new ScilabDouble(2));
		data2.add(new ScilabDouble(51));
		ScilabMList data3 = new ScilabMList();
		data3.add(new ScilabString("hello"));
		data3.add(new ScilabDouble(2));
		data3.add(new ScilabDouble(42));
		data3.add(new ScilabBoolean(true));
		assert data.equals(data2) == true;
		assert data.equals(data3) == false;

		assert data.toString().equals("mlist([\"hello\"], [2.0], [51.0])") == true;
		assert data3.toString().equals("mlist([\"hello\"], [2.0], [42.0], [%t])") == true;
		String []b={"a","b","c"};

		ScilabMList mlist = new ScilabMList(b);
		mlist.add(new ScilabDouble(2));
		mlist.add(new ScilabDouble(3));
		assert mlist.toString().equals("mlist([\"a\", \"b\", \"c\"], [2.0], [3.0])") == true;
		ScilabMList mlistFromAnOther = new ScilabMList(b, data2);
		assert mlistFromAnOther.getHeight() == 1;
		assert mlistFromAnOther.getWidth() == 4;
		assert mlistFromAnOther.toString().equals("mlist([\"a\", \"b\", \"c\"], [\"hello\"], [2.0], [51.0])");

	}


}

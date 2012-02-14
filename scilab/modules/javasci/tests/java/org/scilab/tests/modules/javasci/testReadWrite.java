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
import static org.testng.AssertJUnit.*;

import java.util.Arrays;
import java.util.Map;

import org.scilab.modules.javasci.Scilab;
import org.scilab.modules.javasci.JavasciException;
import org.scilab.modules.javasci.JavasciException.InitializationException;
import org.scilab.modules.javasci.JavasciException.UnsupportedTypeException;
import org.scilab.modules.javasci.JavasciException.UndefinedVariableException;
import org.scilab.modules.types.ScilabType;
import org.scilab.modules.types.ScilabDouble;
import org.scilab.modules.types.ScilabBoolean;
import org.scilab.modules.types.ScilabMList;
import org.scilab.modules.types.ScilabString;
import org.scilab.modules.types.ScilabSparse;
import org.scilab.modules.types.ScilabTypeEnum;

public class testReadWrite {
    private Scilab sci;

    /* 
     * This method will be called for each test.
     * with @AfterMethod, this ensures that all the time the engine is closed
     * especially in case of error.
     * Otherwise, the engine might be still running and all subsequent tests
     * would fail.
     */ 
    @BeforeMethod
    public void open() throws NullPointerException, JavasciException {
        sci = new Scilab();
        assertTrue(sci.open());
    }

    @Test(sequential = true) 
    public void putAndGetEmptyMatrixTest() throws NullPointerException, JavasciException {
        ScilabDouble aOriginal = new ScilabDouble();
        sci.put("a",aOriginal);
        ScilabDouble aFromScilab = (ScilabDouble)sci.get("a");

        assertTrue(aFromScilab.equals(aOriginal));
    }

    @Test(sequential = true) 
    public void putAndGetDoubleTest() throws NullPointerException, JavasciException {
        double [][]a={{21.2, 22.0, 42.0, 39.0},{23.2, 24.0, 44.0, 40.0}};
        ScilabDouble aOriginal = new ScilabDouble(a);
        sci.put("a",aOriginal);
        assertTrue(sci.exec("somme = sum(a);"));

        ScilabDouble aFromScilab = (ScilabDouble)sci.get("a");

        assertTrue(aFromScilab.equals(aOriginal));
    }

    @Test(sequential = true) 
    public void putAndGetComplexDoubleTest() throws NullPointerException, JavasciException {
        double [][]a={{21.2, 22.0, 42.0, 39.0},{23.2, 24.0, 44.0, 40.0}};
        double [][]aImg={{212.2, 221.0, 423.0, 393.0},{234.2, 244.0, 441.0, 407.0}};

        ScilabDouble aOriginal = new ScilabDouble(a, aImg);
        sci.put("a",aOriginal);

        ScilabDouble aFromScilab = (ScilabDouble)sci.get("a");

        assertTrue(aFromScilab.equals(aOriginal));
    }



    @Test(sequential = true) 
    public void putAndGetBooleanTest() throws NullPointerException, JavasciException {
        boolean [][]a={{true, true, false, false},{true, false, true, false}};
        ScilabBoolean aOriginal = new ScilabBoolean(a);
        sci.put("a",aOriginal);

        ScilabBoolean aFromScilab = (ScilabBoolean)sci.get("a");

        assertTrue(aFromScilab.equals(aOriginal));
    }

    @Test(sequential = true) 
    public void putAndGetStringTest() throws NullPointerException, JavasciException {
        String [][]a={{"String1", "String2", "String3", "String4"},
                      {"String5", "String6", "My String 7", "String8"}};
        ScilabString aOriginal = new ScilabString(a);
        sci.put("a",aOriginal);

        assertTrue(sci.exec("checksize = and(size(a)==[2,4]);"));

        ScilabBoolean checksize = (ScilabBoolean)sci.get("checksize");
        assertTrue(checksize.getData()[0][0]);

        ScilabString aFromScilab = (ScilabString)sci.get("a");

        assertTrue(aFromScilab.equals(aOriginal));
    }

//    @Test(sequential = true, expectedExceptions = UnsupportedTypeException.class)
    @Test(sequential = true)
    public void ReadSparseTypeTest() throws NullPointerException, JavasciException {
        assertTrue(sci.exec("W=sparse([1,2;4,5;3,10],[1,2,3]);"));
        assertEquals(sci.getVariableType("W"), ScilabTypeEnum.sci_sparse);
        ScilabSparse aFromScilab = (ScilabSparse)sci.get("W");
        assertTrue(aFromScilab.toString().equals("sparse([1, 2 ; 3, 10 ; 4, 5], [1.0 ; 3.0 ; 2.0], [4, 10])"));
        assertTrue(sci.exec("AZE= "+aFromScilab.toString()));
        ScilabSparse aFromScilab2 = (ScilabSparse)sci.get("AZE");

		assertTrue(Arrays.deepEquals(aFromScilab.getFullRealPart(), aFromScilab2.getFullRealPart()));

		ScilabSparse mySparse = new ScilabSparse(100, 100, 5, new int[] { 1, 1, 1, 1, 1}, new int[]{ 1, 25, 50, 75, 99}, new double[] { 1.0, 2.0, 3.0, 4.0, 5.0});
        // sci.put with a sparse is not yet functionnal
//        assertTrue(sci.put("mySparse", mySparse));
//        String ref="mySparseRef = sparse([1, 2 ; 2, 26 ; 3, 51 ; 4, 76 ; 5, 100], [1.0 ; 2.0 ; 3.0 ; 4.0 ; 5.0], [100, 100]);";
//        sci.exec("isEqual=(mySparseRef==mySparse)");
//        ScilabBoolean isEqual = (ScilabBoolean)sci.get("isEqual");
//        System.out.println("isequal " +isEqual);

        
    }

    @Test(sequential = true)
    public void ReadStructTest() throws NullPointerException, JavasciException {
        assertTrue(sci.exec("myDate=struct('day',25,'month' ,'DEC','year',2006)"));
        assertEquals(sci.getVariableType("myDate"), ScilabTypeEnum.sci_mlist);

        ScilabMList myDate = (ScilabMList)sci.get("myDate");
        assertTrue(myDate.toString().equals("mlist([\"st\", \"dims\", \"day\", \"month\", \"year\"], int32([1, 1]), [25.0], [\"DEC\"], [2006.0])"));
        assertEquals(myDate.getHeight(), 1);
        assertEquals(myDate.getWidth(), 5);
        assertTrue(myDate.getVarName().equals("myDate"));
        assertTrue(myDate.getMListType().equals("st"));
        Map<String, ScilabType> listFields = myDate.getMListFields();
        ScilabString month = (ScilabString)listFields.get("month");
        assertTrue(month.getData()[0][0].equals("DEC"));
        ScilabDouble year = (ScilabDouble)listFields.get("year");
        assertEquals(year.getRealPart()[0][0], 2006.0);
        ScilabDouble day = (ScilabDouble)listFields.get("day");
        assertEquals(day.getRealPart()[0][0], 25.0);

    }

    @Test(sequential = true, expectedExceptions = UndefinedVariableException.class)
    public void UndefinedVariableExceptionTest() throws NullPointerException, JavasciException {
        sci.get("undefinedVar"); /* Will launch an UnsupportedTypeException exception */
    }

    /**
     * See #open()
     */
    @AfterMethod
    public void close() {
        sci.close();
        
    }
}
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
        assert sci.open() == true;
    }

    @Test(sequential = true) 
    public void putAndGetDoubleTest() throws NullPointerException, JavasciException {
        double [][]a={{21.2, 22.0, 42.0, 39.0},{23.2, 24.0, 44.0, 40.0}};
        ScilabDouble aOriginal = new ScilabDouble(a);
        sci.put("a",aOriginal);
        assert sci.exec("somme = sum(a);") == true;

        ScilabDouble aFromScilab = (ScilabDouble)sci.get("a");

        assert aFromScilab.equals(aOriginal);
    }

    @Test(sequential = true) 
    public void putAndGetComplexDoubleTest() throws NullPointerException, JavasciException {
        double [][]a={{21.2, 22.0, 42.0, 39.0},{23.2, 24.0, 44.0, 40.0}};
        double [][]aImg={{212.2, 221.0, 423.0, 393.0},{234.2, 244.0, 441.0, 407.0}};

        ScilabDouble aOriginal = new ScilabDouble(a, aImg);
        sci.put("a",aOriginal);

        ScilabDouble aFromScilab = (ScilabDouble)sci.get("a");

        assert aFromScilab.equals(aOriginal);
    }



    @Test(sequential = true) 
    public void putAndGetBooleanTest() throws NullPointerException, JavasciException {
        boolean [][]a={{true, true, false, false},{true, false, true, false}};
        ScilabBoolean aOriginal = new ScilabBoolean(a);
        sci.put("a",aOriginal);

        ScilabBoolean aFromScilab = (ScilabBoolean)sci.get("a");

        assert aFromScilab.equals(aOriginal);
    }

    @Test(sequential = true) 
    public void putAndGetStringTest() throws NullPointerException, JavasciException {
        String [][]a={{"String1", "String2", "String3", "String4"},
                      {"String5", "String6", "My String 7", "String8"}};
        ScilabString aOriginal = new ScilabString(a);
        sci.put("a",aOriginal);

        assert sci.exec("checksize = and(size(a)==[2,4]);") == true;

        ScilabBoolean checksize = (ScilabBoolean)sci.get("checksize");
        assert checksize.getData()[0][0] == true;

        ScilabString aFromScilab = (ScilabString)sci.get("a");

        assert aFromScilab.equals(aOriginal);
    }

//    @Test(sequential = true, expectedExceptions = UnsupportedTypeException.class)
    @Test(sequential = true)
    public void ReadSparseTypeTest() throws NullPointerException, JavasciException {
        assert sci.exec("W=sparse([1,2;4,5;3,10],[1,2,3]);") == true;
        assert sci.getVariableType("W") == ScilabTypeEnum.sci_sparse;
        ScilabSparse aFromScilab = (ScilabSparse)sci.get("W");
        assert aFromScilab.toString().equals("sparse([1, 2 ; 3, 10 ; 4, 5], [1.0 ; 3.0 ; 2.0], [4, 10])");
        assert sci.exec("AZE= "+aFromScilab.toString());
        ScilabSparse aFromScilab2 = (ScilabSparse)sci.get("AZE");

		assert Arrays.deepEquals(aFromScilab.getFullRealPart(), aFromScilab2.getFullRealPart()) == true;

        
    }

    @Test(sequential = true)
    public void ReadStructTest() throws NullPointerException, JavasciException {
        assert sci.exec("myDate=struct('day',25,'month' ,'DEC','year',2006)") == true;
        assert sci.getVariableType("myDate") == ScilabTypeEnum.sci_mlist;

        ScilabMList myDate = (ScilabMList)sci.get("myDate");
        assert myDate.toString().equals("mlist([\"st\", \"dims\", \"day\", \"month\", \"year\"], int32([1, 1]), [25.0], [\"DEC\"], [2006.0])");
        assert myDate.getHeight() == 1;
        assert myDate.getWidth() == 5;
        assert myDate.getVarName().equals("myDate");
        assert myDate.getMListType().equals("st");
        Map<String, ScilabType> listFields = myDate.getMListFields();
        ScilabString month = (ScilabString)listFields.get("month");
        assert month.getData()[0][0].equals("DEC");
        ScilabDouble year = (ScilabDouble)listFields.get("year");
        assert year.getRealPart()[0][0] == 2006.0;
        ScilabDouble day = (ScilabDouble)listFields.get("day");
        assert day.getRealPart()[0][0] == 25.0;

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
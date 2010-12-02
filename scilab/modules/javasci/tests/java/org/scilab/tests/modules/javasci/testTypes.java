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

import org.scilab.modules.javasci.Scilab;
import org.scilab.modules.javasci.JavasciException;
import org.scilab.modules.javasci.JavasciException.UndefinedVariableException;
import org.scilab.modules.types.ScilabTypeEnum;

public class testTypes {
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
    public void getVariableTypeTest() throws NullPointerException, JavasciException {

        sci.exec("a = 2*%pi");
        assert sci.getVariableType("a") == ScilabTypeEnum.sci_matrix;

        sci.exec("a = rand(10,10)");
        assert sci.getVariableType("a") == ScilabTypeEnum.sci_matrix;

        sci.exec("b = 'plop'");
        assert sci.getVariableType("b") == ScilabTypeEnum.sci_strings;

        sci.exec("b = ['plop', plip]");
        assert sci.getVariableType("b") == ScilabTypeEnum.sci_strings;

        sci.exec("s=poly(0,'s');p=1+s+2*s^2;");
        assert sci.getVariableType("p") == ScilabTypeEnum.sci_poly;

        sci.exec("b=%t;");
        assert sci.getVariableType("b") == ScilabTypeEnum.sci_boolean;

        sci.exec("b=[%t, %f];");
        assert sci.getVariableType("b") == ScilabTypeEnum.sci_boolean;

        sci.exec("sp=sparse([1,2;4,5;3,10],[1,2,3])");
        assert sci.getVariableType("sp") == ScilabTypeEnum.sci_sparse;

        sci.exec("a = sparse([%t, %f, %t ; %f, %t, %f ; %t, %f, %t]);");
        assert sci.getVariableType("a") == ScilabTypeEnum.sci_boolean_sparse;

        sci.exec("sp=sparse([1,2;4,5;3,10],[%t,%t,%t])");
        assert sci.getVariableType("sp") == ScilabTypeEnum.sci_boolean_sparse;

        sci.exec("i8=int8([1 -120 127 312])");
        assert sci.getVariableType("i8") == ScilabTypeEnum.sci_ints;

        sci.exec("i8=uint8([1 -120 127 312])");
        assert sci.getVariableType("i8") == ScilabTypeEnum.sci_ints;

        sci.exec("x=int32(-200:100:400)");
        assert sci.getVariableType("x") == ScilabTypeEnum.sci_ints;

        sci.exec("x=uint32(-200:100:400)");
        assert sci.getVariableType("x") == ScilabTypeEnum.sci_ints;

        sci.exec("t = tlist(['listtype','field1','field2'], [], []);");
        assert sci.getVariableType("t") == ScilabTypeEnum.sci_tlist;

        sci.exec("t.field1(1);");
        assert sci.getVariableType("t") == ScilabTypeEnum.sci_tlist;

        sci.exec("l = list(1,['a' 'b'])");
        assert sci.getVariableType("l") == ScilabTypeEnum.sci_list;

        sci.exec("l(0) = 'foo'");
        assert sci.getVariableType("l") == ScilabTypeEnum.sci_list;

        sci.exec("M=mlist(['V','name','value'],['a','b';'c' 'd'],[1 2; 3 4]);");
        assert sci.getVariableType("M") == ScilabTypeEnum.sci_mlist;

    }

    @Test(sequential = true, expectedExceptions = UndefinedVariableException.class)
    public void failGetVariableTypeTest() throws NullPointerException, IllegalArgumentException, JavasciException {
        sci.getVariableType("nonexistingvariable");

    }

    /**
     * See #open()
     */
    @AfterMethod
    public void close() {
        sci.close();
        
    }
}

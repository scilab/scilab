/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Sylvestre LEDRU
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
package org.scilab.tests.modules.javasci;

import org.junit.*;
import static org.junit.Assert.*;

import org.scilab.modules.javasci.Scilab;
import org.scilab.modules.javasci.JavasciException;
import org.scilab.modules.javasci.JavasciException.UndefinedVariableException;
import org.scilab.modules.types.ScilabTypeEnum;

public class testTypes {
    private Scilab sci;

    /*
     * This method will be called for each test.
     * with @After, this ensures that all the time the engine is closed
     * especially in case of error.
     * Otherwise, the engine might be still running and all subsequent tests
     * would fail.
     */
    @Before
    public void open() throws NullPointerException, JavasciException {
        sci = new Scilab();
        assertTrue(sci.open());
    }

    @Test()
    public void getVariableTypeTest() throws NullPointerException, JavasciException {

        sci.exec("a = 2*%pi");
        assertEquals(sci.getVariableType("a"), ScilabTypeEnum.sci_matrix);

        sci.exec("a = rand(10,10)");
        assertEquals(sci.getVariableType("a"), ScilabTypeEnum.sci_matrix);

        sci.exec("b = 'plop'");
        assertEquals(sci.getVariableType("b"), ScilabTypeEnum.sci_strings);

        sci.exec("b = ['plop', plip]");
        assertEquals(sci.getVariableType("b"), ScilabTypeEnum.sci_strings);

        sci.exec("s=poly(0,'s');p=1+s+2*s^2;");
        assertEquals(sci.getVariableType("p"), ScilabTypeEnum.sci_poly);

        sci.exec("b=%t;");
        assertEquals(sci.getVariableType("b"), ScilabTypeEnum.sci_boolean);

        sci.exec("b=[%t, %f];");
        assertEquals(sci.getVariableType("b"), ScilabTypeEnum.sci_boolean);

        sci.exec("sp=sparse([1,2;4,5;3,10],[1,2,3])");
        assertEquals(sci.getVariableType("sp"), ScilabTypeEnum.sci_sparse);

        sci.exec("a = sparse([%t, %f, %t ; %f, %t, %f ; %t, %f, %t]);");
        assertEquals(sci.getVariableType("a"), ScilabTypeEnum.sci_boolean_sparse);

        sci.exec("sp=sparse([1,2;4,5;3,10],[%t,%t,%t])");
        assertEquals(sci.getVariableType("sp"), ScilabTypeEnum.sci_boolean_sparse);

        sci.exec("i8=int8([1 -120 127 312])");
        assertEquals(sci.getVariableType("i8"), ScilabTypeEnum.sci_ints);

        sci.exec("i8=uint8([1 -120 127 312])");
        assertEquals(sci.getVariableType("i8"), ScilabTypeEnum.sci_ints);

        sci.exec("x=int32(-200:100:400)");
        assertEquals(sci.getVariableType("x"), ScilabTypeEnum.sci_ints);

        sci.exec("x=uint32(-200:100:400)");
        assertEquals(sci.getVariableType("x"), ScilabTypeEnum.sci_ints);

        sci.exec("t = tlist(['listtype','field1','field2'], [], []);");
        assertEquals(sci.getVariableType("t"), ScilabTypeEnum.sci_tlist);

        sci.exec("t.field1(1);");
        assertEquals(sci.getVariableType("t"), ScilabTypeEnum.sci_tlist);

        sci.exec("l = list(1,['a' 'b'])");
        assertEquals(sci.getVariableType("l"), ScilabTypeEnum.sci_list);

        sci.exec("l(0) = 'foo'");
        assertEquals(sci.getVariableType("l"), ScilabTypeEnum.sci_list);

        sci.exec("M=mlist(['V','name','value'],['a','b';'c' 'd'],[1 2; 3 4]);");
        assertEquals(sci.getVariableType("M"), ScilabTypeEnum.sci_mlist);

    }

    @Test( expected = UndefinedVariableException.class)
    public void failGetVariableTypeTest() throws NullPointerException, IllegalArgumentException, JavasciException {
        sci.getVariableType("nonexistingvariable");

    }

    @Test( expected = UndefinedVariableException.class)
    public void failGetVariableType2Test() throws NullPointerException, IllegalArgumentException, JavasciException {
        sci.getVariableTypeInCurrentScilabSession("nonexistingvariable");

    }
    /**
     * See #open()
     */
    @After
    public void close() {
        sci.close();

    }
}

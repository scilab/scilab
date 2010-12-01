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
import org.scilab.modules.javasci.JavasciException.InitializationException;
import org.scilab.modules.javasci.JavasciException.UnsupportedTypeException;
import org.scilab.modules.javasci.JavasciException.UndefinedVariableException;
import org.scilab.modules.types.ScilabDouble;

public class testExistingVariable {
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
    public void existVariableTest() throws NullPointerException, JavasciException {
        double [][]a={{21.2, 22.0, 42.0, 39.0},{23.2, 24.0, 44.0, 40.0}};
        ScilabDouble aOriginal = new ScilabDouble(a);
        sci.put("a",aOriginal);
        assert sci.isExistingVariable("a") == true;
    }

    @Test(sequential = true) 
    public void notExistVariableTest() throws NullPointerException, JavasciException {
        assert sci.isExistingVariable("a") == false;
        assert sci.isExistingVariable("b") == false;
    }


    @Test(sequential = true) 
    public void existVariableAfterExecTest() throws NullPointerException, JavasciException {
        assert sci.exec("a=rand(20,20);") == true;
        assert sci.isExistingVariable("a") == true;
        assert sci.exec("b='test variable';") == true;
        assert sci.isExistingVariable("b") == true;
    }


    /**
     * See #open()
     */
    @AfterMethod
    public void close() {
        sci.close();
        
    }
}
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
import org.scilab.modules.types.ScilabDouble;
import org.scilab.modules.javasci.JavasciException;
import org.scilab.modules.javasci.JavasciException.InitializationException;


public class testErrorManagement {
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
    public void getLastErrorCodeTest() throws NullPointerException, JavasciException {
        assert sci.getLastErrorCode() == 0; // No error
        sci.close();

        assert sci.open("a=1+") == false;
        assert sci.getLastErrorCode() == 2;
        sci.exec("errclear();");
        sci.exec("a+b");
        assert sci.getLastErrorCode() == 4;
        sci.exec("errclear();");
    }

    @Test(sequential = true)
    public void getLastErrorMessageTest() throws NullPointerException, JavasciException {
        sci.exec("errclear();"); // No error by default
        assert sci.getLastErrorMessage().equals("");
        assert sci.getLastErrorMessage().length() == 0;

        sci.exec("errclear();");
        sci.close();

        assert sci.open("a=1+") == false;
        assert sci.getLastErrorMessage().length() > 0;
        sci.exec("errclear();");
        sci.exec("a+b"); //undefined a & b
        assert sci.getLastErrorMessage().length() > 0;
        sci.exec("errclear();");
        sci.exec("a=rand(10,10);");//no error
        assert sci.getLastErrorMessage().length() == 0;
    }

    /**
     * See #open()
     */
    @AfterMethod
    public void close() {
        sci.close();
    }
}

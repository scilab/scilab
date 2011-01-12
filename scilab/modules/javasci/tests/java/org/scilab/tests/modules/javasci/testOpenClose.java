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
import java.io.IOException;

import org.scilab.modules.javasci.Scilab;
import org.scilab.modules.types.ScilabType;
import org.scilab.modules.types.ScilabDouble;
import org.scilab.modules.types.ScilabBoolean;
import org.scilab.modules.javasci.JavasciException;
import org.scilab.modules.javasci.JavasciException.InitializationException;

import org.scilab.modules.commons.ScilabConstants;

public class testOpenClose {
    private Scilab sci;

    /* 
     * This method will be called for each test.
     * with @AfterMethod, this ensures that all the time the engine is closed
     * especially in case of error.
     * Otherwise, the engine might be still running and all subsequent tests
     * would fail.
     */ 
    @BeforeMethod
    public void openTest() throws NullPointerException, JavasciException {
        sci = new Scilab();
        assert sci.open() == true;
    }

    @Test(sequential = true)
    public void multipleOpenCloseTest() throws NullPointerException, JavasciException {
        assert sci.close() == true;
        assert sci.open() == true;
        assert sci.close() == true;
    }

    @Test(sequential = true, expectedExceptions = JavasciException.class)
    public void specificWrongSCIPathTest() throws NullPointerException, JavasciException {
        assert sci.close() == true;
        sci = new Scilab(System.getProperty("java.io.tmpdir")+"/non-existing-directory-scilab/");
    }


    @Test(sequential = true)
    public void specificPropertySCIPathTest() throws NullPointerException, JavasciException {
        assert sci.close() == true;
        sci = new Scilab(System.getProperty("SCI"));
    }

    @Test(sequential = true)
    public void specificEnvSCIPathTest() throws NullPointerException, JavasciException {
        assert sci.close() == true;
        String SCIPath = System.getProperty("SCI"); // Temp backup to set it again
        System.clearProperty("SCI"); // Remove the property to check it is using the variable
        sci = new Scilab();
        assert sci.open("a=42*2;") == true;

    }

    @Test(sequential = true)
    public void OpenWithJobTest() throws NullPointerException, JavasciException {
        assert sci.close() == true;
        assert sci.open("a=42*2;") == true;

        ScilabType a = sci.get("a");

        assert ((ScilabDouble)a).getRealPart()[0][0] == 84.0;
    }

    @Test(sequential = true)
    public void OpenWithJobsTest() throws NullPointerException, JavasciException {
        assert sci.close() == true;
        assert sci.open(new String[]{"a=42*2;","b=44*2", "c=(a==b)"}) == true;

        ScilabType a = sci.get("a");
        assert ((ScilabDouble)a).getRealPart()[0][0] == 84.0;

        ScilabType b = sci.get("b");
        assert ((ScilabDouble)b).getRealPart()[0][0] == 88.0;

        ScilabType c = sci.get("c");
        assert ((ScilabBoolean)c).getData()[0][0] == false;
    }

    @Test(sequential = true, expectedExceptions = JavasciException.class)
    public void OpenMultipleTimeTest() throws NullPointerException, JavasciException {
        assert sci.open("a=42*2;") == true;

    }

    /**
     * See #open()
     */
    @AfterMethod
    public void close() {
        sci.close();
        
    }
}

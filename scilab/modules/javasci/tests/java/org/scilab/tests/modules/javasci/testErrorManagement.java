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
import org.scilab.modules.types.ScilabDouble;
import org.scilab.modules.javasci.JavasciException;
import org.scilab.modules.javasci.JavasciException.InitializationException;
import org.scilab.modules.javasci.JavasciException.ScilabErrorException;


public class testErrorManagement {
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
    public void getLastErrorCodeTest() throws NullPointerException, JavasciException {
        assertEquals(sci.getLastErrorCode(), 0); // No error
        sci.close();

        assertEquals(sci.open("a=1+"), false);
        assertEquals(sci.getLastErrorCode(), 2);
        sci.exec("errclear();");
        sci.exec("a+b");
        assertEquals(sci.getLastErrorCode(), 4);
        sci.exec("errclear();");
    }

    @Test()
    public void getLastErrorMessageTest() throws NullPointerException, JavasciException {
        sci.exec("errclear();"); // No error by default
        assertTrue(sci.getLastErrorMessage().equals(""));
        assertEquals(sci.getLastErrorMessage().length(), 0);

        sci.exec("errclear();");
        sci.close();

        assertEquals(sci.open("a=1+"), false);
        assertTrue(sci.getLastErrorMessage().length() > 0);
        sci.exec("errclear();");
        sci.exec("a+b"); //undefined a & b
        assertTrue(sci.getLastErrorMessage().length() > 0);
        sci.exec("errclear();");
        sci.exec("a=rand(10,10);");//no error
        assertEquals(sci.getLastErrorMessage().length(), 0);
    }

    @Test()
    public void getLastErrorMessageWithExceptionNonErrorTest() throws NullPointerException, JavasciException {
        sci.execException("errclear();"); // No error by default
        assertTrue(sci.getLastErrorMessage().equals(""));
        assertEquals(sci.getLastErrorMessage().length(), 0);

        sci.execException("errclear();");
    }

    @Test()
    public void getLastErrorMessageWithExceptionNonError2Test() throws NullPointerException, JavasciException {
        sci.execException("errclear();"); // No error by default
        assertTrue(sci.getLastErrorMessage().equals(""));
        assertEquals(sci.getLastErrorMessage().length(), 0);
        sci.execException("a=rand(10,10);");//no error
        assertEquals(sci.getLastErrorMessage().length(), 0);
    }


    @Test( expected = ScilabErrorException.class)
    public void getLastErrorMessageWithExceptionWithErrorTest() throws NullPointerException, ScilabErrorException {
        sci.execException("a+b"); //undefined a & b
    }

    @Test( expected = ScilabErrorException.class)
    public void getLastErrorMessageWithExceptionWithError2Test() throws NullPointerException, ScilabErrorException {
        sci.execException("a+b*"); //undefined a & b
    }

    /**
     * See #open()
     */
    @After
    public void close() {
        sci.close();
    }
}

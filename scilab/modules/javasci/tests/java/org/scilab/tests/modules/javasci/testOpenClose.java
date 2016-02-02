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

import java.io.IOException;

import org.scilab.modules.javasci.Scilab;
import org.scilab.modules.types.ScilabType;
import org.scilab.modules.types.ScilabDouble;
import org.scilab.modules.types.ScilabBoolean;
import org.scilab.modules.javasci.JavasciException;
import org.scilab.modules.javasci.JavasciException.InitializationException;
import org.scilab.modules.javasci.JavasciException.AlreadyRunningException;

import org.scilab.modules.commons.ScilabConstants;

public class testOpenClose {
    private Scilab sci;

    /*
     * This method will be called for each test.
     * with @After, this ensures that all the time the engine is closed
     * especially in case of error.
     * Otherwise, the engine might be still running and all subsequent tests
     * would fail.
     */
    @Before
    public void openTest() throws NullPointerException, JavasciException {
        sci = new Scilab();
        assertTrue(sci.open());
    }

    @Test()
    public void multipleOpenCloseTest() throws NullPointerException, JavasciException {
        assertTrue(sci.close());
        assertTrue(sci.open());
        assertTrue(sci.close());
    }

    @Test( expected = JavasciException.class)
    public void specificWrongSCIPathTest() throws NullPointerException, JavasciException {
        assertTrue(sci.close());
        sci = new Scilab(System.getProperty("java.io.tmpdir") + "/non-existing-directory-scilab/");
    }


    @Test()
    public void specificPropertySCIPathTest() throws NullPointerException, JavasciException {
        assertTrue(sci.close());
        sci = new Scilab(System.getProperty("SCI"));
    }

    @Test()
    public void specificEnvSCIPathTest() throws NullPointerException, JavasciException {
        assertTrue(sci.close());
        String SCIPath = System.getProperty("SCI"); // Temp backup to set it again
        System.clearProperty("SCI"); // Remove the property to check it is using the variable
        sci = new Scilab();
        assertTrue(sci.open("a=42*2;"));

    }

    @Test()
    public void OpenWithJobTest() throws NullPointerException, JavasciException {
        assertTrue(sci.close());
        assertTrue(sci.open("a=42*2;"));

        ScilabType a = sci.get("a");

        assertEquals(((ScilabDouble)a).getRealPart()[0][0], 84.0, 1e-8);
    }

    @Test()
    public void OpenWithJobsTest() throws NullPointerException, JavasciException {
        assertTrue(sci.close());
        assertTrue(sci.open(new String[] {"a=42*2;", "b=44*2", "c=(a==b)"}));

        ScilabType a = sci.get("a");
        assertEquals(((ScilabDouble)a).getRealPart()[0][0], 84.0, 1e-8);

        ScilabType b = sci.get("b");
        assertEquals(((ScilabDouble)b).getRealPart()[0][0], 88.0, 1e-8);

        ScilabType c = sci.get("c");
        assertEquals(((ScilabBoolean)c).getData()[0][0], false);
    }

    @Test( expected = AlreadyRunningException.class)
    public void OpenMultipleTimeTest() throws NullPointerException, JavasciException {
        assertTrue(sci.open("a=42*2;"));
    }

    /**
     * See #open()
     */
    @After
    public void close() {
        sci.close();
    }
}

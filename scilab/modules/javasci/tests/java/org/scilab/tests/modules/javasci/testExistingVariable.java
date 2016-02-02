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
import org.scilab.modules.javasci.JavasciException.InitializationException;
import org.scilab.modules.javasci.JavasciException.UnsupportedTypeException;
import org.scilab.modules.javasci.JavasciException.UndefinedVariableException;
import org.scilab.modules.types.ScilabDouble;

public class testExistingVariable {
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
    public void existVariableTest() throws NullPointerException, JavasciException {
        double [][]a = {{21.2, 22.0, 42.0, 39.0}, {23.2, 24.0, 44.0, 40.0}};
        ScilabDouble aOriginal = new ScilabDouble(a);
        sci.put("a", aOriginal);
        assertTrue(sci.isExistingVariable("a"));
    }

    @Test()
    public void notExistVariableTest() throws NullPointerException, JavasciException {
        assertEquals(sci.isExistingVariable("a"), false);
        assertEquals(sci.isExistingVariable("b"), false);
    }


    @Test()
    public void existVariableAfterExecTest() throws NullPointerException, JavasciException {
        assertTrue(sci.exec("a=rand(20,20);"));
        assertTrue(sci.isExistingVariable("a"));
        assertTrue(sci.exec("b='test variable';"));
        assertTrue(sci.isExistingVariable("b"));
    }


    /**
     * See #open()
     */
    @After
    public void close() {
        sci.close();

    }
}
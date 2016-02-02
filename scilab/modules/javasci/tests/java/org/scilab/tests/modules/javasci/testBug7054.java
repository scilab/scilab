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
import org.scilab.modules.types.ScilabType;
import org.scilab.modules.types.ScilabInteger;
import org.scilab.modules.types.ScilabTypeEnum;

public class testBug7054 {
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

    @Test( expected = java.lang.ClassCastException.class)
    public void nonRegBug7054() throws NullPointerException, JavasciException {
        assertTrue(sci.exec("xx = 123;"));
        /* Trigger an ClassCastException exception with the error:
         * Exception in thread "main" java.lang.ClassCastException: org.scilab.modules.types.ScilabDouble cannot be cast to org.scilab.modules.types.ScilabInteger */
        ScilabInteger zz = (ScilabInteger)sci.get("xx");

    }

    @Test()
    public void nonRegBug7054Working() throws NullPointerException, JavasciException {
        assertTrue(sci.exec("xx = int8(123);"));
        ScilabInteger zz = (ScilabInteger)sci.get("xx");
        assertEquals(zz.getData()[0][0], 123);
    }

    /**
     * See #open()
     */
    @After
    public void close() {
        sci.close();

    }
}

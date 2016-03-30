/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - DIGITEO - Vincent COUVERT
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
import org.scilab.modules.types.ScilabDouble;
import org.scilab.modules.types.ScilabBoolean;
import org.scilab.modules.types.ScilabString;
import org.scilab.modules.types.ScilabTypeEnum;

public class testBug10801 {
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
        sci = new Scilab(true);
        assertTrue(sci.open());
    }

    @Test()
    public void nonRegBug10801() throws NullPointerException, JavasciException {
        try {
            assertTrue(sci.exec("plot3d()"));
            assertTrue(sci.isGraphicOpened());
            assertTrue(sci.exec("winId = winsid();"));
            assertTrue(((ScilabDouble)sci.get("winId")).equals(new ScilabDouble(0)));
            sci.close();
            sci.open();
            assertFalse(sci.isGraphicOpened());
            assertTrue(sci.exec("winId = winsid()"));
            assertTrue(((ScilabDouble)sci.get("winId")).equals(new ScilabDouble()));
            sci.close();
        } catch (Exception e) {
            System.err.print("e = " + e);
        }
    }

    /**
     * See #open()
     */
    @After
    public void close() {
        sci.close();
    }
}
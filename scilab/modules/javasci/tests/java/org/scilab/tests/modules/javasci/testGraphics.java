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

import java.awt.GraphicsEnvironment;

import org.scilab.modules.javasci.Scilab;
import org.scilab.modules.javasci.JavasciException;
import org.scilab.modules.javasci.JavasciException.UndefinedVariableException;
import org.scilab.modules.types.ScilabTypeEnum;

public class testGraphics {
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
        sci = new Scilab(true); // True = enable advanced mode
        assertTrue(sci.open());
    }

    @Test()
    public void isGraphicOpenedTest() throws NullPointerException, JavasciException {
        if (!GraphicsEnvironment.isHeadless()) {
            sci.exec("plot3d();");
            assertTrue(sci.isGraphicOpened());
        }
    }

    @Test()
    public void isGraphicNotOpenedTest() throws NullPointerException, JavasciException {

        sci.exec("a=1+1;");
        assertEquals(sci.isGraphicOpened(), false);
    }

    /**
     * See #open()
     */
    @After
    public void close() {
        sci.close();

    }
}

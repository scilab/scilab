/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Sylvestre Ledru
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.tests.modules.commons;

import static org.junit.Assert.assertTrue;

import java.io.IOException;

import org.junit.Before;
import org.junit.Test;
import org.scilab.modules.commons.ScilabCommons;

/**
 * Check the access of the {@link ScilabCommons} public fields
 */
public class CommonsTests {

    @Before
    public void loadLibrary() {
        System.loadLibrary("scilab");
    }

    @Test
    public void getlanguage() {
        assertTrue(ScilabCommons.getlanguage() != null);
        assertTrue(ScilabCommons.getlanguage().length() > 0);
    }

    @Test
    public void createTempFilename() {
        assertTrue(ScilabCommons.createtempfilename("prefix_", 0) != null);
        assertTrue(ScilabCommons.createtempfilename("prefix_", 0).length() > 0);
    }

    @Test
    public void getSCIHOME() throws IOException {
        assertTrue(ScilabCommons.getSCIHOME() != null);
    }

    @Test
    public void getTMPDIR() throws IOException {
        assertTrue(ScilabCommons.getTMPDIR() != null);
    }
}

/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Sylvestre Ledru
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

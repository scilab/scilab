/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Clement DAVID
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

import java.io.File;
import java.io.IOException;

import org.junit.Before;
import org.junit.Test;
import org.scilab.modules.commons.ScilabConstants;

/**
 * Check the access of the {@link ScilabConstants} public fields
 */
public class Constants {

    @Before
    public void loadLibrary() {
        System.loadLibrary("scilab");
    }

    @Test
    public void checkTmp() throws SecurityException, NoSuchFieldException, IllegalArgumentException, IllegalAccessException, IOException {
        assertTrue(ScilabConstants.TMPDIR != null);
        assertTrue(ScilabConstants.TMPDIR.isDirectory());

        final File tmpFile = new File(ScilabConstants.TMPDIR, "tmpFile");
        tmpFile.createNewFile();
        assertTrue(tmpFile.exists());
        assertTrue(tmpFile.delete());
    }

    @Test
    public void checkSci() throws SecurityException, NoSuchFieldException, IllegalArgumentException, IllegalAccessException {
        assertTrue(ScilabConstants.SCI != null);
        assertTrue(ScilabConstants.SCI.isDirectory());

        final File copying = new File(ScilabConstants.SCI, "COPYING");
        assertTrue(copying.exists());
    }

    @Test
    public void checkSciHome() throws SecurityException, NoSuchFieldException, IllegalArgumentException, IllegalAccessException, IOException {
        assertTrue(ScilabConstants.SCIHOME != null);
        assertTrue(ScilabConstants.SCIHOME.exists());
        assertTrue(ScilabConstants.SCIHOME.isDirectory());

        final File homeFile = new File(ScilabConstants.SCIHOME, "homeFile");
        assertTrue(homeFile.createNewFile());
        assertTrue(homeFile.exists());
        assertTrue(homeFile.delete());
    }
}

/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Clement DAVID
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

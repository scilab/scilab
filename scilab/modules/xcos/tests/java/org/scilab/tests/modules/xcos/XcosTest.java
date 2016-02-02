/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Clement DAVID
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

package org.scilab.tests.modules.xcos;

import static org.junit.Assert.assertTrue;

import org.junit.Before;
import org.junit.Test;
import org.scilab.modules.xcos.Xcos;

/**
 * Test of the {@link XcosTest} class.
 */
public class XcosTest {

    @Before
    public void loadLibrary() {
        System.loadLibrary("scilab");
    }

    /**
     * Be careful when modifying the tradename and version.
     */
    @Test
    public void checkVersion() {
        assertTrue(Xcos.TRADENAME.compareTo("Xcos") == 0);
        assertTrue(Xcos.VERSION.compareTo("2.0") == 0);
    }
}

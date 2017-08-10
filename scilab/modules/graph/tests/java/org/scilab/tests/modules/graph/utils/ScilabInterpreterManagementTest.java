/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
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

package org.scilab.tests.modules.graph.utils;

import static org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.buildCall;

import org.junit.*;

/**
 * Test the {@link org.scilab.modules.graph.utils.ScilabInterpreterManagement} class.
 *
 * This test only perform non-execution related method test.
 */
public class ScilabInterpreterManagementTest {

    @Test
    public void buildCallTest() {
        final String complexCall = "function(1, 3.1416, \"string data\", %t, %f); ";
        final String simpleCall = "function(); ";

        final String method = "function";
        final int arg1 = 1;
        final double arg2 = 3.1416;
        final String arg3 = "string data";
        final boolean arg4 = true;
        final boolean arg5 = false;

        String result = buildCall(method, arg1, arg2, arg3, arg4, arg5);
        assert result.equals(complexCall);

        result = buildCall(method);
        assert result.equals(simpleCall);
    }
}

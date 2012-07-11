/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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

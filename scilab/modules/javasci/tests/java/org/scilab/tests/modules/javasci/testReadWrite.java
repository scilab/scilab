/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Sylvestre LEDRU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.tests.modules.javasci;

import org.testng.annotations.*;

import org.scilab.modules.javasci.Scilab;
import org.scilab.modules.types.scilabTypes.ScilabType;
import org.scilab.modules.types.scilabTypes.ScilabDouble;

public class testReadWrite {

	@Test
    public void putAndGetTest() throws NullPointerException {
        Scilab sci = new Scilab();
        sci.open();
        double [][]a={{21.2, 22.0, 42.0, 39.0},{23.2, 24.0, 44.0, 40.0}};
        ScilabDouble aOriginal = new ScilabDouble(a);
        sci.put("a",aOriginal);
        assert sci.exec("somme = sum(a);") == true;

        ScilabDouble aFromScilab = (ScilabDouble)sci.get("a");

        assert aFromScilab.equals(aOriginal);

    }

}

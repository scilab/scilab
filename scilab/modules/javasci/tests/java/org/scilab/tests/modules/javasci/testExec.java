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

public class testExec {

	@Test
    public void execAndReadTest() throws NullPointerException {
        Scilab sci = new Scilab();
        sci.open();

        /* Scalar test */
        assert sci.exec("a = 1+1") == true;
        ScilabType a = sci.get("a");
		double[][] aReal = ((ScilabDouble)a).getRealPart();

        assert a.getHeight() == 1;
        assert a.getWidth() == 1;

        assert ((ScilabDouble)a).getRealPart()[0][0] == 2.0;

        /* Matrix 10x10 */
        assert sci.exec("b = matrix(1:100,10,10)") == true;

        ScilabType b = sci.get("b");

        assert b.getHeight() == 10;
        assert b.getWidth() == 10;

        /* Check results of the addition of two matrixes */
        assert sci.exec("c = [42, 12; 32, 32] + [2, 1; 3, 2]; sumMatrix = sum(c);") == true;
        ScilabType c = sci.get("c");

        assert c.getHeight() == 2;

        assert c.getWidth() == 2;

        double sum = 0;
        /* Compute ourself the sum of all matrices elements */
		for (int i=0; i < c.getHeight(); i++) {
			for (int j=0; j < c.getWidth(); j++) {
                sum += ((ScilabDouble)c).getRealPart()[i][j];
			}
		}
        ScilabType sumMatrix = sci.get("sumMatrix");
        /* Compare if they match */
        assert ((ScilabDouble)sumMatrix).getRealPart()[0][0] == sum;

//        sci.put("cbis",b);

    }

}

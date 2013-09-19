/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Sylvestre LEDRU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
package org.scilab.tests.modules.types;

import static org.junit.Assert.assertTrue;

import java.util.Arrays;

import org.junit.Test;
import org.scilab.modules.types.ScilabDouble;
import org.scilab.modules.types.ScilabTypeEnum;

public class testTypes {

    @Test
    public void doubleTest() throws NullPointerException {
        double[][] a = { { 21.2, 22.0, 42.0, 39.0 }, { 23.2, 24.0, 44.0, 40.0 } };
        ScilabDouble aOriginal = new ScilabDouble(a, null);
        ScilabDouble aOriginal2 = new ScilabDouble(null, a);
        assertTrue(Arrays.deepEquals(aOriginal.getRealPart(), a) == true);
        assertTrue(aOriginal.getType() == ScilabTypeEnum.sci_matrix);
        assertTrue(aOriginal2.getType() == ScilabTypeEnum.sci_matrix);
        assertTrue(aOriginal2.isSwaped() == false);
        assertTrue(aOriginal.isSwaped() == false);

        ScilabDouble adouble = new ScilabDouble(a);
        assertTrue(aOriginal.isReal() == true);
        assertTrue(aOriginal.isEmpty() == false);

        ScilabDouble adouble2 = new ScilabDouble(a);
        assertTrue(adouble.equals(adouble2) == true);

    }

}
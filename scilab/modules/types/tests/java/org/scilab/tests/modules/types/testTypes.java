/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Sylvestre LEDRU
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
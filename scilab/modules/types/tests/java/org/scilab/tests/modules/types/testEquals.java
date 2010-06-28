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

import java.util.Arrays;

import org.scilab.modules.types.scilabTypes.ScilabType;
import org.scilab.modules.types.scilabTypes.ScilabDouble;
import org.scilab.modules.types.scilabTypes.ScilabInteger;
import org.scilab.modules.types.scilabTypes.ScilabBoolean;
import org.scilab.modules.types.scilabTypes.ScilabString;
import org.scilab.modules.types.scilabTypes.ScilabList;

public class testEquals {

	@Test
    public void compareDoubleTest() throws NullPointerException {
        double [][]a={{21.2, 22.0, 42.0, 39.0},{23.2, 24.0, 44.0, 40.0}};
        double [][]b={{21.2, 22.0, 42.0, 39.0},{23.2, 24.0, 44.0, 40.0}};
        double [][]c={{42,43},{21,22}};
        ScilabDouble aMatrix = new ScilabDouble(a);
        ScilabDouble bMatrix = new ScilabDouble(b);
        ScilabDouble cMatrix = new ScilabDouble(c);
        assert aMatrix.equals(bMatrix) == true;
        assert bMatrix.equals(aMatrix) == true;
        assert cMatrix.equals(aMatrix) == false;
        assert Arrays.deepEquals(aMatrix.getRealPart(), bMatrix.getRealPart()) == true;
    }


	@Test
    public void compareIntegerTest() throws NullPointerException {
        int [][]a={{32,42,41}, {12,13,32}};
        int [][]b={{32,42,41}, {12,13,32}};
        int [][]c={{42,43},{21,22}};
        ScilabInteger aMatrix = new ScilabInteger(a, true);
        ScilabInteger bMatrix = new ScilabInteger(b, true);
        ScilabInteger cMatrix = new ScilabInteger(c, true);
        assert aMatrix.equals(bMatrix) == true;
        assert bMatrix.equals(aMatrix) == true;
        assert cMatrix.equals(aMatrix) == false;
        assert Arrays.deepEquals(aMatrix.getData(), bMatrix.getData()) == true;
    }

	@Test
    public void compareBooleanTest() throws NullPointerException {
        boolean [][]a={{true,false,true}, {true,true,true}};
        boolean [][]b={{true,false,true}, {true,true,true}};
        boolean [][]c={{true,false},{false,true}};
        ScilabBoolean aMatrix = new ScilabBoolean(a);
        ScilabBoolean bMatrix = new ScilabBoolean(b);
        ScilabBoolean cMatrix = new ScilabBoolean(c);
        assert aMatrix.equals(bMatrix) == true;
        assert bMatrix.equals(aMatrix) == true;
        assert cMatrix.equals(aMatrix) == false;
        assert Arrays.deepEquals(aMatrix.getData(), bMatrix.getData()) == true;
    }

	@Test
    public void compareStringTest() throws NullPointerException {
        String [][]a={{"This","is","my","string"},{"and","I want to", "compare"," them"}};
        String [][]b={{"This","is","my","string"},{"and","I want to", "compare"," them"}};
        String [][]c={{"Wrong","string"},{"right","string"}};
        ScilabString aMatrix = new ScilabString(a);
        ScilabString bMatrix = new ScilabString(b);
        ScilabString cMatrix = new ScilabString(c);
        assert aMatrix.equals(bMatrix) == true;
        assert bMatrix.equals(aMatrix) == true;
        assert cMatrix.equals(aMatrix) == false;
        assert Arrays.deepEquals(aMatrix.getData(), bMatrix.getData()) == true;
    }

	@Test
    public void compareListTest() throws NullPointerException {
        ScilabList data = new ScilabList();
        data.add(new ScilabDouble(2));
        data.add(new ScilabDouble(51));
        data.add(new ScilabString("hello"));
        ScilabList data2 = new ScilabList();
        data2.add(new ScilabDouble(2));
        data2.add(new ScilabDouble(51));
        data2.add(new ScilabString("hello"));
        ScilabList data3 = new ScilabList();
        data3.add(new ScilabDouble(2));
        data3.add(new ScilabDouble(42));
        data3.add(new ScilabString("hello"));
        assert data.equals(data2) == true;
        assert data.equals(data3) == false;
    }
}

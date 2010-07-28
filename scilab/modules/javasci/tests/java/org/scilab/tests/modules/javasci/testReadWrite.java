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
import org.scilab.modules.javasci.JavasciException.InitializationException;
import org.scilab.modules.types.scilabTypes.ScilabType;
import org.scilab.modules.types.scilabTypes.ScilabDouble;
import org.scilab.modules.types.scilabTypes.ScilabBoolean;
import org.scilab.modules.types.scilabTypes.ScilabString;

public class testReadWrite {

	@Test
	public void putAndGetDoubleTest() throws NullPointerException, InitializationException {
        Scilab sci = new Scilab();
        sci.open();
        double [][]a={{21.2, 22.0, 42.0, 39.0},{23.2, 24.0, 44.0, 40.0}};
        ScilabDouble aOriginal = new ScilabDouble(a);
        sci.put("a",aOriginal);
        assert sci.exec("somme = sum(a);") == true;

        ScilabDouble aFromScilab = (ScilabDouble)sci.get("a");

        assert aFromScilab.equals(aOriginal);

    }

	@Test
	public void putAndGetComplexDoubleTest() throws NullPointerException, InitializationException {
        Scilab sci = new Scilab();
        sci.open();
        double [][]a={{21.2, 22.0, 42.0, 39.0},{23.2, 24.0, 44.0, 40.0}};
        double [][]aImg={{212.2, 221.0, 423.0, 393.0},{234.2, 244.0, 441.0, 407.0}};
        System.out.println("ici");
        ScilabDouble aOriginal = new ScilabDouble(a, aImg);
        System.out.println("ici 2"+aOriginal);
        sci.put("a",aOriginal);
        System.out.println("ici 2.0");

        ScilabDouble aFromScilab = (ScilabDouble)sci.get("a");
        System.out.println("ici 3"+aFromScilab);
        assert aFromScilab.equals(aOriginal);

    }



	@Test
	public void putAndGetBooleanTest() throws NullPointerException, InitializationException {
        Scilab sci = new Scilab();
        sci.open();
        boolean [][]a={{true, true, false, false},{true, false, true, false}};
        ScilabBoolean aOriginal = new ScilabBoolean(a);
        sci.put("a",aOriginal);
        assert sci.exec("somme = sum(a);") == true;

        ScilabBoolean aFromScilab = (ScilabBoolean)sci.get("a");

        assert aFromScilab.equals(aOriginal);

    }

	@Test
	public void putAndGetStringTest() throws NullPointerException, InitializationException {
        Scilab sci = new Scilab();
        sci.open();
        String [][]a={{"String1", "String2", "String3", "String4"},
					  {"String5", "String6", "String7", "String8"}};
        ScilabString aOriginal = new ScilabString(a);
        sci.put("a",aOriginal);
        assert sci.exec("somme = sum(a);") == false;

        ScilabString aFromScilab = (ScilabString)sci.get("a");

        assert aFromScilab.equals(aOriginal);

    }

}

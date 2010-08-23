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
import org.scilab.modules.types.scilabTypes.ScilabInteger;

public class testReadWriteInteger {
	private Scilab sci;

	/* 
	 * This method will be called for each test.
	 * with @AfterMethod, this ensures that all the time the engine is closed
	 * especially in case of error.
	 * Otherwise, the engine might be still running and all subsequent tests
	 * would fail.
	 */ 
	@BeforeMethod
	public void open() throws NullPointerException, InitializationException {
		sci = new Scilab();
        assert sci.open() == true;
	}

	@Test(sequential = true)
	public void putAndGetInteger8UnsignedTest() throws NullPointerException, InitializationException {

        byte [][]a={{21, 22, 42, 39},{23, 24, 44, 40}};
        ScilabInteger aOriginal = new ScilabInteger(a, true); /* unsigned */
        sci.put("b",aOriginal);
		//        assert sci.exec("somme = sum(a);") == true;

        ScilabInteger aFromScilab = (ScilabInteger)sci.get("b");

        assert aFromScilab.equals(aOriginal);
    }

	@Test(sequential = true)
	public void putAndGetInteger8SignedTest() throws NullPointerException, InitializationException {
        byte [][]a={{-21, 22, -42, 39},{23, -24, -44, 40}};
        ScilabInteger aOriginal = new ScilabInteger(a, false); /* signed */
        sci.put("b",aOriginal);
		//        assert sci.exec("somme = sum(a);") == true;

        ScilabInteger aFromScilab = (ScilabInteger)sci.get("b");

        assert aFromScilab.equals(aOriginal);

    }

	@Test(sequential = true)
	public void putAndGetInteger16UnsignedTest() throws NullPointerException, InitializationException {
        short [][]a={{21, 22, 42, 39},{23, 24, 44, 40}};
        ScilabInteger aOriginal = new ScilabInteger(a, true); /* unsigned */
        sci.put("b",aOriginal);

        ScilabInteger aFromScilab = (ScilabInteger)sci.get("b");

        assert aFromScilab.equals(aOriginal);

    }

	@Test(sequential = true)
	public void putAndGetInteger16SignedTest() throws NullPointerException, InitializationException {
        short [][]a={{-21, 22, -42, 39},{23, -24, -44, 40}};
        ScilabInteger aOriginal = new ScilabInteger(a, false); /* signed */
        sci.put("b",aOriginal);
		//        assert sci.exec("somme = sum(a);") == true;

        ScilabInteger aFromScilab = (ScilabInteger)sci.get("b");

        assert aFromScilab.equals(aOriginal);

    }

	@Test(sequential = true)
	public void putAndGetInteger32UnsignedTest() throws NullPointerException, InitializationException {

        int [][]a={{21, 22, 42, 39},{23, 24, 44, 40}};
        ScilabInteger aOriginal = new ScilabInteger(a, true); /* unsigned */

        sci.put("b",aOriginal);
		//        assert sci.exec("somme = sum(a);") == true;

        ScilabInteger aFromScilab = (ScilabInteger)sci.get("b");

        assert aFromScilab.equals(aOriginal);

    }

	@Test(sequential = true)
	public void putAndGetInteger32SignedTest() throws NullPointerException, InitializationException {
        int [][]a={{-21, 22, -42, 39},{23, -24, -44, 40}};
        ScilabInteger aOriginal = new ScilabInteger(a, false); /* signed */
        sci.put("b",aOriginal);
		//        assert sci.exec("somme = sum(a);") == true;

        ScilabInteger aFromScilab = (ScilabInteger)sci.get("b");

        assert aFromScilab.equals(aOriginal);

    }

	//	@Test(sequential = true)
	// Will be unblocked for Scilab 6
	public void putAndGetInteger64UnsignedTest() throws NullPointerException, InitializationException {
        long [][]a={{21, 22, 42, 39},{23, 24, 44, 40}};
        ScilabInteger aOriginal = new ScilabInteger(a, true); /* unsigned */
        sci.put("b",aOriginal);
        assert sci.exec("somme = sum(a);") == true;

        ScilabInteger aFromScilab = (ScilabInteger)sci.get("b");

        assert aFromScilab.equals(aOriginal);


    }

	//	@Test(sequential = true)
	// Will be unblocked for Scilab 6
	public void putAndGetInteger64SignedTest() throws NullPointerException, InitializationException {
        long [][]a={{-21, 22, -42, 39},{23, -24, -44, 40}};
        ScilabInteger aOriginal = new ScilabInteger(a, false); /* signed */
        sci.put("b",aOriginal);
        assert sci.exec("somme = sum(a);") == true;

        ScilabInteger aFromScilab = (ScilabInteger)sci.get("b");

        assert aFromScilab.equals(aOriginal);

    }
	
	/**
	 * See #open()
	 */
	@AfterMethod
	public void close() {
		sci.close();
		
	}
}
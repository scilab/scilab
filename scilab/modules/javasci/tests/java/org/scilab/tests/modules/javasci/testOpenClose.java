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

public class testOpenClose {
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
    public void openTest() throws NullPointerException, InitializationException {
// @TODO: uncomment this
    }

	@Test(sequential = true)
    public void multipleOpenCloseTest() throws NullPointerException, InitializationException {
        assert sci.close() == true;
        assert sci.open() == true;
        assert sci.close() == true;
// @TODO: uncomment this
//        sci.close();
    }

	/**
	 * See #open()
	 */
	@AfterMethod
	public void close() {
		sci.close();
		
	}
}
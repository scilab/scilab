/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Clément DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.tests.modules.commons;

import java.io.File;
import java.io.IOException;

import org.scilab.modules.commons.ScilabConstants;
import org.testng.annotations.BeforeTest;
import org.testng.annotations.Test;

/**
 * Check the access of the {@link ScilabConstants} public fields
 */
public class Constants {

    @BeforeTest
    public void loadLibrary() {
        System.loadLibrary("scilab");
    }
	
	@Test
	public void checkTmp() throws SecurityException, NoSuchFieldException, IllegalArgumentException, IllegalAccessException, IOException {
		assert ScilabConstants.TMPDIR != null;
		assert ScilabConstants.TMPDIR.isDirectory();
		
		final File tmpFile = new File(ScilabConstants.TMPDIR, "tmpFile");
		assert tmpFile.createNewFile();
		assert tmpFile.exists();
		assert tmpFile.delete();
	}
	
	@Test
	public void checkSci() throws SecurityException, NoSuchFieldException, IllegalArgumentException, IllegalAccessException {
		assert ScilabConstants.SCI != null;
		assert ScilabConstants.SCI.isDirectory();
		
		final File copying = new File(ScilabConstants.SCI, "COPYING");
		assert copying.exists();
	}
	
	@Test
	public void checkSciHome() throws SecurityException, NoSuchFieldException, IllegalArgumentException, IllegalAccessException, IOException {
		assert ScilabConstants.SCIHOME != null;
		assert ScilabConstants.SCIHOME.exists();
		assert ScilabConstants.SCIHOME.isDirectory();
		
		final File homeFile = new File(ScilabConstants.SCIHOME, "homeFile");
		assert homeFile.createNewFile();
		assert homeFile.exists();
		assert homeFile.delete();
	}
}

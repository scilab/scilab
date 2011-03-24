/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Clément DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.tests.modules.xcos.utils;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;

import org.scilab.modules.xcos.utils.XcosFileType;
import org.testng.annotations.Test;

/**
 * Test the {@link XcosFileType} class.
 */
public class FileTypeTest {
	private static final String XcosFileHeader = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
	 
	
	@Test
	public void checkSupportedType() {
		assert XcosFileType.values().length == 4;
		assert XcosFileType.getDefault() == XcosFileType.XCOS;
		assert XcosFileType.getScilabFileType() == XcosFileType.HDF5;
	}
	
	@Test(dependsOnMethods = { "checkSupportedType" })
	public void checkNullField() {
		for (XcosFileType type : XcosFileType.values()) {
			assert type.getExtension() != null;
			assert type.getDescription() != null;
		}
	}
	
	@Test(dependsOnMethods = { "checkNullField" })
	public void checkExtension() {
		for (XcosFileType type : XcosFileType.values()) {
			assert type.getDottedExtension().compareTo("." + type.getExtension()) == 0;
			assert type.getFileMask().compareTo("*" + type.getDottedExtension()) == 0;
		}
	}
	
	@Test(dependsOnMethods = { "checkExtension" })
	public void validateFindFileType() throws IOException {
		for (XcosFileType type : XcosFileType.values()) {
			File tmp = File.createTempFile("xcosTest", type.getDottedExtension());
			
			if (type != XcosFileType.XCOS) {
				assert type == XcosFileType.findFileType(tmp);
			} else {
				assert XcosFileType.findFileType(tmp) == null;
			}
			
			tmp.delete();
		}
	}
	
	@Test(dependsOnMethods = { "validateFindFileType" })
	public void validateXcosFindFileType() throws IOException {
		File tmp = File.createTempFile("xcosTest", XcosFileType.XCOS
				.getDottedExtension());
		FileOutputStream stream = new FileOutputStream(tmp);
		stream.write(XcosFileHeader.getBytes());
		stream.close();
		
		assert XcosFileType.XCOS == XcosFileType.findFileType(tmp);
		
		tmp.delete();
	}
	
}

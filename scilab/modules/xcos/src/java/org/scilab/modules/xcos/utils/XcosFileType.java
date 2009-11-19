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

package org.scilab.modules.xcos.utils;

import java.io.File;

/**
 * All the filetype recognized by Xcos.
 */
public enum XcosFileType {
	COSF("cosf"),
	COS("cos"),
	XCOS("xcos"),
	HF5("h5"),
	UNKNOW("");
	
	
	private String extension;
	
	XcosFileType (String extension) {
		this.extension = extension;
	}
	
	/**
	 * @return the extension
	 */
	public String getExtension() {
		return extension;
	}
	
	/**
	 * Find a filetype by the filename extension
	 * @param theFile Current file
	 * @return The determined filetype
	 */
	public static XcosFileType findFileType(File theFile) {
		int dotPos = theFile.getName().lastIndexOf('.');
		String extension = "";

		if (dotPos > 0 && dotPos <= theFile.getName().length() - 2) {
			extension = theFile.getName().substring(dotPos + 1);
		}
		
		for (XcosFileType currentFileType : XcosFileType.values()) {
			if (extension.equals(currentFileType.extension)) {
				return currentFileType;
			}
		}
		
		return XcosFileType.UNKNOW;
	}
	
	/** 
	 * @return the Xcos default filetype
	 */
	public XcosFileType getDefault() {
		return XcosFileType.XCOS;
	}
	
	/** 
	 * @return the Scilab default filetype
	 */
	public XcosFileType getScilabFileType() {
		return XcosFileType.HF5;
	}
}

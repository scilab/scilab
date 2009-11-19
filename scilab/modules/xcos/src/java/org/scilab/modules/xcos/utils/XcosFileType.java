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
import java.io.IOException;

import org.scilab.modules.action_binding.InterpreterManagement;

/**
 * All the filetype recognized by Xcos.
 */
public enum XcosFileType {
	COSF("cosf") {
		public File exportToHdf5(File arg0) {
			File tempOutput = null;
			try {
				tempOutput = File.createTempFile("xcos", XcosFileType.HDF5.getDottedExtension());
				String cmd = "exec(\"" + arg0.getAbsolutePath() + "\", -1);";
				cmd += "export_to_hdf5(\"" + tempOutput.getAbsolutePath() + "\", \"scs_m\");";
				cmd += "xcosNotify(\"" + tempOutput.getAbsolutePath() + "\");";
				System.err.println(cmd);
				InterpreterManagement.requestScilabExec(cmd);
			} catch (IOException e) {
				e.printStackTrace();
			}
			Signal.wait(tempOutput.getAbsolutePath());
			return tempOutput;
		}
	},
	COS("cos") {
		public File exportToHdf5(File arg0) {
			File tempOutput = null;
			try {
				tempOutput = File.createTempFile("xcos", XcosFileType.HDF5.getDottedExtension());
				String cmd = "load(\"" + arg0.getAbsolutePath() + "\");";
				cmd += "export_to_hdf5(\"" + tempOutput.getAbsolutePath() + "\", \"scs_m\");";
				cmd += "xcosNotify(\"" + tempOutput.getAbsolutePath() + "\");";
				System.err.println(cmd);
				InterpreterManagement.requestScilabExec(cmd);
			} catch (IOException e) {
				e.printStackTrace();
			}
			Signal.wait(tempOutput.getAbsolutePath());
			return tempOutput;
		}
	},
	XCOS("xcos"),
	HDF5("h5") {
		public File exportToHdf5(File arg0) {
			return arg0;
		}
	},
	UNKNOW("");
	
	
	private String extension;
	
	XcosFileType (String extension) {
		this.extension = extension;
	}
	
	/**
	 * @return the extension prefixed with a dot
	 */
	public String getDottedExtension() {
		return "." + extension;
	}
	
	/**
	 * @return the raw extension
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
	public static XcosFileType getDefault() {
		return XcosFileType.XCOS;
	}
	
	/** 
	 * @return the Scilab default filetype
	 */
	public static XcosFileType getScilabFileType() {
		return XcosFileType.HDF5;
	}
	
	/**
	 * Convert the file passed as an argument to Hdf5.
	 * @param file The file to convert
	 * @return The created file
	 */
	public File exportToHdf5(File file) {
		return null;
	}
}

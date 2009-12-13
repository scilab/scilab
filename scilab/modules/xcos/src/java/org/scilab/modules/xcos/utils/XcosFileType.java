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
	COSF("cosf", XcosMessages.FILE_COSF) {
		public File exportToHdf5(File arg0) {
			File tempOutput = null;
			try {
				tempOutput = File.createTempFile("xcos", XcosFileType.HDF5.getDottedExtension());
				String cmd = "exec(\"" + arg0.getAbsolutePath() + "\", -1);";
				cmd += "export_to_hdf5(\"" + tempOutput.getAbsolutePath() + "\", \"scs_m\");";
				cmd += "xcosNotify(\"" + tempOutput.getAbsolutePath() + "\");";
				InterpreterManagement.requestScilabExec(cmd);
			} catch (IOException e) {
				e.printStackTrace();
			}
			Signal.wait(tempOutput.getAbsolutePath());
			return tempOutput;
		}
	},
	COS("cos", XcosMessages.FILE_COS) {
		public File exportToHdf5(File arg0) {
			File tempOutput = null;
			try {
				tempOutput = File.createTempFile("xcos", XcosFileType.HDF5.getDottedExtension());
				String cmd = "load(\"" + arg0.getAbsolutePath() + "\");";
				cmd += "export_to_hdf5(\"" + tempOutput.getAbsolutePath() + "\", \"scs_m\");";
				cmd += "xcosNotify(\"" + tempOutput.getAbsolutePath() + "\");";
				InterpreterManagement.requestScilabExec(cmd);
			} catch (IOException e) {
				e.printStackTrace();
			}
			Signal.wait(tempOutput.getAbsolutePath());
			return tempOutput;
		}
	},
	HDF5("h5", XcosMessages.FILE_HDF5) {
		public File exportToHdf5(File arg0) {
			return arg0;
		}
	},
	XCOS("xcos", XcosMessages.FILE_XCOS),
	UNKNOW("", "");
	
	
	private String extension;
	private String description;
	
	/**
	 * Default constructor
	 * @param extension file extension (without the dot)
	 * @param description file description
	 */
	XcosFileType (String extension, String description) {
		this.extension = extension;
		this.description = description;
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
	 * @return the mask of this file
	 */
	public String getFileMask() {
		return "*." + getExtension();
	}
	
	/**
	 * @return the file description
	 */
	public String getDescription() {
		return description;
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
			if (extension.compareToIgnoreCase(currentFileType.extension) == 0) {
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
	    throw new Error("Not implemented operation");
	}
	
	/**
	 * Get a valid file mask (useable by file selector)
	 * @return A valid file mask
	 */
	public static String[] getValidFileMask() {
	    String[] result = new String[XcosFileType.values().length-1];
	    
	    for (int i = 0; i < result.length; i++) {
		result[i] = XcosFileType.values()[i].getFileMask();
	    }
	    
	    return result;
	}
	
	/**
	 * Get a valid file description (useable by file selector)
	 * @return A valid file mask
	 */
	public static String[] getValidFileDescription() {
	    String[] result = new String[XcosFileType.values().length-1];
	    
	    for (int i = 0; i < result.length; i++) {
		result[i] = XcosFileType.values()[i].getDescription() + " (*." + XcosFileType.values()[i].getExtension() + ")";
	    }
	    
	    return result;
	}
}

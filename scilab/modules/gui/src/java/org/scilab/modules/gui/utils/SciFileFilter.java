
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.utils;

import java.io.File;

import javax.swing.filechooser.FileFilter;

/**
 * Generic file filter used for Scilab file selection GUIs
 * @author Vincent COUVERT
 */
public class SciFileFilter extends FileFilter {
	
	private String mask;
	private String description;

	/**
	 * Constructor
	 * @param fileMask the file maks to apply
	 */
	public SciFileFilter(String fileMask) {
		// Remove the * at the beginning
		mask = fileMask.substring(1);
		if (mask.equals(".*")) {
			mask = "";
		} else if (mask.equals(".sci")) {
			description = "Scilab SCI file";
		} else if (mask.equals(".sce")) {
			description = "Scilab SCE file";
		} else if (mask.equals(".bin")) {
			description = "Scilab binary file";
		} else {
			description = "All " + fileMask + " files";
		}
	}

	/**
	 * Accept a file or not ?
	 * @param pathname the file to test
	 * @return true if the file matches the mask
	 * @see java.io.FileFilter#accept(java.io.File)
	 */
	public boolean accept(File pathname) {
		if (pathname.isDirectory()) {
			return true;
		}
		return pathname.getAbsolutePath().endsWith(mask);
	}

	/**
	 * Get the description of this filter
	 * @return the description of this filter
	 * @see javax.swing.filechooser.FileFilter#getDescription()
	 */
	public String getDescription() {
		// TODO Auto-generated method stub
		return description;
	}
	
}

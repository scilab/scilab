/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent Couvert
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.utils;

import java.io.File;
import javax.swing.filechooser.FileFilter;

import org.scilab.modules.gui.filechooser.FileChooserInfos;

/**
 * Generic file filter used for Scilab file selection GUIs
 * @author Vincent COUVERT
 * @author Sylvestre KOUMAR
 */
public class SciFileFilter extends FileFilter {

	private String mask;
	private String description;
	private int filterIndex;
	//private int lastFilterIndex;

	/**
	 * Constructor
	 * @param fileMask the file mask to apply
	 * @param maskdescription description of each mask
	 * @param filterIndex index the mask from the mask matrix
	 */
	public SciFileFilter(String fileMask, String maskdescription, int filterIndex) {
		
		if (maskdescription == null) {

			if (fileMask.equals("*.sci")) {
				description = "Scilab SCI files (*.sci)";
			} else if (fileMask.equals("*.sce")) {
				description = "Scilab SCE files (*.sce)";
			} else if (fileMask.equals("*.bin")) {
				description = "Scilab binary files (*.bin)";
			} else if (fileMask.equals("*.sc*")) {
				description = "All Scilab files (*.sc*)";
			} else if (fileMask.equals("*.cos*")) {
				description = "Scicos files (*.cos*)";
			} else {
				description = "All " + fileMask + " files";
			}

		} else {
			//If the mask description is filled
			//we use those descriptions given by the user
			this.description = maskdescription;
		}

		// Create a regexp
		mask = fileMask.replaceAll("\\.", "\\\\."); // Point is a special regexp character
		mask = mask.replaceAll("\\*", ".\\*");
		
		this.filterIndex = filterIndex;
		//this.lastFilterIndex = lastFilterIndex;
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
		
		if (mask.equals("")) { // Bug 2861: have to return true for all files if no mask given
			return true;
		} else {			
			
			int selectedIndex = this.filterIndex + 1;
			FileChooserInfos.getInstance().setFilterIndex(selectedIndex);
			//System.out.println("JAVA this.filterIndex: "+selectedIndex);			
			return pathname.getAbsolutePath().matches(mask);
		}
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
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - INRIA - Sylvestre KOUMAR
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.gui.bridge.filechooser;

import java.io.File;
import java.util.Enumeration;
import java.util.Hashtable;
import javax.swing.filechooser.FileFilter;

/**
 * This Class manage file mask for the graphic export filechooser
 * @author Sylvestre KOUMAR
 */
public class FileMask extends FileFilter {
	
	private Hashtable filters = null;
	private String description = null;
	private String fullDescription = null;
	private boolean useExtensionsInDescription = true;	

	/**
	 * Create a file filter
	 * If there is no filter added, display all files
	 */
	public FileMask() {
		this.filters = new Hashtable();
	}

	/**
	 * Creates a file filter that accepts the given file type.
	 * Ex: "jpg", "JPEG Images"
	 */
	public FileMask(String extension, String description) {
		this();
		if(extension!=null) addExtension(extension);
		if(description!=null) setDescription(description);
	}

	/**
	 * Allow the file to be display
	 * @param f a File
	 */
	public boolean accept(File f) {
		if(f != null) {
			if(f.isDirectory() || filters.size()==0) {
				return true;
			}
			String extension = getExtension(f);
			if(extension != null && filters.get(getExtension(f)) != null) {
				return true;
			};
		}
		return false;
	}
	
	/**
	 * Get the file's extension
	 * @param f a File
	 */
	public String getExtension(File f) {
		if(f != null) {
			String filename = f.getName();
			int i = filename.lastIndexOf('.');
			if(i>0 && i<filename.length()-1) {
				return filename.substring(i+1).toLowerCase();
			};
		}
		return null;
	}
	
	/**
	 * Add a file type to the file mask
	 * @param extension the new type
	 */
	public void addExtension(String extension) {
		if(filters == null) {
			filters = new Hashtable(5);
		}
		filters.put(extension.toLowerCase(), this);
		fullDescription = null;
	}
	
	/**
	 * Get description of the mask
	 */
	public String getDescription() {
		if(fullDescription == null) {
			if(description == null || isExtensionListInDescription()) {
				fullDescription = description==null ? "(" : description + " ("; 
				Enumeration extensions = filters.keys();
				if(extensions != null) {
					fullDescription += "." + (String) extensions.nextElement();
					while (extensions.hasMoreElements()) {
						fullDescription += ", ." + (String) extensions.nextElement();
					}
				}
				fullDescription += ")";
			}
			else {
				fullDescription = description;
			}
		}
		return fullDescription;
	}
	
	/**
	 * Set the description for the mask
	 */
	public void setDescription(String description) {
		this.description = description;
		fullDescription = null;
	}

	/**
	 * Allow to display description
	 */
	public boolean isExtensionListInDescription() {
		return useExtensionsInDescription;
	}

	public void clearExtensions() {
		filters.clear();
	}


} 
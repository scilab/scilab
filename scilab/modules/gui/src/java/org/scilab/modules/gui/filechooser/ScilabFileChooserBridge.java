/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.filechooser;

import org.scilab.modules.gui.bridge.filechooser.SwingScilabExportFileChooser;
import org.scilab.modules.gui.bridge.filechooser.SwingScilabFileChooser;

/**
 * Bridge dedicated to Scilab file chooser GUIs
 * @author Vincent COUVERT
 */
public class ScilabFileChooserBridge {
	
	/**
	 * Constructor
	 */
	protected ScilabFileChooserBridge() {
		throw new UnsupportedOperationException(); /* Prevents calls from subclass */
	}

	/**
	 * Creates a new FileChooser
	 * @return the created FileChooser
	 */
	public static SimpleFileChooser createFileChooser() {
		return new SwingScilabFileChooser();
	}
	
	/**
	 * Creates a new ExportFileChooser
	 * @param figureID id of the exported figure
	 * @return the created ExportFileChooser
	 */
	public static SimpleFileChooser createExportFileChooser(int figureID) {
		return new SwingScilabExportFileChooser(figureID);
	}
	
	/**
	 * Set the title of the file chooser
	 * @param fileChooser the file chooser we want to set the title of
	 * @param title the title to set
	 */
	public static void setTitle(FileChooser fileChooser, String title) {
		fileChooser.getAsSimpleFileChooser().setTitle(title);
	}
	
	/**
	 * Set the mask for files to choose
	 * @param fileChooser the file chooser we want to set the mask of
	 * @param mask the mask to set
	 */
	public static void setMask(FileChooser fileChooser, String mask) {
		fileChooser.getAsSimpleFileChooser().setMask(mask);
	}
	
	/**
	 * Set the initial directory used for file search
	 * @param fileChooser the file chooser we want to set the initial directory of
	 * @param path the default path
	 */
	public static void setInitialDirectory(FileChooser fileChooser, String path) {
		fileChooser.getAsSimpleFileChooser().setInitialDirectory(path);
	}

	/**
	 * Display this chooser and wait for user selection 
	 * @param fileChooser the file chooser we want to display
	 */
	public static void displayAndWait(FileChooser fileChooser) {
		fileChooser.getAsSimpleFileChooser().displayAndWait();
	}
	
	/**
	 * Get the number of files selected
	 * @param fileChooser the file chooser we want to get the selection size of
	 * @return the number of files selected
	 */
	public static int getSelectionSize(FileChooser fileChooser) {
		return fileChooser.getAsSimpleFileChooser().getSelectionSize();
	}
	
	/**
	 * Get the names of selected files
	 * @param fileChooser the file chooser we want to get the selection of 
	 * @return the names of selected files
	 */
	public static String[] getSelection(FileChooser fileChooser) {
		return fileChooser.getAsSimpleFileChooser().getSelection();
	}
	
	/**
	 * Set the flag indicating that we want only select directories
	 * @param fileChooser the file chooser we want to set the flag of 
	 */
	public static void setDirectorySelectionOnly(FileChooser fileChooser) {
		fileChooser.getAsSimpleFileChooser().setDirectorySelectionOnly();
	}

	/**
	 * Set the flag indicating that we want only select files
	 * @param fileChooser the file chooser we want to set the flag of 
	 */
	public static void setFileSelectionOnly(FileChooser fileChooser) {
		fileChooser.getAsSimpleFileChooser().setFileSelectionOnly();
	}
}

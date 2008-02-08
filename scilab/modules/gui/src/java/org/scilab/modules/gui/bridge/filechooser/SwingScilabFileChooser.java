
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.bridge.filechooser;

import java.io.File;
import java.util.StringTokenizer;

import javax.swing.JFileChooser;
import javax.swing.JFrame;

import org.scilab.modules.gui.filechooser.SimpleFileChooser;
import org.scilab.modules.gui.utils.SciFileFilter;

/**
 * Swing implementation of a Scilab File ChooserS
 * @author Vincent COUVERT
 */
public class SwingScilabFileChooser extends JFileChooser implements SimpleFileChooser {
	
	private static final long serialVersionUID = 1L;

	private int elementId;

	private String[] selection; 
	
	private int selectionSize; 

	/**
	 * Default constructor
	 */
	public SwingScilabFileChooser() {
		super();
	}

	/**
	 * Set the title of the file chooser
	 * @param title the title to set
	 */
	public void setTitle(String title) {
		super.setDialogTitle(title);
	}
	
	/**
	 * Set the mask for files to choose
	 * @param mask the mask to set
	 */
	public void setMask(String mask) {
		super.addChoosableFileFilter(new SciFileFilter(mask));
	}
	
	/**
	 * Set the initial directory used for file search
	 * @param path the default path
	 */
	public void setInitialDirectory(String path) {
		// When empty string given
		if (path.length() == 0) {
			return;
		}
		// Replace beginning of the path if is an environment variable
		String newPath = path;
		StringTokenizer tok = new StringTokenizer(path, File.separator);
		String firstToken = tok.nextToken();
		if (firstToken != null && System.getenv(firstToken) != null)  {
			newPath = newPath.replaceFirst(firstToken, System.getenv(firstToken));
		}
		
		super.setCurrentDirectory(new File(newPath));
	}
	
	/**
	 * Set the element id for this file chooser
	 * @param id the id of the corresponding file chooser object
	 */
	public void setElementId(int id) {
		this.elementId = id;
	}
	
	/**
	 * Get the element id for this chooser
	 * @return id the id of the corresponding chooser object
	 */
	public int getElementId() {
		return this.elementId;
	}
	
	/**
	 * Display this chooser and wait for user selection 
	 */
	public void displayAndWait() {
		JFrame parentFrame = new JFrame();
		int returnValue = this.showOpenDialog(parentFrame);
		if (returnValue == APPROVE_OPTION) {
			if (this.isMultiSelectionEnabled()) {
				File[] files = this.getSelectedFiles();
				selection = new String[files.length];
				selectionSize = files.length;
				for (int i = 0; i < files.length; i++) {
					selection[i] = files[i].getAbsolutePath();
				}
				
			} else {
				File file = this.getSelectedFile();
				selection = new String[1];
				selection[0] = file.getAbsolutePath();
				selectionSize = 1;
			}
		} else {
			selection = null;
			selectionSize = 0;
		}
	}
	
	/**
	 * Get the number of files selected
	 * @return the number of files selected
	 */
	public int getSelectionSize() {
		return selectionSize;
	}
	
	/**
	 * Get the names of selected files
	 * @return the names of selected files
	 */
	public String[] getSelection() {
		return selection;
	}
	
	/**
	 * Set the flag indicating that we want only select directories
	 */
	public void setDirectorySelectionOnly() {
		setFileSelectionMode(DIRECTORIES_ONLY);
	}

	/**
	 * Set the flag indicating that we want only select files
	 */
	public void setFileSelectionOnly() {
		setFileSelectionMode(FILES_ONLY);
	}
	
}

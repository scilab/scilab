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

import org.scilab.modules.gui.bridge.ScilabBridge;
import org.scilab.modules.gui.uielement.ScilabUIElement;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.gui.utils.UIElementMapper;

/**
 * Interface for file chooser used by Scilab function such as uigetfile 
 * @author Vincent COUVERT
 */
public class ScilabFileChooser extends ScilabUIElement implements FileChooser {

	private SimpleFileChooser component;
	
	private String[] selectedFiles;
	
	/**
	 * Constructor
	 * @param isExportFileChooser if is an graphic export file chooser
	 * @param figureId id of the exported figure
	 */
	protected ScilabFileChooser(boolean isExportFileChooser, int figureId) {
		if (isExportFileChooser) {
			component = ScilabBridge.createExportFileChooser(figureId);
		} else {
			component = ScilabBridge.createFileChooser();
		}
		component.setElementId(UIElementMapper.add(this));
		
		//setMenuBarId(UIElementMapper.getDefaultId());
	}

	/**
	 * Creates a Scilab FileChooser
	 * @return the created file chooser
	 */
	public static FileChooser createFileChooser() {
		return new ScilabFileChooser(false, 0);
	}
	
	/**
	 * Creates a Scilab ExportFileChooser
	 * @param figureId int
	 * @return the created export file chooser
	 */
	public static FileChooser createExportFileChooser(int figureId) {
		return new ScilabFileChooser(true, figureId);
	}
	

	/**
	 * Gets this Bridge component object
	 * @return this Bridge component object
	 */
	public SimpleFileChooser getAsSimpleFileChooser() {
		return component;
	}

	/**
	 * Set the title of the file chooser
	 * @param title the title to set
	 */
	public void setTitle(String title) {
		ScilabBridge.setTitle(this, title);
	}
	
	/**
	 * Set the mask for files to choose
	 * @param mask the mask to set
	 */
	public void setMask(String mask) {
		ScilabBridge.setMask(this, mask);
	}
	
	/**
	 * Set the initial directory used for file search
	 * @param path the default path
	 */
	public void setInitialDirectory(String path) {
		ScilabBridge.setInitialDirectory(this, path);
	}

	/**
	 * Display this chooser and wait for user selection 
	 */
	public void displayAndWait() {
		ScilabBridge.displayAndWait(this);
	}
	
	/**
	 * Get the number of files selected
	 * @return the number of files selected
	 */
	public int getSelectionSize() {
		return ScilabBridge.getSelectionSize(this);
	}
	
	/**
	 * Get the names of selected files
	 * @return the names of selected files
	 */
	public String[] getSelection() {
		return ScilabBridge.getSelection(this);
	}
	
	/**
	 * Set the flag indicating that we want only select directories
	 */
	public void setDirectorySelectionOnly() {
		ScilabBridge.setDirectorySelectionOnly(this);
	}

	/**
	 * Set the flag indicating that we want only select files
	 */
	public void setFileSelectionOnly() {
		ScilabBridge.setFileSelectionOnly(this);
	}

	/**
	 * Should not be used, just here to implemeent org.scilab.modules.gui.UIElement#draw()
	 */
	public void draw() {
		throw new UnsupportedOperationException();
	}

	/**
	 * Should not be used, just here to implemeent org.scilab.modules.gui.UIElement#getDims()
	 * @return nothing
	 */
	public Size getDims() {
		throw new UnsupportedOperationException();
	}

	/**
	 * Should not be used, just here to implemeent org.scilab.modules.gui.UIElement#draw()
	 * @return nothing
	 */
	public Position getPosition() {
		throw new UnsupportedOperationException();
	}

	/**
	 * Should not be used, just here to implemeent org.scilab.modules.gui.UIElement#draw()
	 * @return nothing
	 */
	public boolean isVisible() {
		throw new UnsupportedOperationException();
	}

	/**
	 * Should not be used, just here to implemeent org.scilab.modules.gui.UIElement#draw()
	 * @param newSize is never used
	 */
	public void setDims(Size newSize) {
		throw new UnsupportedOperationException();
	}

	/**
	 * Should not be used, just here to implemeent org.scilab.modules.gui.UIElement#draw()
	 * @param newPosition is never used
	 */
	public void setPosition(Position newPosition) {
		throw new UnsupportedOperationException();
	}

	/**
	 * Should not be used, just here to implemeent org.scilab.modules.gui.UIElement#draw()
	 * @param newVisibleState is never used
	 */
	public void setVisible(boolean newVisibleState) {
		throw new UnsupportedOperationException();
	}
	
}

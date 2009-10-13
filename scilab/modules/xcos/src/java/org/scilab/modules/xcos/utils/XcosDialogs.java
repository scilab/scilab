/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.utils;

import org.scilab.modules.gui.messagebox.MessageBox;
import org.scilab.modules.gui.messagebox.ScilabMessageBox;

/**
 * Xcos standard dialogs
 * @author Vincent COUVERT
 */
public final class XcosDialogs {

	/**
	 * Constructor
	 */
	private XcosDialogs() {
		
	}
	
	/**
	 * Dialog displayed when no  block is selected and one is needed for action
	 */
	public static void noBlockSelected() {
		MessageBox messageBox = ScilabMessageBox.createMessageBox();
		messageBox.setTitle(XcosMessages.XCOS_ERROR);
		messageBox.setMessage(XcosMessages.NO_BLOCK_SELECTED);
		messageBox.setModal(true);
		messageBox.setIcon("error");
		messageBox.displayAndWait();
	}

	/**
	 * Dialog displayed when saving failed
	 */
	public static void couldNotSaveFile() {
		MessageBox messageBox = ScilabMessageBox.createMessageBox();
		messageBox.setTitle(XcosMessages.XCOS_ERROR);
		messageBox.setMessage(XcosMessages.FAIL_SAVING_DIAGRAM);
		messageBox.setModal(true);
		messageBox.setIcon("error");
		messageBox.displayAndWait();

	}
	
	/**
	 * Dialog displayed when loading failed
	 */
	public static void couldNotLoadFile() {
		MessageBox messageBox = ScilabMessageBox.createMessageBox();
		messageBox.setTitle(XcosMessages.XCOS_ERROR);
		messageBox.setMessage(XcosMessages.FAIL_LOADING_DIAGRAM);
		messageBox.setModal(true);
		messageBox.setIcon("error");
		messageBox.displayAndWait();
	}
}

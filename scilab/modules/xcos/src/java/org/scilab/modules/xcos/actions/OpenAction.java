/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Vincent COUVERT
 * Copyright (C) 2010 - DIGITEO - Clément DAVID
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.actions;

import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;
import java.io.File;

import javax.swing.JFileChooser;
import javax.swing.filechooser.FileFilter;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.base.DefaultAction;
import org.scilab.modules.gui.bridge.filechooser.SwingScilabFileChooser;
import org.scilab.modules.gui.filechooser.ScilabFileChooser;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.xcos.Xcos;
import org.scilab.modules.xcos.utils.XcosFileType;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * File opening management
 */
public final class OpenAction extends DefaultAction {
	/** Name of the action */
	public static final String NAME = XcosMessages.OPEN;
	/** Icon name of the action */
	public static final String SMALL_ICON = "document-open.png";
	/** Mnemonic key of the action */
	public static final int MNEMONIC_KEY = KeyEvent.VK_O;
	/** Accelerator key for the action */
	public static final int ACCELERATOR_KEY = Toolkit.getDefaultToolkit().getMenuShortcutKeyMask();

	/**
	 * Constructor
	 * @param scilabGraph associated Scilab Graph
	 */
	public OpenAction(ScilabGraph scilabGraph) {
		super(scilabGraph);
	}

	/**
	 * Create a menu to add in Scilab Graph menu bar
	 * @param scilabGraph associated Scilab Graph
	 * @return the menu
	 */
	public static MenuItem createMenu(ScilabGraph scilabGraph) {
		return createMenu(scilabGraph, OpenAction.class);
	}

	/**
	 * Create a button to add in Scilab Graph tool bar
	 * @param scilabGraph associated Scilab Graph
	 * @return the button
	 */
	public static PushButton createButton(ScilabGraph scilabGraph) {
		return createButton(scilabGraph, OpenAction.class);
	}

	/**
	 * @param e parameter
	 * @see org.scilab.modules.graph.actions.base.DefaultAction#actionPerformed(java.awt.event.ActionEvent)
	 */
	@Override
	public void actionPerformed(ActionEvent e) {
	    SwingScilabFileChooser fc = ((SwingScilabFileChooser) ScilabFileChooser.createFileChooser().getAsSimpleFileChooser());

		fc.setTitle(XcosMessages.OPEN);
	    fc.setUiDialogType(JFileChooser.OPEN_DIALOG);
	    fc.setMultipleSelection(true);
	    
	    /* Standard files */
	    fc.setAcceptAllFileFilterUsed(true);
	    final FileFilter[] filters = XcosFileType.getValidFilters();
	    for (FileFilter fileFilter : filters) {
	    	fc.addChoosableFileFilter(fileFilter);
		}
	    fc.setFileFilter(filters[0]);

	    int status = fc.showOpenDialog(getGraph(e).getAsComponent());
	    if (status != JFileChooser.APPROVE_OPTION) {
	    	return;
	    }

	    final File onlySelected = fc.getSelectedFile();
	    if (onlySelected != null) {
	    	Xcos.getInstance().open(onlySelected);
	    }
	    
	    final File[] multiSelected = fc.getSelectedFiles();
	    for (File file : multiSelected) {
	    	Xcos.getInstance().open(file);
		}
	}
}

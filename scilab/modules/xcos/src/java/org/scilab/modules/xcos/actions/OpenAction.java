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

import javax.swing.JFileChooser;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.base.DefaultAction;
import org.scilab.modules.gui.bridge.filechooser.SwingScilabFileChooser;
import org.scilab.modules.gui.filechooser.ScilabFileChooser;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.gui.utils.SciFileFilter;
import org.scilab.modules.xcos.Xcos;
import org.scilab.modules.xcos.XcosTab;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.utils.ConfigXcosManager;
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

		/* Standard files */
	    fc.setTitle(XcosMessages.OPEN);
	    fc.setUiDialogType(JFileChooser.OPEN_DIALOG);
	    fc.setMultipleSelection(false);
	    
	    /*
	     * FIXME: why hardcoded values ?  
	     */
	    SciFileFilter xcosFilter = new SciFileFilter("*.xcos", null, 0);
	    SciFileFilter cosFilter = new SciFileFilter("*.cos*", null, 1);
	    SciFileFilter allFilter = new SciFileFilter("*.*", null, 2);
	    fc.addChoosableFileFilter(xcosFilter);
	    fc.addChoosableFileFilter(cosFilter);
	    fc.addChoosableFileFilter(allFilter);
	    fc.setFileFilter(xcosFilter);

	    fc.setAcceptAllFileFilterUsed(false);
	    fc.displayAndWait();

	    if (fc.getSelection() == null || fc.getSelection().length == 0 || fc.getSelection()[0].equals("")) {
		return;
	    }
	    ConfigXcosManager.saveToRecentOpenedFiles(fc.getSelection()[0]);

	    if (getGraph(null) == null) { // Called from palettes
		//save to recentopenedfile while opening from palettes is handle in Xcos.xcos(filename)
		Xcos.xcos(fc.getSelection()[0]);
	    } else {
		((XcosDiagram) getGraph(null)).openDiagramFromFile(fc.getSelection()[0]);
	    }
	    XcosTab.updateRecentOpenedFilesMenu(((XcosDiagram) getGraph(null)));
	}
}

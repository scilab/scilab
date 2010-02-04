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

package org.scilab.modules.xcos.palette.actions;

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
import org.scilab.modules.xcos.palette.XcosPaletteManager;
import org.scilab.modules.xcos.utils.XcosFileType;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Load a diagram on the palette.
 * 
 * It will load each block as an independent block and produce a visible panel
 * similar to {@link XcosPalette}.
 */
public final class LoadAsPalAction extends DefaultAction {
	public static final String NAME = XcosMessages.OPEN;
	public static final String SMALL_ICON = "document-open.png";
	public static final int MNEMONIC_KEY = KeyEvent.VK_O;
	public static final int ACCELERATOR_KEY = Toolkit.getDefaultToolkit().getMenuShortcutKeyMask();

    /**
     * Constructor
     * @param scilabGraph associated Scilab Graph
     */
    public LoadAsPalAction(ScilabGraph scilabGraph) {
    	super(scilabGraph);
    }

    /**
     * Create a menu to add in Scilab Graph menu bar
     * @param scilabGraph associated Scilab Graph
     * @return the menu
     */
    public static MenuItem createMenu(ScilabGraph scilabGraph) {
    	return createMenu(scilabGraph, LoadAsPalAction.class);
    }

    /**
     * Create a button to add in Scilab Graph tool bar
     * @param scilabGraph associated Scilab Graph
     * @return the button
     */
    public static PushButton createButton(ScilabGraph scilabGraph) {
    	return createButton(scilabGraph, LoadAsPalAction.class);
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


	SciFileFilter xcosFilter = new SciFileFilter(XcosFileType.XCOS.getFileMask(), null, 0);
	SciFileFilter cosFilter = new SciFileFilter(XcosFileType.COS.getFileMask() + "*", null, 1);
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

	XcosPaletteManager.loadUserPalette(fc.getSelection()[0]);
    }
}

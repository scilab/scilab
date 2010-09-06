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
import org.scilab.modules.xcos.palette.PaletteManager;
import org.scilab.modules.xcos.palette.model.Category;
import org.scilab.modules.xcos.palette.model.Custom;
import org.scilab.modules.xcos.palette.model.PaletteNode;
import org.scilab.modules.xcos.palette.model.VariablePath;
import org.scilab.modules.xcos.utils.XcosFileType;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Load a diagram on the palette.
 * 
 * It will load each block as an independent block and produce a visible panel
 * similar to {@link XcosPalette}.
 */
public final class LoadAsPalAction extends DefaultAction {
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
	public LoadAsPalAction(final ScilabGraph scilabGraph) {
		super(scilabGraph);
	}

	/**
	 * Create a menu to add in Scilab Graph menu bar
	 * @param scilabGraph associated Scilab Graph
	 * @return the menu
	 */
	public static MenuItem createMenu(final ScilabGraph scilabGraph) {
		return createMenu(scilabGraph, LoadAsPalAction.class);
	}

	/**
	 * Create a button to add in Scilab Graph tool bar
	 * @param scilabGraph associated Scilab Graph
	 * @return the button
	 */
	public static PushButton createButton(final ScilabGraph scilabGraph) {
		return createButton(scilabGraph, LoadAsPalAction.class);
	}

	/**
	 * @param e parameter
	 * @see org.scilab.modules.graph.actions.base.DefaultAction#actionPerformed(java.awt.event.ActionEvent)
	 */
	@Override
	public void actionPerformed(final ActionEvent e) {
		final SwingScilabFileChooser fc = ((SwingScilabFileChooser) ScilabFileChooser.createFileChooser().getAsSimpleFileChooser());

		/* Standard files */
		fc.setTitle(XcosMessages.OPEN);
		fc.setUiDialogType(JFileChooser.OPEN_DIALOG);
		fc.setMultipleSelection(false);


		final SciFileFilter xcosFilter = new SciFileFilter(XcosFileType.XCOS.getFileMask(), null, 0);
		final SciFileFilter cosFilter = new SciFileFilter(XcosFileType.COS.getFileMask() + "*", null, 1);
		final SciFileFilter allFilter = new SciFileFilter("*.*", null, 2);
		fc.addChoosableFileFilter(xcosFilter);
		fc.addChoosableFileFilter(cosFilter);
		fc.addChoosableFileFilter(allFilter);
		fc.setFileFilter(xcosFilter);

		fc.setAcceptAllFileFilterUsed(false);
		fc.displayAndWait();

		if ((fc.getSelection() == null) || (fc.getSelection().length == 0) || fc.getSelection()[0].equals("")) {
			return;
		}

		final String file = fc.getSelection()[0];
		final Custom c = new Custom();
		c.setName(file);
		c.setEnable(true);
		final VariablePath v = new VariablePath();
		v.setPath(file);
		c.setPath(v);

		final Category root = PaletteManager.getInstance().getRoot();
		root.getNode().add(c);
		c.setParent(root);
		PaletteNode.refreshView(c);
	}
}

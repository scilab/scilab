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

package org.scilab.modules.xcos.simulink;

import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;
import java.io.IOException;
import java.util.Set;
import java.util.TreeSet;

import org.apache.commons.logging.LogFactory;
import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.base.DefaultAction;
import org.scilab.modules.gui.bridge.filechooser.SwingScilabFileChooser;
import org.scilab.modules.gui.filechooser.FileChooser;
import org.scilab.modules.gui.filechooser.ScilabFileChooser;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xcos.utils.XcosMessages;

import edu.tum.cs.simulink.builder.SimulinkModelBuildingException;

//import edu.tum.cs.simulink.builder.SimulinkModelBuildingException;

// FIXME: A lot to changes in this file, as it is only
// 			changed ExportAction

/**
 * Diagram import action management
 */
public final class ImportAction extends DefaultAction {
	/** Name of the action */
	public static final String NAME = XcosMessages.IMPORT;
	/** Icon name of the action */
	public static final String SMALL_ICON = "";
	/** Mnemonic key of the action */
	public static final int MNEMONIC_KEY = KeyEvent.VK_L;
	/** Accelerator key for the action */
	public static final int ACCELERATOR_KEY = Toolkit.getDefaultToolkit().getMenuShortcutKeyMask();
	
	/**
	 * Constructor
	 * @param scilabGraph associated Scilab Graph
	 */
	public ImportAction(ScilabGraph scilabGraph) {
		super(scilabGraph);
	}

	/**
	 * Create export menu
	 * @param scilabGraph associated Scilab Graph
	 * @return the menu
	 */
	public static MenuItem createMenu(ScilabGraph scilabGraph) {
		return createMenu(scilabGraph, ImportAction.class);
	}

	/**
	 * Action !!!
	 * @param e parameter
	 * @see org.scilab.modules.graph.actions.base.DefaultAction#actionPerformed(java.awt.event.ActionEvent)
	 */
	@Override
	public void actionPerformed(ActionEvent e) {

		String filename = null;

		FileChooser fc = ScilabFileChooser.createFileChooser();
		
		// The mask and mask description ordered collection
		Set<String> mask = new TreeSet<String>();
		Set<String> maskDesc = new TreeSet<String>();

		/* FIXME : is .mdl the only possible extension ? */
		mask.add(".mdl");

		maskDesc.add("Simulink model");

		// Adds filter that accepts all supported image formats
		//fc.addChoosableFileFilter(new DefaultFileFilter.ImageFileFilter(mxResources.get("allImages")));
				
		((SwingScilabFileChooser) fc.getAsSimpleFileChooser()).addMask(mask.toArray(new String[mask.size()]) , 
			maskDesc.toArray(new String[maskDesc.size()]));
		fc.setTitle(XcosMessages.IMPORT);
		fc.displayAndWait();
		
		if (fc.getSelection() == null || fc.getSelection().length == 0 || fc.getSelection()[0].equals("")) {
			return;
		}
		
		filename = fc.getSelection()[0];
		
		try {
			ImportMdl.fromFile(filename);
		} catch (IOException e1) {
			LogFactory.getLog(ImportAction.class).error(e1);
		} catch (SimulinkModelBuildingException e2) {
			LogFactory.getLog(ImportAction.class).error(e2);
		}
	}
}


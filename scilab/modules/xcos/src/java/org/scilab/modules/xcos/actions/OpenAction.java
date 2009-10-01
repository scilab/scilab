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

package org.scilab.modules.xcos.actions;

import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;
import java.util.HashMap;
import java.util.List;

import javax.swing.KeyStroke;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.DefaultAction;
import org.scilab.modules.gui.filechooser.FileChooser;
import org.scilab.modules.gui.filechooser.ScilabFileChooser;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.BlockReader;
import org.scilab.modules.xcos.utils.XcosMessages;

public class OpenAction extends DefaultAction {

	private OpenAction(ScilabGraph scilabGraph) {
		super(XcosMessages.OPEN, scilabGraph);
	}

	public static MenuItem createMenu(ScilabGraph scilabGraph) {
		return createMenu(XcosMessages.OPEN, null, new OpenAction(scilabGraph), KeyStroke.getKeyStroke(KeyEvent.VK_O, ActionEvent.CTRL_MASK));
	}

	public static PushButton createButton(ScilabGraph scilabGraph) {
		return createButton(XcosMessages.OPEN, "document-open.png", new OpenAction(scilabGraph));
	}
	
	public void doAction() {
	    FileChooser fc = ScilabFileChooser.createFileChooser();
	    fc.setMultipleSelection(false);
	    fc.displayAndWait();

	    if (fc.getSelection() == null || 
		    fc.getSelection().length == 0 ||
		    fc.getSelection()[0].isEmpty()) {
		return;
	    }
	    String fileName = fc.getSelection()[0];
	    System.out.println("Openning to file : {"+fileName+"}");
	    
	    HashMap<String, List> allObjects = BlockReader.readDiagramFromFile(fileName);
	    
	    List<BasicBlock> allBlocks = allObjects.get("Blocks");
	    
	    for (int i = 0 ; i < allBlocks.size() ; ++i) {
	    	getGraph(null).addCell(allBlocks.get(i));
	    }
	}

}

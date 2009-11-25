/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos;

import java.awt.Color;
import java.awt.EventQueue;
import java.io.File;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.scilab.modules.action_binding.InterpreterManagement;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.xcos.actions.ViewPaletteBrowserAction;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.SuperBlock;
import org.scilab.modules.xcos.block.SuperBlockDiagram;

import org.scilab.modules.xcos.palette.XcosPaletteManager;
import org.scilab.modules.xcos.utils.ConfigXcosManager;

public class Xcos {

	private static Map<String, SuperBlock> openedSuperBlock = new HashMap<String, SuperBlock>();

	/** Palette creation */
    static {
	/* load scicos libraries (macros) */
	InterpreterManagement.requestScilabExec("loadScicosLibs();");
    }
    
    /**
     * @param args
     */
    public static void main(String[] args) {
	EventQueue.invokeLater(new Runnable() {
	    public void run() {
		xcos();
	    }
	});
    }

    public static void xcos() {
	XcosPaletteManager.loadPalette();
	ViewPaletteBrowserAction.setPalettesVisible(true);
	createEmptyDiagram();
    }

    public static void xcos(String fileName) {
	ConfigXcosManager.saveToRecentOpenedFiles(fileName);
	if (XcosTab.focusOnExistingFile(fileName) == false) {
	    XcosDiagram diagram = createEmptyDiagram();
	    diagram.openDiagramFromFile(fileName);
	}
    }

    public static XcosDiagram createEmptyDiagram() {
	XcosDiagram xcosDiagramm = new XcosDiagram();
	xcosDiagramm.installListeners();
	XcosTab.showDiagram(xcosDiagramm);
	return xcosDiagramm;
    }

    public static XcosDiagram createHiddenDiagram() {
	XcosDiagram xcosDiagramm = new XcosDiagram();
	xcosDiagramm.installListeners();
	return xcosDiagramm;
    }

    public static void closeSession() {
	List<XcosDiagram> diagrams = XcosTab.getAllDiagrams();
	for (XcosDiagram diagram :  diagrams) {
	    diagram.closeDiagram();
	}
	ViewPaletteBrowserAction.setPalettesVisible(false);
    }

    public static Tab getPalettes() {
	return XcosPaletteManager.getPalettes();
    }

    public static List<XcosDiagram> getDiagrams() {
	return XcosTab.getAllDiagrams();
    }

    /**
     * Look in each diagram to find the block corresponding to the given uid and
     * display a warning message.
     * 
     * @param UID
     *            - A String as UID.
     * @param message
     *            - The message to display.
     */
    public static void warnCellByUID(String UID, String message) {
	// Try to find a block with given index (UID)
	List<XcosDiagram> allDiagrams = Xcos.getDiagrams();
	for (int i = 0; i < allDiagrams.size(); ++i) {
	    allDiagrams.get(i).warnCellByUID(UID, message);
	}
    }

    /**
     * This method is called when the user exits from Scilab
     */
    public static void closeXcosFromScilab() {
	// call close on all diagrams
	while (XcosTab.getAllDiagrams().size() > 0) {
	    XcosTab.getAllDiagrams().get(0).closeDiagram(true);
	}
    }

    /**
     * This function convert a Xcos diagram to Scilab variable
     */
    public static int xcosDiagramToHDF5(String xcosFile, String h5File,
	    boolean forceOverwrite) {
	XcosDiagram diagram = createHiddenDiagram();
	diagram.openDiagramFromFile(xcosFile);
	File temp = new File(h5File);
	if (temp.exists()) {
	    if (forceOverwrite == false) {
		return 1;
	    } else {
		temp.delete();
	    }
	}

	diagram.dumpToHdf5File(temp.getAbsolutePath());
	diagram.closeDiagram();
	return 0;
    }

        public static void xcosDiagramOpen(String UID, boolean show) {
            BasicBlock block = null;
            List<XcosDiagram> allDiagrams = Xcos.getDiagrams();
            for(XcosDiagram diagram : allDiagrams) {
        	//exclude SuperBlock from parsing 
        	if(diagram instanceof SuperBlockDiagram) {
        	    continue;
        	}

        	block = diagram.getChildById(UID);
        	if(block != null) {
        	    SuperBlock newSP = (SuperBlock)BasicBlock.createBlock("SUPER_f");
        	    newSP.setRealParameters(block.getRealParameters());
        	    newSP.setParentDiagram(block.getParentDiagram());
        	    if(show == true) {
        		newSP.openBlockSettings(null);
        		//lock cells and change background to gray to show read-only
        		newSP.getChild().setCellsLocked(true);
        		newSP.getChild().getAsComponent().setBackground(new Color(204,204,204));
        		//look to disable open setting dialog on double click too
        	    }
        	    openedSuperBlock.put(UID, newSP);
        	    break;
        	}
            }
        }

        public static void xcosDiagramClose(String UID) {
            SuperBlock SP = openedSuperBlock.get(UID);
            if(SP != null) {
        	openedSuperBlock.remove(UID);
        	SP.closeBlockSettings();
        	SP = null;
            }
        }
}
